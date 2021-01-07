#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "users.h"
#include "helpers.h"

// lookup table for days in month
const int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int calculateHashKey(int user_id) {
	int hash = 0, count = user_id;
	while (count != 0) {
		hash += count % 10;
		count /= 10;
	}
	return hash;
}

int getPosition(int user_id) {
	int hashKey = calculateHashKey(user_id);
	return hashKey % TABLE_SIZE;
}

int addUpOneDay(int timestamp, user single) {
	time_t t = timestamp;
	struct tm *day = gmtime(&t);
	single->total_wh += day->tm_hour + ((float)day->tm_min / 60);
	return 0;
}

int displayDaysForUser(user single, char* s_from, char* s_to) {
	int from[3] = { 0,0,0 };
	int to[3] = { 0,0,0 };
	sscanf(s_from, "%d.%d.%d", &from[0], &from[1], &from[2]);
	sscanf(s_to, "%d.%d.%d", &to[0], &to[1], &to[2]);
	printf("\nWorking days for user: %s %s\n", single->name, single->surname);
	workingHours temp = single->wh->next;
	while (from[0] != to[0] + 1 || from[1] != to[1]) {
		int status = showedUp(from[0], from[1], from[2], (temp != NULL ? temp->timestamp : 0));
		if (status == 0)
			didntShowUp(from[0], from[1], from[2], 0);
		else if (status == 1) {
			displayDay(temp->timestamp, temp->next->timestamp);
			temp = temp->next->next;
		}
		else if (status == -1)
			temp = temp->next->next;
		else if (status == -2)
			didntShowUp(from[0], from[1], from[2], 1);

		if (from[0] < days_in_month[from[1] - 1] && status != -1)
			from[0]++;
		else if (status != -1) {
			from[0] = 1;
			from[1]++;
		}
	}
	return 0;
}

int displayDay(int ts1, int ts2) {
	time_t t1 = ts1;
	time_t t2 = ts2;
	struct tm *in = gmtime(&t1);
	struct tm *out = gmtime(&t2);
	timeline(in->tm_mday, in->tm_mon + 1, in->tm_year + 1900);
	markBegining(ts1);
	fillWorkingTime(ts2 - ts1);
	return 0;
}

int timeline(int day, int month, int year) {
	printf("\n%d.%d.%d\n", day, month, year);
	printf("\n...\t5\t6\t7\t8\t9\t10\t11\t12\t13\t14\t15\t16\t17\t18\t19\t...\n");
	printf("\t|\t|\t|\t|\t|\t|\t|\t|\t|\t|\t|\t|\t|\t|\t|\n");
	return 0;
}

int markBegining(int ts) {
	time_t t = ts;
	struct tm *in = gmtime(&t);
	int hour = in->tm_hour;
	int min = in->tm_min;
	printf("\t");
	for (int i = 0; i < hour - TIMELINE_START; i++)
		printf("\t");
	for (int i = 0; i < (8 * min) / 60; i++)
		printf(" ");
	printf("|");
	return 0;
}

int fillWorkingTime(int ts) {
	time_t t = ts;
	struct tm *worked_time = gmtime(&t);
	int hours = worked_time->tm_hour;
	int mins = worked_time->tm_min;
	for (int i = 0; i < hours; i++)
		printf("--------");
	for (int i = 0; i < (8 * mins) / 60; i++)
		printf("-");
	printf("|\n\n");
	return 0;
}

int showedUp(int day, int month, int year, int ts) {
	if (ts == 0) return 0;
	time_t t = ts;
	struct tm *showed = gmtime(&t);
	if (showed->tm_mday == day && showed->tm_mon + 1 == month && showed->tm_year + 1900 == year) return 1;
	if (showed->tm_mon + 1 < month && showed->tm_mday < day) return -1;
	return 0;
}

int didntShowUp(int day, int month, int year, int weekend) {
	if (!weekend)
		printf("\n%d.%d.%d\n\nDidn't show up.\n\n", day, month, year);
	else
		printf("\n%d.%d.%d\n\nWeekend.\n\n", day, month, year);
	return 0;
}

int exportReport(char* filename, user* users, int margin) {
	FILE *f = fopen(filename, "w");
	if (f == NULL) return -1;

	fprintf(f, "%-10s%-15s%-20s%-20s%-20s\n\n", "Name", "Surname", "Total Hours", "Total Minutes", "Difference");
	for (int i = 0; i < TABLE_SIZE; i++)
		if (users[i]->next != NULL) {
			user row = users[i]->next;
			while (row != NULL) {
				fprintf(f, "%-10s%-15s%3d %-16s%3d %-16s%+7.2lf hrs\n", row->name, row->surname, (int)floor(row->total_wh), "hrs", (int)((row->total_wh - (int)floor(row->total_wh)) * 60), "mins", (row->total_wh - margin));
				row = row->next;
			}
		}

	fclose(f);
	return 0;
}

int dateDifference(int fd, int fm, int fy, int sd, int sm, int sy) {
	if (fd < 0 || fm < 0 || fy < 0 || sd < 0 || sm < 0 || sy < 0) return -1;
	if (fd > days_in_month[fm-1] || sd > days_in_month[sm-1]) return -1;
	if (fm > 12 || sm > 12) return -1;
	if (fy != sy) return -1;
	if (fm > sm) return -1;
	if (fm == sm)
	{
		if (fd > sd) return -1;
		else return 0;
	}
	else {
		if (fm + 1 != sm) return -1;
		int days = days_in_month[fm-1] - fd;
		days += sd;
		if (days > 30) return -1;
	}
	return 0;
}