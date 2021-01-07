int loadWorkingHours(user*);
int loadDeviceLog(user*);
int readLog(char*, user*);
int storeLog(int, workingHours, user*);
int calculateTotalWH(user*);
int calculateForRow(user);
int calculateSingleWH(user);
workingHours initWH(int);