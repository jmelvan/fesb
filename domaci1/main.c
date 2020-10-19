#include <stdio.h>
#include <stdlib.h>
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

  int lines = 1; // jer se \n ne nalazi na zadnjoj liniji, a nju isto treba prebrojati
  while(!feof(f)){
    if(fgetc(f) == '\n')
      lines++;
  }
  rewind(f);

  Student *studenti = (Student *)malloc(lines*sizeof(Student)); // svaki student je u svojoj liniji u file-u

  printf("Ime\tPrezime\tBodovi\tRelativni bodovi\n");
  
  // za broj linija provuci funkciju fscanf kroz file
  for(int i = 0; i < lines; i++){
    fscanf(f, " %s %s %d", studenti[i].name, studenti[i].surname, &studenti[i].points);
    // ispis vrsimo odmah kako nebi ponovno morali zavrtiti for/while petlju
    printf("%s\t%s\t%d\t%.0f\n", studenti[i].name, studenti[i].surname, studenti[i].points, ((double)studenti[i].points/50)*100); // za maksimalan broj bodova uzeto je 50
  }

  fclose(f);
  return 0;
}