#define _CRT_SECURE_NO_WARNINGS
#define nextword 2
#define sbrosind -1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

void deletesym(char *s, int a, int b) {

	while (a <= b) {
		s[a] = s[a + 1];
		a++;
	}

	return;
}

int main(void) {
	setlocale(LC_ALL, "Russian");

	FILE* input;
	int reqlength = 0;
	int filelength = 0;
	int curpos = 0;
	int startpos = 0;
	int lastwordind = sbrosind;
	int curlength = 0;
	int outpointer = 0;
	char filename[256] = { 0 };
	char* str = NULL;

	printf("Введите имя файла...\n");
	scanf("%s", &filename);
	if (strchr(filename, '.') == NULL) {
		input = fopen(strcat(filename, ".txt"), "r+");
	}
	else {
		input = fopen(filename, "r+");
	}
	if (input == NULL) {
		printf("Не удалось открыть файл, проверьте имя файла...\n");
		return 1;
	}

	fscanf(input, "%d", &reqlength);

	fgetc(input);
	while (fgetc(input) != EOF) {
		filelength++;
	}
	fseek(input, -(filelength), SEEK_END);

	str = calloc((filelength + 1), sizeof(char));
	if (str == NULL) {
		printf("Не удалось выделить память...");
		return 1;
	}

	for (curpos = 0; curpos < filelength; curpos++) {
		str[curpos] = fgetc(input);
	}
	curpos = 0;
	
	while (curpos < filelength) {
		if (((isspace(str[curpos])) && (isspace(str[curpos + 1])) || isspace(str[0]))) {
			deletesym(str, curpos, filelength);
			filelength--;
			continue;
		}
		curpos++;
	}
	curpos = 0;

	while (curpos < filelength) {
		if (isspace(str[curpos])) {
			lastwordind = curpos - 1;
		}
		if (curlength == reqlength) {
			if (lastwordind == sbrosind) {
				outpointer = curpos - curlength;
				for (outpointer; outpointer < curpos; outpointer++) {
					printf("%c", str[outpointer]);
				}
				printf("\n");
				startpos = curpos;

			}
			else {
				if (isspace(str[curpos])) {
					lastwordind = curpos - 1;
				}
				outpointer = 0;
				for (outpointer; outpointer < (reqlength - (lastwordind - startpos + 1)); outpointer++) {
					printf(" ");
				}
				outpointer = startpos;
				for (outpointer; outpointer <= lastwordind; outpointer++) {
					printf("%c", str[outpointer]);
				}
				printf("\n");
				startpos = lastwordind + nextword;
				curpos = startpos;
				lastwordind = sbrosind;
			}
			curlength = 0;
		}
		curpos++;
		curlength++;
	}

	fclose(input);

	free(str);

	return 0;
}