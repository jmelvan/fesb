#define TIMELINE_START 5
#define TIMELINE_END 19

int calculateHashKey(int);
int getPosition(int);
int addUpOneDay(int, user);
int displayDaysForUser(user, char*, char*);
int displayDay(int, int);
int timeline(int, int, int);
int markBegining(int);
int fillWorkingTime(int);
int showedUp(int, int, int, int);
int didntShowUp(int, int, int, int);
int exportDifference(char* ,user* , int);