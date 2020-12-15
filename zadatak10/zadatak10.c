#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFFER 255

typedef struct Node * Edge;
struct Node{
  int number;
  char operand;
  Edge L, R, next;
};

Edge readFromFile(char*);
Edge parseLine(char*);
Edge newEdge();
Edge pop(Edge);
int push(Edge, int, char, int);
int isOperand(char);
int writeToFile(char*, Edge);
int formatInfix(char*, Edge);
int hasChildren(Edge);

int main(){
  Edge root = readFromFile("postfix.txt");
  writeToFile("infix.txt", root);
  return 0;
}

Edge readFromFile(char *filename){
  FILE *f = fopen(filename, "r");
  if(f == NULL) return NULL;

  char buffer[BUFFER];
  fgets(buffer, BUFFER, f);

  fclose(f);
  return parseLine(buffer);
}

Edge parseLine(char *buffer){
  Edge root = newEdge();

  int number = 0, offset = 0;
  while(sscanf(buffer, " %d %n", &number, &offset) > 0){
    buffer += offset;
    push(root, number, '\0', 0);
    while(*buffer != '\0' && !isdigit(*buffer)){
      isOperand(*buffer) && push(root, 0, *buffer, 1);
      buffer++;
    }
  }

  return root;
}

Edge newEdge(){
  Edge new = (Edge)malloc(sizeof(struct Node));
  if(new == NULL) { printf("\nMalloc failed\n"); return NULL; }
  new->L = NULL;
  new->R = NULL;
  new->next = NULL;
  return new;
}

Edge pop(Edge position){
  if(position == NULL || position->next == NULL) return NULL;
  Edge temp = position->next;
	position->next = position->next->next; 
	temp->next = NULL;
  return temp;
}

int push(Edge position, int n, char o, int type){
  if (position == NULL) return -1;
  Edge new = newEdge();
  if(type){
    new->operand = o;
    new->R = pop(position);
    new->L = pop(position);
  } else {
    new->number = n;
    new->operand = o;
  }
  new->next = position->next;
  position->next = new;
  return 0;
}

int isOperand(char c){
  return (c > 39 && c < 48) ? 1 : 0;
}

int writeToFile(char *filename, Edge position){
	if(position == NULL) return -1;
	FILE *f = fopen(filename, "w");
	if(f == NULL) return -1;

	char buffer[BUFFER] = { '\0' };
	formatInfix(buffer, position->next);
	fprintf(f, "%s", buffer);

	fclose(f);
	return 0;
}

int formatInfix(char *buffer, Edge position){
  if(position == NULL) return 0;
  hasChildren(position) && snprintf(buffer, BUFFER-strlen(buffer), "( %s", buffer);
  formatInfix(buffer, position->L);
	isOperand(position->operand) ? snprintf(buffer, BUFFER-strlen(buffer), "%s %c", buffer, position->operand) : snprintf(buffer, BUFFER-strlen(buffer), "%s %d", buffer, position->number);
  formatInfix(buffer, position->R);
	hasChildren(position) && snprintf(buffer, BUFFER-strlen(buffer), "%s )", buffer);
  return 0;
}

int hasChildren(Edge position){
  return position->L != NULL && position->R != NULL ? 1 : 0;
}