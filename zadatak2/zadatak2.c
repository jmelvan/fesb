#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Osoba* Position;
int prepend(char *name, char *surname, int years, Position head);
int show(Position head);
int append(char *name, char *surname, int years, Position head);
Position search(char *surname, Position head);
int del(char *name, char *surname, int years, Position head);

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
	searched != NULL && printf("\nPretrazena osoba: %s %s %d", searched->name, searched->surname, searched->years);
	printf("\n\n");
	del("Ante", "Kuic", 19, head);
	show(head);
	system("pause");
	return 0;
}

int del(char *name, char *surname, int years, Position head) {
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

Position search(char *surname, Position head) {
	if (head == NULL) return NULL;
	while (head->next != NULL && strcmp(head->surname, surname) != 0) {
		head = head->next;
	}
	return head;
}

int prepend(char *name, char *surname, int years, Position head) {
	if (head == NULL) return -1;
	Position new = (Position)malloc(sizeof(struct Osoba));
	strcpy(new->name, name);
	strcpy(new->surname, surname);
	new->years = years;
	new->next = head->next;
	head->next = new;
	return 0;
}

int append(char *name, char *surname, int years, Position head) {
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

int show(Position head) {
	if(head != NULL){
		while (head->next != NULL) {
			head = head->next;
			printf("%s %s %d, ", head->name, head->surname, head->years);
		}
		return 0;
	}
	return -1;
}
