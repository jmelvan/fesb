#include <stdio.h>
#include <stdlib.h>
#include "users.h"
#include "device.h"
#include "helpers.h"
#include "program.h"

int startUserInterface(user*);
int handleDates(char *, char *, char *);
int dateDifference(int, int, int, int, int, int);

int start(){
	user* users = initTable();
	
	loadUsers(users);
	loadWorkingHours(users);
	printf("Program uspijesno ucitan...\n\nUcitani korisnici:");
	displayUsers(users);
	startUserInterface(users);
	return 0;
}

int startUserInterface(user* users)
{

	if (users == NULL) return -1;

	int opcija = 1,
		id = -1,
		ind = 1;
	user temp = NULL;
	char buffer[BUFFER],
		 poc[64],
		 kraj[64]; // ZA UNOS

	/**/
	while (opcija > 0)
	{
		printf("-------------------------------------------------\n");
		printf("\nOpcije: \n1 - Ispisi radne sate za korisnika\n"
			"2 - Ispisi korisnike\n3 - Export izvjestaja\n"
			"4 - Ocisti ekran\n5 - Zavrsi s programom\n"
			"\nUnesite opciju: ");

		scanf(" %d", &opcija);

		switch (opcija)
		{
		case 1:
		{
			printf("\nOdabrali ste opciju za ispis radnih sati.\n"
				"Unesite ID korisnika: ");
			scanf(" %d", &id);
			temp = findUserById(id, users);
			if (temp == NULL) {
				printf("\nUnijeli ste krivi ID korisnika!");
				break;
			}
			else {
			
				printf("Uredu, odabrali ste korisnika %s %s ( %d ).\n\n", temp->name, temp->surname, id);
				printf("Unesite pocetni i kranji datum za ispis radnih sati: \n");
				while (ind != 0 && ind != -1) {
					printf("Format unosa (dd.mm.gggg dd.mm.gggg): ");
					fseek(stdin, 0, SEEK_END); // buffer erase
					if (fgets(buffer, BUFFER, stdin) != NULL)
						ind = handleDates(buffer, poc, kraj);

					if (ind == -1)
						printf("Greska u memoriji.\n");
					if (ind == 2)
						printf("Niste unjeli drugi datum.\n");
					if (ind == 3)
						printf("Datumi se moraju nalaziti u istoj godini sa max razmakom od 30 dana.\n");
				}
				printf("\nWorking days: [%s - %s]\n", poc, kraj);
				displayDaysForUser(temp, poc, kraj);

			}
			break;
		}
		case 2:
		{
			displayUsers(users);
			printf("\n");
			break;
		}
		case 3: {
			printf("\nUnesite koliko je radnih sati bilo potrebno: ");
			scanf(" %d", &id);
			if (id > 0)
			{
				exportReport("izvjestaj.txt", users, id);
			}
			else {
				printf("Unijeli ste negativan broj!\n");
			}
			break;
		}
		case 4: {
			system("cls");
			break;
		}
		default: {
			opcija = 0;
		}

		}

	}
	return 0;
}

int handleDates(char *buffer, char *p, char *k)
{
	if (buffer == NULL) return -1;
	if (strlen(buffer) == 0) return -1;
	int br[6] = { 0 },
		count = 0,
		i = 0;
	while (sscanf(buffer, "%d.%d.%d%n", &br[i], &br[i+1], &br[i+2], &count))
	{
		buffer += count;
		i += 3;
		if (i >= 6) break;
	}

	if (i < 6) return 2; // Nije unesen drugi datum
	if (dateDifference(br[0], br[1], br[2], br[3], br[4], br[5]) == -1) return 3; // krivi format
	sprintf(p, "%d.%d.%d\0", br[0], br[1], br[2]);
	sprintf(k, "%d.%d.%d\0", br[3], br[4], br[5]);
	return 0;
}

