#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node * Position;
struct Node{
  int br;
  Position next;
};

int push(Position stog);
int pop(Position stog);
int red_push(Position last);
int red_pop(Position red);

int main(){
  Position stog;
  stog->next = NULL;
  
  srand(time(0)); 
  
  push(stog);
  push(stog);
  push(stog);
  printf("\n%d %d %d %d\n\n", pop(stog), pop(stog), pop(stog), pop(stog)); // ako nema nista vise u listi, a koristimo pop(), vratit ce -1

  Position red = (Position)malloc(sizeof(struct Node)), last = (Position)malloc(sizeof(struct Node));
  red->next = NULL;
  last->next = red;

  red_push(last);
  red_push(last);
  red_push(last);
  printf("\n%d %d %d %d\n\n", pop(red), pop(red), pop(red), pop(red));
  return 0;
}

int push(Position stog){
  Position new = (Position)malloc(sizeof(struct Node));
  new->br = (rand() % (100 - 10 + 1)) + 10;
  printf("%d ", new->br);
  new->next = stog->next;
  stog->next = new;
  return 0;
}

int pop(Position stog){
  if(stog->next == NULL) return -1;
  Position temp = stog->next;
  stog->next = temp->next;
  int temp_br = temp->br;
  free(temp);
  return temp_br;
}

int red_push(Position last){
  Position new = (Position)malloc(sizeof(struct Node));
  new->br = (rand() % (100 - 10 + 1)) + 10;
  printf("%d ", new->br);
  new->next = NULL;
  last->next->next = new;
  last->next = last->next->next;
  return 0;
}