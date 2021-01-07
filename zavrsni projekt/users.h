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

user* initTable(); // function to initialize hash table
user initUser(char*, char*, int); // create new struct user and set data
user findUserById(int, user*); // find and return user with given ID
int loadUsers(user*); // function to load users from database into hash table
int readUser(char*, user*); // function to parse line from users database
int storeUser(user, user*); // store user to hash table
int sortedInsertion(user, user); // sorted insertion to linked list
int displayUsers(user*);