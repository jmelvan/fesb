#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILENAME 30

typedef struct Node * Polynom;
struct Node{
  int c, e, s; // koeficijent i eksponent
  Polynom next;
};

Polynom* readFromFile(char filename[FILENAME], int *numberOfPols);
int addPolynom(int c, int e, int s, Polynom head);
int display(Polynom *pols, int n);
int show(Polynom head);
Polynom multiply(Polynom* pols, int numberOfPols);
int sum(Polynom forSum);
Polynom addition(Polynom* pols, int numberOfPols);
int deleteAll(Polynom* pols, int numberOfPols);

int main(){
  int numberOfPols = 0, count = 0, loop = 1;
  Polynom *pols = readFromFile("polinomi.txt", &numberOfPols);
  if(pols == NULL) return 0;

  while(loop){
    if(count == 0){
      printf("\n\nPOLINOMI:\n");
      display(pols, numberOfPols);
      printf("\nODABERITE RADNJU S POLINOMIMA:\n"
        "1. Mnozenje svih polinoma\n"
        "2. Zbrajanje svih polinoma\n"
        "3. Ucitaj ponovno iz file-a\n"
        "0. Izlazak iz programa\n");
    }
    int choice = 0;
    printf("\nUnos: ");
    scanf(" %d", &choice);
    switch(choice){
      case 1:
        printf("\nREZULTAT MNOZENJA:\n");
        Polynom result = multiply(pols, numberOfPols);
        show(result);
        printf("\n");
        count++;
      break;
      case 2:
        printf("\nREZULTAT ZBRAJANJA:\n");
        Polynom result2 = addition(pols, numberOfPols);
        show(result2);
        printf("\n");
        count++;
      break;
      case 3:
        pols = readFromFile("polinomi.txt", &numberOfPols);
        printf("\n\nPOLINOMI:\n");
        display(pols, numberOfPols);
        count == 0 && (count++);
      break;
      case 0:
        loop = 0;
      break;
    }
    count >= 3 && (count = 0);
  }
  deleteAll(pols, numberOfPols);
  return 0;
}

int deleteAll(Polynom* pols, int numberOfPols){
  Polynom *start = pols;
  Polynom head = NULL, temp = NULL;
  for(int i = 0; i < numberOfPols; i++){
    head = start[i];
    while (head->next != NULL) {
      temp = head;
      head = head->next;
      free(temp);
    }
  }
  return 0;
}

Polynom* readFromFile(char filename[FILENAME], int *numberOfPols){
  FILE *f = fopen(filename, "r");
  if(f == NULL) return NULL;

  fseek(f, 0, SEEK_END);
  unsigned long len = (unsigned long)ftell(f);
  if(len <= 0) {
    printf("\nOdabrani dokument je prazan.\n");
    return NULL;
  }
  rewind(f);

  int lines = 1;
  while(!feof(f)){
    fgetc(f) == '\n' && lines++;
  }
  rewind(f);

  Polynom *pols = (Polynom*)malloc(lines*sizeof(Polynom));
  for(int i = 0; i < lines; i++){
    Polynom n = (Polynom)malloc(sizeof(struct Node));
    n->next = NULL;
    pols[i] = n;
  }
  *numberOfPols = lines;

  int pol = 0, type = 0, c = 0, e = 0, s = 1;
  char ch;
  while((ch = fgetc(f)) != EOF){
    if(ch != '\n'){
      switch(ch){
        case '+':
          if(c > 0){
            addPolynom(c, e, s, pols[pol]);
            c = 0;
            e = 0;
            type = 0;
          }
          s = 1;
        break;
        case '-':
          if(c > 0){
            addPolynom(c, e, s, pols[pol]);
            c = 0;
            e = 0;
            type = 0;
          }
          s = -1;
        break;
        case 'x': 
          e = 1;
        break;
        case ' ':
        break;
        case '^':
          type = 1;
          e = 0;
        break;
        default:
          if(type == 0){
            c = c*10 + (ch - '0');
          } else if(type == 1){
            e = e*10 + (ch - '0');
          }
        break;
      }
    } else {
      addPolynom(c, e, s, pols[pol]);
      pol++;
      c = 0;
      e = 0;
      s = 1;
      type = 0;
    }
  }
  addPolynom(c, e, s, pols[pol]); // ako je EOF onda se nece dodati zadnji element u listu bez ove linije
  
  fclose(f);
  return pols;
}

Polynom multiply(Polynom *pols, int numberOfPols){
  Polynom forSum = (Polynom)malloc(sizeof(struct Node)), p1, p2;
  Polynom *sumfor = (Polynom*)malloc((numberOfPols-1)*sizeof(Polynom));
  for(int i = 0; i < numberOfPols-1; i++){
    Polynom n = (Polynom)malloc(sizeof(struct Node));
    n->next = NULL;
    sumfor[i] = n;
  }
  Polynom *start = pols;
  forSum->next = NULL;
  int c, e, s;
  for(int i = 0; i < numberOfPols; i++){
    for(int j = i+1; j < numberOfPols; j++){
      p1 = (i == 0) ? start[i]->next : sumfor[i-1];
      p2 = start[j]->next;
      while(p1 != NULL){
        while(p2 != NULL){
          c = p1->c * p2->c;
          e = p1->e + p2->e;
          s = p1->s * p2->s;
          addPolynom(c, e, s, sumfor[i]);
          p2 = p2->next;
        }
        p2 = start[j]->next;
        p1 = p1->next;
      }
    }
    show(sumfor[i]);
    sum(sumfor[i]);
    show(sumfor[i]);
  }
  sum(sumfor[numberOfPols-2]);
  return sumfor[numberOfPols-2];
}

Polynom addition(Polynom* pols, int numberOfPols){
  Polynom forSum = (Polynom)malloc(sizeof(struct Node)), pol;
  forSum->next = NULL;
  Polynom *start = pols;
  for(int i = 0; i < numberOfPols; i++){
    pol = start[i]->next;
    while(pol != NULL){
      addPolynom(pol->c, pol->e, pol->s, forSum);
      pol = pol->next;
    }
  }
  sum(forSum);
  return forSum;
}

int sum(Polynom forSum){
  Polynom head = forSum->next, last = forSum;
  Polynom forDel = NULL;
  while(head->next != NULL){
    if(head->e == head->next->e){
      head->c += head->next->s*head->next->c;
      forDel = head->next;
      head->next = head->next->next;
      free(forDel);
      if(head->c == 0){
        forDel = head;
        last->next = head->next;
        free(forDel);
      }
    } else {
      last = head;
      head = head->next;
    }
  }
  return 0;
}

int addPolynom(int c, int e, int s, Polynom head){
  Polynom after = head->next;
  while(after != NULL && e < after->e){
    head = after;
    after = after->next;
  }
  Polynom new = (Polynom)malloc(sizeof(struct Node));
  new->c = c;
  new->e = e;
  new->s = s;
  new->next = after;
  head->next = new;
  return 0;
}

int display(Polynom *pols, int n){
  Polynom *start = pols;
  for(int i = 0; i < n; i++){
    printf("%d. ", i+1);
    show(start[i]);
  }
  return 0;
}

int show(Polynom head){
  if(head != NULL){
    int counter = 0;
		while (head->next != NULL) {
			head = head->next;
      if(head->s == -1 && counter == 0) printf("-");
      counter++;
			printf("%dx^%d", head->c, head->e);
      if(head->next != NULL) printf(" %c ", head->next->s == 1 ? '+' : '-');
		}
		printf("\n");
		return 0;
	}
	return -1;
}