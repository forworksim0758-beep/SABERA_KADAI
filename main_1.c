#include "func_1.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "");

    if (loadDictionary("dict.txt") != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    gameStatus gs;
    fwprintf(stdout, L"> Select difficulty (EASY/NORMAL/DIFFICULT):\n");
    if (selectDifficulty(&gs) != EXIT_SUCCESS) {
        free_dictionary();
        return EXIT_FAILURE;
    }

    gameLoop(&gs);
    free_dictionary();
    return EXIT_SUCCESS;
}