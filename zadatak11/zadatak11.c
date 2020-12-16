#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER 255
#define TABLE_SIZE 11

typedef struct _student * student;
struct _student{
  int maticni_broj;
  char name[BUFFER], surname[BUFFER];
  student next;
};

student* initTable();
student createNewStudent(char*);
student initStudent();
student search(char*, char*, student*);
int readFromFile(char*, student*);
int calculateHashKey(char*);
int assignStudent(student, student*);
int sortedInsertion(student, student);
int display(student*);
int offerSearch(student*);
int searchInput(student*);
int deleteAll(student*);

int main(){
  student* studenti = initTable();
  readFromFile("studenti.txt", studenti);
  display(studenti);
  offerSearch(studenti);
  deleteAll(studenti);
  return 0;
}

student* initTable(){
  student* studenti = (student*)malloc(TABLE_SIZE*sizeof(student));
  for(int i = 0; i < TABLE_SIZE; i++){
    studenti[i] = (student)malloc(sizeof(struct _student));
    studenti[i]->next = NULL;
  }
  return studenti;
}

int readFromFile(char* filename, student* studenti){
  FILE *f = fopen(filename, "r");
  if(f == NULL) return -1;

  char buffer[BUFFER];
  while(fgets(buffer, BUFFER, f) != NULL){
    student new = createNewStudent(buffer);
    assignStudent(new, studenti);
  }

  fclose(f);
  return 0;
}

student createNewStudent(char* buffer){
  student s = initStudent();
  int mb;
  char name[BUFFER], surname[BUFFER];
  sscanf(buffer, " %d %s %s", &mb, name, surname);
  s->maticni_broj = mb;
  strcpy(s->name, name);
  strcpy(s->surname, surname);
  return s;
}

student initStudent(){
  student s = (student)malloc(sizeof(struct _student));
  if(s == NULL) exit(1);
  s->next = NULL;
  return s;
}

int assignStudent(student s, student* studenti){
  int hashKey = calculateHashKey(s->surname);
  int position = hashKey % TABLE_SIZE;
  sortedInsertion(studenti[position], s);
  return 0;
}

int calculateHashKey(char* surname){
  int hash = 0, count = 0;
  while(count < 5){
    for(int i = 0; i < strlen(surname)-1; i++){
      hash += surname[i];
      count++;
      if(count > 4) break;
    }
  }
  return hash;
}

int sortedInsertion(student position, student s){
  while(position->next != NULL && position->next->next != NULL){
    if(strcmp(position->next->surname, s->surname) > 0 || (strcmp(position->next->surname, s->surname) == 0 && strcmp(position->next->name, s->name) > 0))
      break;
    position = position->next;
  }
  s->next = position->next;
  position->next = s;
  return 0;
}

int display(student* studenti){
  printf("\nBr.\t\tHash\t\tIme\t\tPrezime\n\n");
  for(int i = 0; i < TABLE_SIZE; i++){
    student temp = studenti[i];
    while(temp->next != NULL){
        printf(
            "%d.\t\t%d\t\t%s\t\t%s\n",
            i+1,
            calculateHashKey(temp->next->surname),
            temp->next->name,
            temp->next->surname
        );
        temp = temp->next;
    }
  }
  return 0;
}

int offerSearch(student* studenti){
  char c;
  printf("\nZelite li ispisati maticni broj odredenog studenta (y/n): ");
  while((c = getchar()) == '\n');
  if(c == 'y')
    searchInput(studenti);
  else
    return -1;
  return 0;
}

int searchInput(student* studenti){
  char name[BUFFER], surname[BUFFER];
  printf("\nUnesite ime i prezime studenta: ");
  scanf(" %s %s", name, surname);
  student searched = search(name, surname, studenti);
  if(searched != NULL){
    printf("\nStudent %s %s ima maticni broj `%d`.\n", name, surname, searched->maticni_broj);
  } else {
    printf("\nPretrazeni student ne postoji.\n");
  }
  offerSearch(studenti);
  return 0;
}

student search(char *name, char* surname, student* studenti){
  int hash = calculateHashKey(surname);
  int position = hash % TABLE_SIZE;
  student temp = studenti[position];
  while(temp->next != NULL){
    temp = temp->next;
    if(strcmp(temp->surname, surname) == 0 && strcmp(temp->name, name) == 0){
      return temp;
    }
  }
  return NULL;
}

int deleteAll(student* studenti){
  for(int i = 0; i < TABLE_SIZE; i++){
    while(studenti[i]->next != NULL){
      student temp = studenti[i]->next;
      studenti[i]->next = studenti[i]->next->next;
      free(temp);
    }
  }
  free(studenti);
  return 0;
}