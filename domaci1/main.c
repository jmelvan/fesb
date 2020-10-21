#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_NAME 255

typedef struct student{
  char name[MAX_NAME];
  char surname[MAX_NAME];
  int points;
} Student;

int main(){

  FILE *f = fopen("studenti.txt", "r");
  if(f == NULL){
    printf("Greska pri ucitavanju datoteke.\n");
    return 0;
  }

  int lines = 1;

  while(!feof(f)){
    fgetc(f) == '\n' && lines++;
  }

  rewind(f);

  Student *studenti = (Student *)malloc(lines*sizeof(Student));

  printf("Ime\tPrezime\tBodovi\tRelativni bodovi\n");

  int max_br_bod = 0;
  for(int i = 0; i < lines; i++){
    fscanf(f, " %s %s %d", studenti[i].name, studenti[i].surname, &studenti[i].points);
    studenti[i].points > max_br_bod && (max_br_bod = studenti[i].points);
  }

  for(int i = 0; i < lines; i++){
    printf("%s\t%s\t%d\t%.0f %%\n", studenti[i].name, studenti[i].surname, studenti[i].points, ((double)studenti[i].points/max_br_bod)*100);
  }

  fclose(f);
  return 0;
}