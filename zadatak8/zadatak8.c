/*
  Naredbe za korištenje:
  md <ime_direktorija> - kreira novi direktorij
  cd <ime_direktorija> - ulazi u željeni direktorij
  cd.. - ide u parent direktorij (treba paziti da nema razmaka između cd i .. kao u standardnoj konzoli)
  dir - ispisuje poddirektorije trenutnog direktorija
  tree - ispisuje stablo svih poddirektorija root direktorija
  exit - izlaz iz programa
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER 255
#define CMD 15

typedef struct { char *key; int val; } lookup_format;
static lookup_format lookuptable[] = {
    { "md", 1 }, { "cd", 2 }, { "cd..", 3 }, { "dir", 4 }, { "tree", 5 }, { "exit", 6 }
};

#define NK (sizeof(lookuptable)/sizeof(lookup_format))

// funkcija za citanje vrijednosti iz lookup tablice naredbi
int lookup(char *key){
  for(int i = 0; i < NK; i++) {
    lookup_format *temp = &lookuptable[i];
    if (strcmp(temp->key, key) == 0)
      return temp->val;
  }
  return 0;
}

typedef struct Node * Edge;
struct Node{
  char name[BUFFER];
  Edge parent;
  Edge sibling;
  Edge child;
};

Edge newEdge();
int md(char*, Edge);
int display_tree(Edge, int);
int tree(Edge);
int dir(Edge);
int cd(Edge*, char*);
int cd2dot(Edge*);
int parseInput(char*, Edge*);
int execCMD(char*, Edge*, char*);
int deleteAll(Edge root);
int printPosition(Edge position);

int main(){
  Edge root = newEdge();
  Edge position = root;
  char buffer[BUFFER];
  int loop = 1;
  while(loop){
    printPosition(position);
    printf("> ");
    scanf(" %[^\n]", buffer);
    switch(parseInput(buffer, &position)){
      case 5:
        tree(root);
      break;
      case 6:
        loop = 0;
      break;
    }
  }
  deleteAll(root);
  return 0;
}

/*
  Funkcija koja prije scanf-a ispisuje trenutnu lokaciju
  npr. root:\>
  npr. root:\first\>
*/
int printPosition(Edge position){
  position->parent != NULL ? printPosition(position->parent) : printf("root:\\");
  position->parent != NULL && printf("%s\\", position->name);
  return 0;
}

// funkcija za odvajanje naredbe i vrijednosti nakon naredbe
int parseInput(char input[BUFFER], Edge* position){
  char command[CMD];
  int offset = 0;
  sscanf(input, "%s%n", command, &offset);
  strlen(input) > offset && (input += offset+1);
  return execCMD(command, position, input);
}

// funkcija za izvrsavanje naredbe preko lookup tablice
int execCMD(char command[CMD], Edge* position, char name[BUFFER]){
  switch(lookup(command)){
    case 1:
      md(name, *position);
    break;
    case 2:
      cd(position, name);
    break;
    case 3:
      cd2dot(position);
    break;
    case 4:
      dir(*position);
    break;
    case 5:
      return 5;
    break;
    case 6:
      return 6;
    break;
    default:
      printf("\n`%s` nije prepoznata kao naredba.\n\n", command);
    break;
  }
  return 0;
}

// funckija za stvaranje novog ruba
Edge newEdge(){
  Edge new = (Edge)malloc(sizeof(struct Node));
  new->parent = NULL;
  new->sibling = NULL;
  new->child = NULL;
  return new;
}

int cd(Edge* position, char name[BUFFER]){
  Edge temp = (*position)->child;
  while(temp != NULL && strcmp(temp->name, name) != 0)
    temp = temp->sibling;
  if(temp != NULL){
    *position = temp;
  } else {
    printf("\nDirektorij `%s` ne postoji.\n\n", name);
    return -1;
  }
  return 0;
}

// cd..
int cd2dot(Edge* position){
  if((*position)->parent != NULL){
    *position = (*position)->parent;
  } else {
    printf("\nNalazite se u root direktoriju.\n");
    return -1;
  }
  return 0;
}

int md(char name[BUFFER], Edge position){
  if(position->child != NULL){
    Edge temp = position->child;
    while(temp->sibling != NULL)
      temp = temp->sibling;
    temp->sibling = newEdge();
    temp->sibling->parent = position;
    strcpy(temp->sibling->name, name);
  } else{
    position->child = newEdge();
    position->child->parent = position;
    strcpy(position->child->name, name);
  }
  return 0;
}

int dir(Edge position){
  printf("\nSadržaj direktorija `%s`:\n", strlen(position->name) != 0 ? position->name : "root");
  Edge temp = position->child;
  while(temp != NULL){
    printf("|-%s\n", temp->name);
    temp = temp->sibling;
  }
  printf("\n");
  return 0;
}

int tree(Edge position){
  printf("\n");
  display_tree(position, 0);
  return 0;
}

// funkcija za formatirani ispis tree-a rekurzivno
int display_tree(Edge position, int depth){
  Edge temp = position;
  while(1){
    for(int i = 0; i < depth; i++)
      depth != 1 ? printf("   |") : printf("   |\n   |");
    depth != 0 ? printf("--%s\n", temp->name) : printf(" root\n");
    temp->child != NULL && display_tree(temp->child, depth+1);
    if(temp->sibling != NULL){
      temp = temp->sibling;
    } else { break; }
  }
  depth > 1 ? printf("   |\n") : printf("\n");
  return 0;
}

// brisanje stabla
int deleteAll(Edge root){
  Edge temp = NULL;
  while(1){
    root->child != NULL && deleteAll(root->child);
    if(root->sibling != NULL){
      temp = root;
      root = root->sibling;
      free(temp);
    } else { break; }
  }
  return 0;
}