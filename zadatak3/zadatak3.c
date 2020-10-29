#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Osoba* Position;
int prepend(char name[], char surname[], int years, Position head);
int show(Position head);
int append(char name[], char surname[], int years, Position head);
Position search(char surname[], Position head);
int del(char name[], char surname[], int years, Position head);
int addAfter(char after_surname[], char name[], char surname[], int years, Position head);
int addBefore(char after_surname[], char name[], char surname[], int years, Position head);
int sort(Position head);
int readFromFile(char file[], Position head);
int writeToFile(char file[], Position head);

struct Osoba {
	char name[30];
	char surname[30];
	int years;
	Position next;
};

int main() {
	Position head = NULL;
	head = (Position)malloc(sizeof(struct Osoba));
	head->next = NULL;

	append("Ante", "Kuic", 19, head);
	prepend("Jakov", "Melvan", 20, head);
	append("John", "Doe", 23, head);
	show(head);

	Position searched = search("Melvan", head);
	searched != NULL && printf("\nPretrazena osoba: %s %s %d\n\n", searched->name, searched->surname, searched->years);
	
	del("Ante", "Kuic", 19, head);
	show(head);

	addAfter("Melvan", "Marin", "Ivandic", 31, head);
	show(head);
	
	addBefore("Ivandic", "Drasko", "Lukas", 35, head);
	show(head);
	
	sort(head);
	show(head);
	
	readFromFile("studenti.txt", head);
	show(head);

	writeToFile("studenti.txt", head);

	system("pause");
	return 0;
}

/* 
	INPUT: filename, Position head
	RETURN: success or error
	DESCRIPTION: zapisuje vezanu listu u file
*/
int writeToFile(char file[], Position head){
	FILE *f = fopen(file, "w");
	if(f == NULL){
    printf("Greska pri ucitavanju datoteke.\n");
    return -1;
  }
	if(head != NULL){
		while (head->next != NULL) {
			head = head->next;
			fprintf(f, "%s %s %d\n", head->name, head->surname, head->years);
		}
		return 0;
	}
	fclose(f);
	return 0;
}

/* 
	INPUT: filename, Position head
	RETURN: success or error
	DESCRIPTION: ucitava studente iz file-a na kraj vezane liste
*/
int readFromFile(char file[], Position head){
	FILE *f = fopen(file, "r");
	if(f == NULL){
    printf("Greska pri ucitavanju datoteke.\n");
    return -1;
  }
	char name[30], surname[30];
	int years;
	while(!feof(f)){
		fscanf(f, " %s %s %d", name, surname, &years);
		append(name, surname, years, head);
	}
	fclose(f);
	return 0;
}

/* 
	INPUT: Position head
	RETURN: success or error
	DESCRIPTION: sortira vezanu listu po prezimenima
*/
int sort(Position head){
	if (head == NULL) return -1;
	int changed = 1;
	Position temp = (Position)malloc(sizeof(struct Osoba));
	Position start;
	while(changed){
		changed = 0;
		start = head;
		while (start->next->next != NULL){
			start = start->next;
			if(strcmp(start->surname, start->next->surname) > 0){
				changed = 1;
				strcpy(temp->name, start->name);
				strcpy(temp->surname, start->surname);
				temp->years = start->years;
				strcpy(start->name, start->next->name);
				strcpy(start->surname, start->next->surname);
				start->years = start->next->years;
				strcpy(start->next->name, temp->name);
				strcpy(start->next->surname, temp->surname);
				start->next->years = temp->years;
			}
		}
	}
	free(temp);
	return 0;
}

/* 
	INPUT: [char after_surname](Prezime nakon kojeg zelimo dodati), char name, char surname, int years, Position head
	RETURN: success or error
	DESCRIPTION: dodaje element iza elementa cije je prezime `after_surname`
*/
int addAfter(char after_surname[], char name[], char surname[], int years, Position head){
  if (head == NULL) return -1;
  Position before = search(after_surname, head);
  Position new = (Position)malloc(sizeof(struct Osoba));
  strcpy(new->name, name);
	strcpy(new->surname, surname);
	new->years = years;
  new->next = before->next;
  before->next = new;
  return 0;
}

/* 
	INPUT: [char after_surname](Prezime prije kojeg zelimo dodati), char name, char surname, int years, Position head
	RETURN: success or error
	DESCRIPTION: dodaje element prije elementa cije je prezime `after_surname`
*/
int addBefore(char after_surname[], char name[], char surname[], int years, Position head){
  if (head->next == NULL) return -1;
  while (head->next->next != NULL && strcmp(head->next->surname, after_surname) != 0) {
		head = head->next;
	}
	Position new = (Position)malloc(sizeof(struct Osoba));
  strcpy(new->name, name);
	strcpy(new->surname, surname);
	new->years = years;
  new->next = head->next;
  head->next = new;
  return 0;
}

/* 
	INPUT: char name, char surname, int years, Position head
	RETURN: success or error
	DESCRIPTION: brise element koji ima vrijednosti `name`, `surname` i `year` jednake kao one sto su proslijedene u funkciju
*/
int del(char name[], char surname[], int years, Position head) {
	if (head == NULL) return -1;
	Position last = (Position)malloc(sizeof(struct Osoba));
	while (head->next != NULL && strcmp(head->surname, surname) != 0 && strcmp(head->name, name) != 0 && head->years != years) {
		last = head;
		head = head->next;
	}
	last->next = head->next;
	free(head);
	return -1;
}

/* 
	INPUT: char surname, Position head
	RETURN: Position element
	DESCRIPTION: brise element koji ima vrijednosti `name`, `surname` i `year` jednake kao one sto su proslijedene u funkciju
*/
Position search(char surname[], Position head) {
	if (head == NULL) return NULL;
	while (head->next != NULL && strcmp(head->surname, surname) != 0) {
		head = head->next;
	}
	return head;
}

/* 
	INPUT: char name, char surname, int years, Position head
	RETURN: success or error
	DESCRIPTION: dodaje element na pocetak vezane liste
*/
int prepend(char name[], char surname[], int years, Position head) {
	if (head == NULL) return -1;
	Position new = (Position)malloc(sizeof(struct Osoba));
	strcpy(new->name, name);
	strcpy(new->surname, surname);
	new->years = years;
	new->next = head->next;
	head->next = new;
	return 0;
}

/* 
	INPUT: char name, char surname, int years, Position head
	RETURN: success or error
	DESCRIPTION: dodaje element na kraj vezane liste
*/
int append(char name[], char surname[], int years, Position head) {
	if (head == NULL) return -1;
	while (head->next != NULL) {
		head = head->next;
	}
	Position new = (Position)malloc(sizeof(struct Osoba));
	strcpy(new->name, name);
	strcpy(new->surname, surname);
	new->years = years;
	new->next = NULL;
	head->next = new;
	return 0;
}

/* 
	INPUT: Position head
	RETURN: success or error
	DESCRIPTION: ispisuje elemente vezane liste
*/
int show(Position head) {
	if(head != NULL){
		while (head->next != NULL) {
			head = head->next;
			printf("%s %s %d, ", head->name, head->surname, head->years);
		}
		printf("\n");
		return 0;
	}
	return -1;
}
