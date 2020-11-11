#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER 256

typedef struct Node * List;
struct Node{
  int br;
  List next;
};

typedef struct holder * ListHolder;
struct holder{
  List head;
  ListHolder next;
};

ListHolder readFromFile(char *filename);
ListHolder parseLine(char *buffer, List head, ListHolder LH);
ListHolder init();
List Union(ListHolder LH);
List Intersection(ListHolder LH);
int addNode(List head, int br);
int display(ListHolder LH);
int show(List L);

int main(){
  ListHolder LH = readFromFile("liste.txt");
  display(LH);

  List U = Union(LH);
  printf("\nUNIJA:\n");
  show(U);

  List I = Intersection(LH);
  printf("\nPRESJEK:\n");
  show(I);

  return 0;
}

List Intersection(ListHolder LH){
  List I = (List)malloc(sizeof(struct Node));
  I->next = NULL;

  ListHolder start = LH;
  while(start->next != NULL){
    List L1 = start->head->next;
    List L2 = start->next->head->next;
    while(1){
      if(L1 == NULL || L2 == NULL) break;
      L1->br > L2->br && (L2 = L2->next);
      L1->br < L2->br && (L1 = L1->next);
      L1->br == L2->br && (addNode(I, L1->br), L1 = L1->next, L2 = L2->next);
    }
    start = start->next;
  }

  return I;
}

ListHolder readFromFile(char *filename){
  FILE *f = fopen(filename, "r");
  if(f == NULL) return NULL;

  ListHolder LH = init();
  ListHolder start = LH;

  char buffer[BUFFER];
  while(fgets(buffer, BUFFER, f) != NULL){
    ListHolder new = parseLine(buffer, LH->head, LH);
    LH->next = new;
    LH = LH->next;
  }

  fclose(f);
  return start;
}

ListHolder parseLine(char *buffer, List head, ListHolder LH){
  int br = 0, offset = 0;
  while(sscanf(buffer, " %d,%n%*[^\n]", &br, &offset) == 1){
    buffer += offset;
    addNode(head, br);
  }
  ListHolder new = init();
  return new;
}

int addNode(List head, int br){
  if(head == NULL) return -1;
  List after = head->next;
  while(after != NULL && br >= after->br){
    head = after;
    after = after->next;
  }
  if(br != head->br){
    List new = (List)malloc(sizeof(struct Node));
    new->br = br;
    new->next = after;
    head->next = new;
  }
  return 0;
}

ListHolder init(){
  ListHolder LH = (ListHolder)malloc(sizeof(struct holder));
  LH->head = (List)malloc(sizeof(struct Node));
  LH->head->next = NULL;
  LH->next = NULL;
  return LH;
}

List Union(ListHolder LH){
  List U = (List)malloc(sizeof(struct Node));
  U->next = NULL;

  ListHolder start = LH;
  while(start->next != NULL){
    List head = start->head;
    while(head->next != NULL){
      head = head->next;
      addNode(U, head->br);
    }
    start = start->next;
  }

  return U;
}

int display(ListHolder LH){
  ListHolder start = LH;
  printf("\nLISTE:\n\n");
  while(start->next != NULL){
    List head = start->head;
    while(head->next != NULL){
      head = head->next;
      printf("%d", head->br);
      if(head->next != NULL) printf(", ");
    }
    start = start->next;
    printf("\n");
  }
  printf("\n");
  return 0;
}

int show(List L){
  if(L == NULL) return -1;
  List Show = L;
  while(Show->next != NULL){
    Show = Show->next;
    printf("%d\t", Show->br);
  }
  printf("\n\n");
  return 0;
}