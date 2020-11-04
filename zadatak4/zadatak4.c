#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILENAME 30

typedef struct Node * Polynom;
struct Node{
  int c, e; // koeficijent i eksponent
  Polynom next;
};

Polynom* readFromFile(char filename[FILENAME], int *numberOfPols);
int addPolynom(int c, int e, Polynom head);
int display(Polynom *pols, int n);
int show(Polynom head);
Polynom multiply(Polynom* pols, int numberOfPols);
int sum(Polynom forSum);
Polynom addition(Polynom* pols, int numberOfPols);

int main(){
  int numberOfPols = 0;
  Polynom *pols = readFromFile("polinomi.txt", &numberOfPols);
  printf("\n\nPOLINOMI:\n");
  display(pols, numberOfPols);

  printf("\n\nREZULTAT MNOZENJA:\n");
  Polynom result = multiply(pols, numberOfPols);
  show(result);
  printf("\n");

  printf("\n\nREZULTAT ZBRAJANJA:\n");
  Polynom result2 = addition(pols, numberOfPols);
  show(result2);
  printf("\n");
  return 0;
}

Polynom addition(Polynom* pols, int numberOfPols){
  Polynom forSum = (Polynom)malloc(sizeof(struct Node));
  forSum->next = NULL;
  Polynom start = *pols, pol;
  for(int i = 0; i < numberOfPols; i++){
    pol = (&start[i])->next;
    while(pol != NULL){
      addPolynom(pol->c, pol->e, forSum);
      pol = pol->next;
    }
  }
  sum(forSum);
  return forSum;
}

Polynom* readFromFile(char filename[FILENAME], int *numberOfPols){
  FILE *f = fopen(filename, "r");
  if(f == NULL) return NULL;

  int lines = 1;
  while(!feof(f)){
    fgetc(f) == '\n' && lines++;
  }
  rewind(f);

  Polynom *pols = (Polynom*)malloc(lines*sizeof(Polynom));
  for(int i = 0; i < lines; ++i){
    Polynom n = (Polynom)malloc(sizeof(struct Node));
    n->next = NULL;
    pols[i] = n;
  }
  *numberOfPols = lines;

  int pol = 0, type = 0, c = 0, e = 0;
  char ch;
  while((ch = fgetc(f)) != EOF){
    if(ch != '\n'){
      switch(ch){
        case '+':
          addPolynom(c, e, pols[pol]);
          c = 0;
          e = 0;
          type = 0;
        break;
        case 'x':
        break;
        case '^':
          type = 1;
        break;
        default:
          if(type == 0){
            c = c*10 + (ch - '0');
          } else {
            e = e*10 + (ch - '0');
          }
        break;
      }
    } else {
      addPolynom(c, e, pols[pol]);
      pol++;
      c = 0;
      e = 0;
      type = 0;
    }
  }
  addPolynom(c, e, pols[pol]); // ako je EOF onda se nece dodati zadnji element u listu bez ove linije

  fclose(f);
  return pols;
}

Polynom multiply(Polynom *pols, int numberOfPols){
  Polynom forSum = (Polynom)malloc(sizeof(struct Node)), p1, p2;
  Polynom start = *pols;
  forSum->next = NULL;
  int c, e;
  for(int i = 0; i < numberOfPols; i++){
    for(int j = i+1; j < numberOfPols; j++){
      p1 = (&start[i])->next;
      p2 = (&start[j])->next;
      while(p1 != NULL){
        while(p2 != NULL){
          c = p1->c * p2->c;
          e = p1->e + p2->e;
          addPolynom(c, e, forSum);
          p2 = p2->next;
        }
        p2 = (&start[j])->next;
        p1 = p1->next;
      }
    }
  }
  sum(forSum);
  return forSum;
}

int sum(Polynom forSum){
  Polynom head = forSum->next;
  Polynom forDel = NULL;
  while(head->next != NULL){
    if(head->e == head->next->e){
      head->c += head->next->c;
      forDel = head->next;
      head->next = head->next->next;
      free(forDel);
    } else {
      head = head->next;
    }
  }
  return 0;
}

int addPolynom(int c, int e, Polynom head){
  Polynom after = head->next;
  while(after != NULL && e < after->e){
    head = after;
    after = after->next;
  }
  Polynom new = (Polynom)malloc(sizeof(struct Node));
  new->c = c;
  new->e = e;
  new->next = after;
  head->next = new;
  return 0;
}

int display(Polynom *pols, int n){
  Polynom start = *pols;
  for(int i = 0; i < n; i++){
    printf("%d. ", i+1);
    show(&start[i]);
  }
  return 0;
}

int show(Polynom head){
  if(head != NULL){
		while (head->next != NULL) {
			head = head->next;
			printf("%dx^%d", head->c, head->e);
      if(head->next != NULL) printf(" + ");
		}
		printf("\n");
		return 0;
	}
	return -1;
}