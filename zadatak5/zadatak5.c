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
ListHolder parseLine(char *buffer, List head);
ListHolder init();
List Union(ListHolder LH);
List Intersection(ListHolder LH);
int addNode(List head, int br);
int display(ListHolder LH);
int show(List L);
int deleteHolder(ListHolder LH);
int deleteList(List head);

int main(){
  ListHolder LH = readFromFile("liste.txt");
  display(LH);

  List U = Union(LH);
  printf("\nUNIJA:\n");
  show(U);

  List I = Intersection(LH);
  printf("\nPRESJEK:\n");
  show(I);

  deleteHolder(LH);
  deleteList(U);
  deleteList(I);

  return 0;
}

/*
	INPUT: [char filename](Ime datoteke)
	RETURN: [ListHolder](Sadrzi head elemente lista ucitanih iz file-a)
	DESCRIPTION: ucitavanje lista iz file-a, te stvaranje vezane liste za pohranjivanje head elemenata pojedinih lista
*/
ListHolder readFromFile(char *filename){
  FILE *f = fopen(filename, "r");
  if(f == NULL) return NULL;

  ListHolder LH = init();
  ListHolder start = LH;

  char buffer[BUFFER];
  while(fgets(buffer, BUFFER, f) != NULL){
    ListHolder new = parseLine(buffer, LH->head);
    LH->next = new;
    LH = LH->next;
  }

  fclose(f);
  return start;
}

/*
	INPUT: [char *buffer](Ucitani buffer), [List head](head element liste u kojeg se unosi lista)
	RETURN: [ListHolder](Novi alocirani ListHolder spreman za unos sljedece liste)
	DESCRIPTION: parsiranje linije i spremanje liste u strukturu
*/
ListHolder parseLine(char *buffer, List head){
  int br = 0, offset = 0;
  while(sscanf(buffer, " %d,%n%*[^\n]", &br, &offset) == 1){
    buffer += offset;
    addNode(head, br);
  }
  ListHolder new = init();
  return new;
}

/*
	INPUT: [List head](head element liste kojem element pripada), [int br](Vrijednost elementa)
	RETURN: success or error
	DESCRIPTION: sortirani unos liste
*/
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

/*
	RETURN: [ListHolder](Alocirani ListHolder)
	DESCRIPTION: alocira memoriju za novi ListHolder i head element, te stavlja vrijednosti next i head->next u NULL
*/
ListHolder init(){
  ListHolder LH = (ListHolder)malloc(sizeof(struct holder));
  LH->head = (List)malloc(sizeof(struct Node));
  LH->head->next = NULL;
  LH->next = NULL;
  return LH;
}

/*
	INPUT: [ListHolder LH](Vezana lista vezanih lista)
	RETURN: [List](lista koja sadrzi vrijednosti presjeka)
	DESCRIPTION: trazenje presjeka
*/
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

/*
	INPUT: [ListHolder LH](Vezana lista vezanih lista)
	RETURN: [List](lista koja sadrzi vrijednosti unije)
	DESCRIPTION: trazenje unije
*/
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

/*
	INPUT: [ListHolder LH](Vezana lista head elemenata vezanih lista)
	RETURN: success or error
	DESCRIPTION: slanje head elemenata iz liste u deleteList() i brisanje ListHolder-a
*/
int deleteHolder(ListHolder LH){
  ListHolder start = LH, temp = NULL;
  while(start->next != NULL){
    deleteList(start->head);
    temp = start;
    start = start->next;
    free(temp);
  }
  return 0;
}

/*
	INPUT: [List head](Head element liste)
	RETURN: success or error
	DESCRIPTION: brisanje liste iz memorije
*/
int deleteList(List head){
  List temp = NULL, start = head;
  while(start->next != NULL){
    temp = start;
    start = start->next;
    free(temp);
  }
  return 0;
}