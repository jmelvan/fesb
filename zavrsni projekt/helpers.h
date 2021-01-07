#define TIMELINE_START 5
#define TIMELINE_END 19

// function from calculating hash key from user ID
int calculateHashKey(int);

 // function to get position in hash table by user ID
int getPosition(int);

// function to calculate worked hours for one day, and add up them to total working hours of specified user
int addUpOneDay(int, user);

// function to display worked days and hours for specified user in specified date range
int displayDaysForUser(user, char*, char*);

// function to display single working day
int displayDay(int, int);

// display timeline layout 5-19h
int timeline(int, int, int);

// function to move cursor to begining of work time
int markBegining(int);

// function to fill worked time of the day with dashes
int fillWorkingTime(int);

// function to check if worker showed up on work on specified date
int showedUp(int, int, int, int);

// print that worker didn't show up on specified date
int didntShowUp(int, int, int);

// function to export report of all workers and their working hours
int exportReport(char* ,user* , int);

// check date difference
int dateDifference(int, int, int, int, int, int);