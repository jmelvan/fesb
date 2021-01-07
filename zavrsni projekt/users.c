#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "users.h"
#include "helpers.h"

user* initTable(){
  user* users = (user*)malloc(TABLE_SIZE*sizeof(user));
  for(int i = 0; i < TABLE_SIZE; i++){
    users[i] = (user)malloc(sizeof(struct _user));
    users[i]->next = NULL;
  }
  return users;
}

int loadUsers(user* users){
  FILE *f = fopen("users.txt", "r");
  if(f == NULL) return -1;

  char buffer[BUFFER];
  while(fgets(buffer, BUFFER, f) != NULL)
    readUser(buffer, users);

  fclose(f);
  return 0;
}

int readUser(char* buffer, user* users){
  int id;
  char name[NAME], surname[NAME];
  sscanf(buffer, " %s %s %d", name, surname, &id);
  user new = initUser(name, surname, id);
  storeUser(new, users);
  return 0;
}

user initUser(char name[NAME], char surname[NAME], int id){
  user new = (user)malloc(sizeof(struct _user));
  if(new == NULL) exit(1);
  new->next = NULL;
  new->id = id;
  strcpy(new->name, name);
  strcpy(new->surname, surname);
  new->total_wh = 0;
  new->wh = (workingHours)malloc(sizeof(struct _workingHours));
  if(new->wh == NULL) exit(1);
  new->wh->next = NULL;
  return new;
}

int storeUser(user new, user* users){
  int position = getPosition(new->id);
  sortedInsertion(users[position], new);
  return 0;
}

int sortedInsertion(user position, user new){
  while(position->next != NULL && position->next->next != NULL){
    if(strcmp(position->next->surname, new->surname) > 0 || (strcmp(position->next->surname, new->surname) == 0 && strcmp(position->next->name, new->name) > 0))
      break;
    position = position->next;
  }
  new->next = position->next;
  position->next = new;
  return 0;
}

user findUserById(int user_id, user* users){
  int position = getPosition(user_id);
  user temp = users[position];
  while(temp->next != NULL && temp->next->id != user_id)
    temp = temp->next;
  return temp->next;
}

int displayUsers(user* users){
  printf("\nBr.\t\tHash\t\tID\t\tIme\t\tPrezime\n\n");
  for(int i = 0; i < TABLE_SIZE; i++){
    user temp = users[i];
    while(temp->next != NULL){
        printf(
            "%d.\t\t%d\t\t%d\t\t%s\t\t%s\n",
            i,
            calculateHashKey(temp->next->id),
            temp->next->id,
            temp->next->name,
            temp->next->surname
        );
        temp = temp->next;
    }
  }
  return 0;
}