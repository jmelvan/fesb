/*
  Polinomi mogu biti svih oblika (npr. 2x^4 + 3x - 4)...
  Ukoliko iza X-a nema '^' i vrijednost eksponenta, podrazumijeva se da je x na prvu, odnosno x^1
  Program obavlja operaciju i oduzimanja polinoma ukoliko neki polinom sadrži negativne članove
  Broj polinoma se ne ogranicava na 2, vec je program napravljen da ih moze racunati dok ne nestane memorije
  Svaki polinom ide u svoj red, podrazumijeva se da nece biti praznih redova izmedu polinoma
*/ 

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILENAME 30

typedef struct Node * Polynom;
struct Node{
  int c, e, s; // koeficijent, eksponent i sign
  Polynom next;
};

Polynom* readFromFile(char filename[FILENAME], int *numberOfPols);
int getNumberOfPolynoms(char *buffer);
int parseData(Polynom* pols, char *buffer);
int addPolynom(int c, int e, int s, Polynom head);
int display(Polynom *pols, int n);
int show(Polynom head);
Polynom multiply(Polynom* pols, int numberOfPols);
Polynom addition(Polynom* pols, int numberOfPols);
int sum(Polynom forSum);
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

/*
	INPUT: [char filename](Ime datoteke), [int* numberOfPols](pokazivac na broj polinoma kako bi mogli izmjeniti varijablu u mainu)
	RETURN: array pokazivaca na polinome
	DESCRIPTION: ucitavanje polinoma iz file-a, te stvaranje alociranog niza za pohranjivanje head elemenata pojedinih polinoma
*/
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

  char *buffer = (char*)malloc(len*sizeof(char));
  fread(buffer, sizeof(char), len, f);

  int nPolynoms = getNumberOfPolynoms(buffer); // broj linija odnosno polinoma (potrebno za alokaciju)
  *numberOfPols = nPolynoms; // kako bi u mainu znali koliko ima polinoma (za koristenje for petlje itd...)

  Polynom *pols = (Polynom*)malloc(nPolynoms*sizeof(Polynom)); // koristen niz zbog indeksiranja, olaksava programiranje
  for(int i = 0; i < nPolynoms; i++){ // inicijalizacija head elemenata
    Polynom n = (Polynom)malloc(sizeof(struct Node));
    n->next = NULL;
    pols[i] = n;
  }

  parseData(pols, buffer); // parsiranje buffera, spremanje polinoma u vezane liste
  
  fclose(f);
  return pols;
}

/*
	INPUT: [Polynom *pols](Niz head elemenata), [char *buffer](sadrzaj file-a)
	RETURN: error or success
	DESCRIPTION: parsiranje polinoma i spremanje u strukture
*/
int parseData(Polynom *pols, char *buffer){
  int pol = 0, type = 0, c = 0, e = 0, s = 1, i = 0;
  char ch = buffer[0];
  while(ch != '\0'){
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
          c == 0 && c++;
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
    ch = buffer[++i];
  }
  addPolynom(c, e, s, pols[pol]); // ako je EOF onda se nece dodati zadnji element u listu bez ove linije
  return 0;
}

/*
	INPUT: [char *buffer](sadrzaj file-a)
	RETURN: [int](broj polinoma)
	DESCRIPTION: brojanje polinoma iz file-a
*/
int getNumberOfPolynoms(char *buffer){
  char c = buffer[0];
  int polynoms = 1, i = 0;
  while(c != '\0'){
    c == '\n' && polynoms++;
    c = buffer[++i];
  }
  return polynoms;
}

/*
	INPUT: [Polynom *pols](Niz head elemenata), [int numberOfPols](broj polinoma)
	RETURN: [Polynom](Rezultat mnozenja)
	DESCRIPTION: mnozenje polinoma
    npr.
      3  \
          (6x) \
      2x /      (12x^4+18x)
      2x^3+3 __/
*/
Polynom multiply(Polynom *pols, int numberOfPols){
  Polynom p1, p2;
  Polynom *forSum = (Polynom*)malloc((numberOfPols-1)*sizeof(Polynom));
  for(int i = 0; i < numberOfPols-1; i++){
    Polynom n = (Polynom)malloc(sizeof(struct Node));
    n->next = NULL;
    forSum[i] = n;
  }
  Polynom *start = pols;
  int c, e, s;
  for(int i = 0; i < numberOfPols-1; i++){
    p1 = (i == 0) ? start[i]->next : forSum[i-1]->next;
    p2 = start[i+1]->next;
    while(p1 != NULL){
      while(p2 != NULL){
        c = p1->c * p2->c;
        e = p1->e + p2->e;
        s = p1->s * p2->s;
        addPolynom(c, e, s, forSum[i]);
        p2 = p2->next;
      }
      p2 = start[i+1]->next;
      p1 = p1->next;
    }
    sum(forSum[i]);
  }
  sum(forSum[numberOfPols-2]);
  return forSum[numberOfPols-2];
}

/*
	INPUT: [Polynom *pols](Niz head elemenata), [int numberOfPols](broj polinoma)
	RETURN: [Polynom](Rezultat zbrajanja)
	DESCRIPTION: stvara novi niz u kojeg spremi sortirano clanove za zbrajanje
*/
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

/*
	INPUT: [Polynom *forSum](Vezana lista polinoma za zbrajanje)
	RETURN: error or success
	DESCRIPTION: zbrajanje polinoma
    npr.
      2x^2 + 3x^2 + 4x^2 + 3
      |_________| 
      5x^2 + 4x^2 + 3
      |_________|
      9x^2 + 3
*/
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

/*
	INPUT: coefficient, exponent, sign, [Polynom head](head element polinoma kojem element pripada)
	RETURN: success or error
	DESCRIPTION: sortirani unos polinoma
*/
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

/*
	INPUT: [Polynom *pols](Niz head elemenata), [int n](broj polinoma)
	RETURN: success or error
	DESCRIPTION: razdvajanje na polinoma po redovima, kruzenje kroz niz head elemenata i slanje na ispis
*/
int display(Polynom *pols, int n){
  Polynom *start = pols;
  for(int i = 0; i < n; i++){
    printf("%d. ", i+1);
    show(start[i]);
  }
  return 0;
}

/*
	INPUT: [Polynom head](head element polinoma)
	RETURN: success or error
	DESCRIPTION: ispisivanje polinoma, tj vezane liste
*/
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

/*
	INPUT: [Polynom *pols](Niz head elemenata), [int numberOfPols](broj polinoma)
	RETURN: success or error
	DESCRIPTION: brisanje polinoma iz memorije
*/
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
  free(head);
  free(pols);
  return 0;
}