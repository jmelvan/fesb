#define NAME 60
#define BUFFER 256
#define TABLE_SIZE 10

typedef struct _workingHours * workingHours;
struct _workingHours{
  int timestamp;
  workingHours next;
};

typedef struct _user* user;
struct _user{
  int id;
  float total_wh; // total number of working hours
  char name[NAME], surname[NAME];
  user next;
  workingHours wh;
};

// function to initialize hash table
user* initTable();

// create new struct user and set data
user initUser(char*, char*, int);

// find and return user with given ID
user findUserById(int, user*);

// function to load users from database into hash table
int loadUsers(user*);

// function to parse line from users database
int readUser(char*, user*);

// store user to hash table
int storeUser(user, user*);

// sorted insertion to linked list
int sortedInsertion(user, user);

// function to display all workers
int displayUsers(user*);