#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER 256

typedef struct Node * List;
struct Node{
  int br, type;
  char operand;
  List next;
};

typedef struct Operands * Stack;
struct Operands{
  char operand;
  Stack next;
};

typedef struct Numbers * Nstack;
struct Numbers{
  int br;
  Nstack next;
};

List readFromFile(char *);
int push_operands(Stack, char, List);
int check_operands(Stack, char, List);
int empty_operands(Stack, List);
int empty_operands_until_bracket(Stack, List);
int pop_operands(Stack);
int push(List, int, char, int);
int calculate_result(List);
int calculate_until_number(Nstack, List);
int push_until_operand(Nstack, List);
int push_numbers(Nstack, int);
int pop_result(Nstack);
int pop_postfix(List);
int parseLine(char *, List);
int display(List);
int deleteAll(List);

int main(){
  List postfix = readFromFile("infix.txt");
  display(postfix);
  int result = calculate_result(postfix);
  printf("\n\nRezultat: %d\n", result);

  deleteAll(postfix);
  return 0;
}

// Ucitavanje linije iz file-a
List readFromFile(char *filename){
  FILE *f = fopen(filename, "r");
  if(f == NULL) return NULL;

  List postfix = (List)malloc(sizeof(struct Node)), last = (List)malloc(sizeof(struct Node));
  postfix->next = NULL;
  last->next = postfix;

  char buffer[BUFFER];
  fgets(buffer, BUFFER, f);
  parseLine(buffer, last);

  fclose(f);
  return postfix;
}

// Parsiranje infix-a, pushanje u red u postfix izrazu
int parseLine(char *buffer, List postfix){
  Stack stog = (Stack)malloc(sizeof(struct Operands));
  
  int br = 0, offset = 0, offset2 = 0;
  char operand;
  if(buffer[0] == '(') (push_operands(stog, '(', postfix), buffer += 1); // ako je zagrada na prvom charu
  while(sscanf(buffer, "%d%n%*[+-*/()]%n%*[^\n]", &br, &offset, &offset2) == 1){
    buffer += offset;
    push(postfix, br, '\0', 0);
    for(int i = 0; i < offset2-offset; i++)
      (buffer[i] >= 40 && buffer[i] <= 47) && push_operands(stog, buffer[i], postfix); // ASCII provjera znakova da se radi o operandima koji su potrebni
    buffer += offset2-offset;
  }
  empty_operands(stog, postfix);
  
  return 0;
}

// Racunanje rezultata - glavna funkcija racunanja
int calculate_result(List head){
  Nstack stog = (Nstack)malloc(sizeof(struct Numbers));
  stog->next = NULL;

  while(head->next != NULL){
    push_until_operand(stog, head);
    calculate_until_number(stog, head);
  }
  return stog->next->br;
}

// Racunanje operatorima do sljedeceg broja - pomocna funkcija racunanja
int calculate_until_number(Nstack stog, List head){
  if(head->next == NULL || head->next->type == 0) return 0;
  char operand = head->next->operand;
  stog->next->next->br = (operand == '+') ? (stog->next->next->br + stog->next->br) : (operand == '-') ? (stog->next->next->br - stog->next->br) : (operand == '*') ? (stog->next->next->br * stog->next->br) : (operand == '/') ? (stog->next->next->br * stog->next->br) : 0;
  pop_postfix(head);
  pop_result(stog);
  return calculate_until_number(stog, head);
}

// Pripremanje brojeva za pushanje na stog do sljedeceg operatora - pomocna funkcija racunanja
int push_until_operand(Nstack stog, List head){
  if(head->next == NULL || head->next->type == 1) return 0;
  push_numbers(stog, head->next->br);
  pop_postfix(head);
  return push_until_operand(stog, head);
}

// Pushanje brojeva na stog - pomocna funkcija racunanja
int push_numbers(Nstack stog, int br){
  Nstack new = (Nstack)malloc(sizeof(struct Numbers));
  new->br = br;
  new->next = stog->next;
  stog->next = new;
  return 0;
}

// Izbacivanje i brisanje zadnjeg elementa sa reda postfixa
int pop_postfix(List head){
  if(head->next == NULL) return -1;
  List temp = head->next;
  head->next = temp->next;
  free(temp);
  return 0;
}

// Izbacivanje i brisanje zadnjeg elementa sa stoga rezultata
int pop_result(Nstack stog){
  if(stog->next == NULL) return -1;
  Nstack temp = stog->next;
  stog->next = temp->next;
  free(temp);
  return 0;
}

// Pushanje elemnta postfix izraza u red postfix
int push(List postfix, int br, char operand, int type){
  List new = (List)malloc(sizeof(struct Node));
  new->br = br;
  new->operand = operand;
  new->type = type;
  new->next = NULL;
  postfix->next->next = new;
  postfix->next = postfix->next->next;
  return 0;
}

// Pushanje operanda na stog za operande
int push_operands(Stack stog, char operand, List postfix){
  int status = check_operands(stog, operand, postfix);
  if(status == -1) return 0;
  Stack new = (Stack)malloc(sizeof(struct Operands));
  new->operand = operand;
  new->next = stog->next;
  stog->next = new;
  return 0;
}

// Rekurzivna provjera operanada na stogu u svrhu redoslijeda racunskih operacija
int check_operands(Stack stog, char operand, List postfix){
  if(stog->next == NULL) return 0;
  if(operand == ')'){
    empty_operands_until_bracket(stog, postfix);
    return -1;
  }
  if((operand == '+' || operand == '-') && (stog->next->operand == '*' || stog->next->operand == '/' || stog->next->operand == '+' || stog->next->operand == '-')) {
    push(postfix, 0, stog->next->operand, 1);
    pop_operands(stog);
    return check_operands(stog, operand, postfix);
  }
  return 0;
}

// Izbacivanje operanada sa stoga do otvorene zagrade, (kada dode zatvorena zagrada svi operandi prije se unesu u red postfixa)
int empty_operands_until_bracket(Stack stog, List postfix){
  if(stog->next == NULL) return 0;
  if(stog->next->operand == '('){
    pop_operands(stog);
    return 0;
  }
  push(postfix, 0, stog->next->operand, 1);
  pop_operands(stog);
  return empty_operands_until_bracket(stog, postfix);
}

// Izbacivanje i unosenje preostalih operanada u red postfixa na kraju izraza 
int empty_operands(Stack stog, List postfix){
  if(stog->next == NULL) return 0;
  push(postfix, 0, stog->next->operand, 1);
  pop_operands(stog);
  return empty_operands(stog, postfix);
}

// Izbacivanje i brisanje jednog operanda
int pop_operands(Stack stog){
  if(stog->next == NULL) return -1;
  Stack temp = stog->next;
  stog->next = temp->next;
  free(temp);
  return 0;
}

// Ispis reda postfixa
int display(List head){
  printf("\nPostfix: ");
  while(head->next != NULL){
    head = head->next;
    if(head->type == 0) 
      printf("%d ", head->br);
    else
      printf("%c ", head->operand);  
  }
  printf("\n");
  return 0;
}

int deleteAll(List head){
  while(head->next != NULL){
    List temp = head;
    head = head->next;
    free(temp);
  }
  return 0;
}