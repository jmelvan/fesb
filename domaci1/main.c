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

  char c;
  int lines = 1, // odmah smo pribrojali zadnju liniju file-a
      max_br_bod = 0, 
      br_bod = 0,
      bod_studenta[2] = {0, 0}, // zbog pretpostavke da test ima najvise 99 bodova (da ima preko 99, bila bi 3 clana array-a)
      position = 0;

  while(!feof(f)){
    c = fgetc(f);
    // provjera je li se radi o znamenci ili slovu (Ime i prezime nemaju znamenke, samo bodovi imaju)
    if(c > 47 && c < 58){
      bod_studenta[position] = c - '0'; // c - '0' kako se char ne bi spremio kao ascii kod
      position++; // povecavanje indexa arraya
    } else if(c == '\n'){
      br_bod = bod_studenta[0]*pow(10, position-1) + bod_studenta[1]*pow(10, position-2); // s obzirom da su samo dvije znamenke, ovaj nacin je efikasniji nego da radimo preko for loopa+
      br_bod > max_br_bod && (max_br_bod = br_bod);
      bod_studenta[0] = 0;
      bod_studenta[1] = 0;
      position = 0;
      lines++;
    }
  }
  // vrijednosti bod_studenta i position-a pamte vrijednost posljednje linije file-a
  // ali kako se ne mogu pribrojiti ukupnom zbroju unutar while-a zbog EOF-a, to radimo nakon while-a
  br_bod = bod_studenta[0]*pow(10, position-1) + bod_studenta[1]*pow(10, position-2); // s obzirom da su samo dvije znamenke, ovaj nacin je efikasniji nego da radimo preko for loopa+
  br_bod > max_br_bod && (max_br_bod = br_bod);

  rewind(f);

  Student *studenti = (Student *)malloc(lines*sizeof(Student)); // svaki student je u svojoj liniji u file-u

  printf("Ime\tPrezime\tBodovi\tRelativni bodovi\n");

  // za broj linija provuci funkciju fscanf kroz file
  for(int i = 0; i < lines; i++){
    fscanf(f, " %s %s %d", studenti[i].name, studenti[i].surname, &studenti[i].points);
    // ispis vrsimo odmah kako nebi ponovno morali zavrtiti for/while petlju
    printf("%s\t%s\t%d\t%.0f %%\n", studenti[i].name, studenti[i].surname, studenti[i].points, ((double)studenti[i].points/max_br_bod)*100);
  }

  fclose(f);
  return 0;
}