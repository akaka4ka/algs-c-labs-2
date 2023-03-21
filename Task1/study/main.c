#define _CRT_SECURE_NO_WARNINGS
#define LENGHT 10
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    setlocale(LC_ALL, "Russian");

    FILE* input;
    char name[256] = { 0 };
    char* str = NULL;
    int length = 0;
    int endofmaxlen = 0;
    int count = 0;
    int maxlen = 0;
    int i = 0;

    printf("Введите имя файла в формате имя.расширение, если расширение .txt, укажите только имя...\n");
    scanf("%s", name);
    if (strchr(name, '.') == NULL) {
        input = fopen(strcat(name, ".txt"), "r+");
    }
    else {
        input = fopen(name, "r+");
    }
    if (input == NULL) {
        printf("Не удалост открыть файл... Проверть имя файла, расширение и попробуйте снова\n");
        return 1;
    }

    while (fgetc(input) != EOF) {
        length++;
    }
    fseek(input, -(length), SEEK_END);
    
    str = calloc((length + 1), sizeof(char));
    if (str == NULL) {
        printf("Не удалось выделить память...");
        return 1;
    }

    while (i != length) {
        str[i] = fgetc(input);
        i++;
    }

    i = 0;
    while (i < length) {
        if (isdigit(str[i]) != 0) {
            while (isdigit(str[i]) != 0) {
                count++;
                i++;
            }
            if (maxlen < count) {
                maxlen = count;
                endofmaxlen = i;
            }
        }
        count = 0;
        i++;
    }
    fclose(input);

    if ((maxlen % 10) == 1) {
        printf("Самая длиная строка в %d символ:\n", maxlen);
    }
    if ((maxlen % 10) >= 2 && (maxlen % 10) <= 4) {
        printf("Самая длиная строка в %d символа:\n", maxlen);
    }
    if (((maxlen % 10) >= 5 && (maxlen % 10) <= 9) || ((maxlen % 10) == 0)) {
        printf("Самая длиная строка в %d символов:\n", maxlen);
    }
    for (i = (endofmaxlen - maxlen); i < endofmaxlen; i++) {
        printf("%c", str[i]);
    }

    free(str);

    return 0;
}