// function that orders tasks to load and save all data from device log
int loadWorkingHours(user*);

// function to read device log
int loadDeviceLog(user*);

// function to parse one line from devices log
int readLog(char*, user*);

// function to store parsed line from device log
int storeLog(int, workingHours, user*);

// function to calculate total number of working hours for all users
int calculateTotalWH(user*);

// function to calculate total number of working hours for row of users in hash table
int calculateForRow(user);

// function to calculate total number of working hours for single user
int calculateSingleWH(user);

// create new struct workingHours and save timestamp in it
workingHours initWH(int);