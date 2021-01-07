#include <stdio.h>
#include <stdlib.h>
#include "users.h"
#include "device.h"
#include "helpers.h"
#include "program.h"

int start(){
  user* users = initTable();
  loadUsers(users);
  loadWorkingHours(users);
  displayUsers(users);
  displayDaysForUser(users[7]->next, "1.9.2020", "5.9.2020");
  exportDifference("izvjestaj.txt", users, 10);
  return 0;
}