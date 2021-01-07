#include <stdio.h>
#include <stdlib.h>
#include "users.h"
#include "device.h"
#include "helpers.h"

int loadWorkingHours(user* users){
  loadDeviceLog(users);
  calculateTotalWH(users);
  return 0;
}

int loadDeviceLog(user* users){
  FILE *f = fopen("devicelog.txt", "r");
  if(f == NULL) return -1;

  char buffer[BUFFER];
  while(fgets(buffer, BUFFER, f) != NULL)
    readLog(buffer, users);

  fclose(f);
  return 0;
}

int readLog(char* buffer, user* users){
  int user_id = 0;
  int timestamp = 0;
  sscanf(buffer, " %d %d", &user_id, &timestamp);
  workingHours new = initWH(timestamp);
  storeLog(user_id, new, users);
  return 0;
}

int storeLog(int user_id, workingHours new, user* users){
  user u = findUserById(user_id, users);
  if(u == NULL) return -1;
  workingHours temp = u->wh;
  while(temp->next != NULL)
    temp = temp->next;
  temp->next = new;
  return 0;
}

workingHours initWH(int timestamp){
  workingHours new = (workingHours)malloc(sizeof(struct _workingHours));
  if(new == NULL) exit(1);
  new->next = NULL;
  new->timestamp = timestamp;
  return new;
}

int calculateTotalWH(user* users){
  for(int i = 0; i < TABLE_SIZE; i++)
    users[i]->next != NULL && calculateForRow(users[i]->next);
  return 0;
}

int calculateForRow(user row){
  while(row != NULL){
    calculateSingleWH(row);
    row = row->next;
  }
  return 0;
}

int calculateSingleWH(user single){
  workingHours temp = single->wh->next;
  while(temp != NULL && temp->next != NULL){
    addUpOneDay((temp->next->timestamp - temp->timestamp), single);
    temp = temp->next->next;
  }
  return 0;
}