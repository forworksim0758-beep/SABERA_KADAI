#include "func_1.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

wardsChain *wht[WHTSIZE] = {NULL};
wardsChain *whist = NULL;
wardsChain *dict[DICTSIZE] = {NULL};

int selectDifficulty(gameStatus *gs)
{
    wchar_t buf[32];

    while (fgetws(buf, sizeof(buf) / sizeof(buf[0]), stdin) != NULL) {
        size_t len = wcslen(buf);
        if (len > 0 && buf[len - 1] == L'\n') buf[--len] = L'\0';

        if (!wcscmp(buf, L"EASY")) {
            gs->mode = GM_EASY;
            gs->hp = 5;
            return EXIT_SUCCESS;
        } else if (!wcscmp(buf, L"NORMAL")) {
            gs->mode = GM_NORMAL;
            gs->hp = 1;
            return EXIT_SUCCESS;
        } else if (!wcscmp(buf, L"DIFFICULT")) {
            gs->mode = GM_DIFFICULT;
            gs->hp = 1;
            return EXIT_SUCCESS;
        }

        fwprintf(stdout, L"> Invalid difficulty -> Retry.\n");
    }

    return EXIT_FAILURE;
}

int gameLoop(gameStatus *gs)
{
    int initHp = gs->hp;
    int errFlag = 0;
    wchar_t *prev = NULL;
    wchar_t *word = NULL;

    fwprintf(stdout, L"> REST HP = %d\n", gs->hp);
    while (getWord(&word) == GWR_OK) {
        if (commandControl(gs, initHp, &prev, word)) continue;

        if (applyRules(gs, prev, word) != ARR_ALW) {
            free(word);
        } else {
            CWR cwrRes = checkWard_Existed(word);
            if (cwrRes == CWR_NFND) {
                free(prev);
                prev = word;
            } else if (cwrRes == CWR_FND) {
                wprintf(L"%ls is already appeared.\n", word);
                free(word);
                gs->hp--;
            } else {
                fprintf(stderr, "> Unknown-Error Detected in function (checkWard_Existed).\n");
                free(word);
                errFlag = 1;
                break;
            }
        }

        JCR jcrRes = judgeContinue(gs);
        if (jcrRes == JCR_CONTINUE) {
            fwprintf(stdout, L"> REST HP = %d\n", gs->hp);
            continue;
        } else if (jcrRes == JCR_END) {
            fwprintf(stdout, L"> GameOver -> Quit.\n");
            break;
        }else {
            fprintf(stderr, "> Unknown-Error Detected in function (judgeContinue).\n");
            break;
        }
    }

    free(prev);
    free_table();
    if (errFlag) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int applyRules(gameStatus *gs, wchar_t *prev, wchar_t *w)
{
    if (findNo_Hiragana(w) != FNHR_ALW) {
        fwprintf(stdout, L"> This input includes No-Hiragana -> Retry.\n");
        return ARR_DNY;
    }

    size_t gwlen = wcslen(w);
    if (gwlen <= 1) {
        fwprintf(stdout, L"> This input-length is 1 or less -> Retry.\n");
        return ARR_DNY;
    }

    if (checkWard_InDictionary(w) != DWR_FND) {
        gs->hp--;
        fwprintf(stdout, L"> This word is not found in dictionary -> Retry.\n");
        fwprintf(stdout, L"> REST HP = %d\n", gs->hp);
        return ARR_DNY;
    }

    if (w[gwlen - 1] == L'ん') {
        gs->hp--;
        fwprintf(stdout, L"> This input`s tail is 「ん」 -> Retry.\n");
        fwprintf(stdout, L"> REST HP = %d\n", gs->hp);
        return ARR_DNY;
    }

    if (prev == NULL) return ARR_ALW;

    size_t pwlen = wcslen(prev);
    if (prev[pwlen - 1] != w[0]) {
        gs->hp--;
        fwprintf(stdout, L"> REST HP = %d\n", gs->hp);
        return ARR_DNY;
    }

    if (gs->mode == GM_DIFFICULT && gwlen < pwlen) {
        gs->hp--;
        fwprintf(stdout, L"> REST HP = %d\n", gs->hp);
        return ARR_DNY;
    }

    return ARR_ALW;
}

int judgeContinue(gameStatus *gs)
{
    if (gs->hp > 0) return JCR_CONTINUE;
    else return JCR_END;
}

int getWord(wchar_t **word)
{
    wchar_t buf[128];

    while (fgetws(buf, sizeof(buf) / sizeof(buf[0]), stdin) != NULL) {
        size_t gwlen = wcslen(buf);

        if (gwlen > 0 && buf[gwlen - 1] == L'\n') buf[--gwlen] = L'\0';

        if (gwlen == 0) {
            fwprintf(stdout, L"> No-input -> Retry.\n");
            continue;
        }

        *word = wcsdup(buf);
        if (*word == NULL) {
            fprintf(stderr, "> Failed to execute wcsdup in function (getWord).\n");
            return GWR_ERR;
        }
        return GWR_OK;
    }

    return GWR_EOF;
}

int findNo_Hiragana(wchar_t *w)
{
    for (int i = 0; w[i] != L'\0'; i++) {
        wchar_t tw = w[i];
        if ((tw < L'ぁ' || tw > L'ゖ') && tw != L'ー') return FNHR_DNY;
    }

    return FNHR_ALW;
}

unsigned int hashValue (wchar_t *w)
{
    unsigned int hashV = 5381;

    while(*w != L'\0') {
        hashV = (hashV << 5) + hashV + (unsigned int)(*w);
        w++;
    }

    return hashV % WHTSIZE;
}

int checkWard_Existed(wchar_t *w)
{
    unsigned int hashV = hashValue(w);
    wardsChain *wc = wht[hashV];

    while (wc != NULL) {
        if (!wcscmp(wc->word, w)) return CWR_FND;
        wc = wc->next;
    }

    if (createChain(&wht[hashV], w) != EXIT_SUCCESS ) {
        fprintf(stderr, "> Failed to execute function (createChain).\n");
        return CWR_ERR;
    }

    return CWR_NFND;
}

int createChain(wardsChain **wc, wchar_t *w)
{
    wardsChain *t = (wardsChain*)calloc(1, sizeof(wardsChain));
    if (t == NULL) {
        fprintf(stderr, "> Failed to execute calloc in function (createChain).\n");
        return EXIT_FAILURE;
    }

    t->word = wcsdup(w);
    if (t->word == NULL) {
        free(t);
        fprintf(stderr, "> Failed to execute wcsdup in function (createChain).\n");
        return EXIT_FAILURE;
    }

    t->next = *wc;
    *wc = t;

    t->prev = whist;
    whist = t;

    return EXIT_SUCCESS;
}

int isRetryCommand(wchar_t *w)
{
    return !wcscmp(w, L"Retry");
}

int commandControl(gameStatus *gs, int initHp, wchar_t **prev, wchar_t *word)
{
    if (isRetryCommand(word)) {
        free(word);
        free(*prev);
        *prev = NULL;
        free_table();
        gs->hp = initHp;
        fwprintf(stdout, L"> Reset -> Restart.\n");
        fwprintf(stdout, L"> REST HP = %d\n", gs->hp);
        return 1;
    }

    unsigned int prevNUM;
    if (isPrevCommand(word, &prevNUM)) {
        showPrev(prevNUM);
        free(word);
        return 1;
    }

    return 0;
}

int isPrevCommand(wchar_t *w, unsigned int *n)
{
    const wchar_t *prefix = L"prev";
    size_t plen = wcslen(prefix);

    if (wcsncmp(w, prefix, plen) != 0) return 0;

    const wchar_t *digits = w + plen;
    if (digits[0] == L'\0') return 0;

    for (int i = 0; digits[i] != L'\0'; i++) {
        if (digits[i] < L'0' || digits[i] > L'9') return 0;
    }

    *n = wcstol(digits, NULL, 10);
    return 1;
}

void showPrev(unsigned int n)
{
    wardsChain *wc = whist;

    for (unsigned int i = 0; i < n && wc != NULL; i++) {
        wc = wc->prev;
    }

    if (wc == NULL) {
        fwprintf(stdout, L"> prev%ld is out of range.\n", n);
    } else {
        fwprintf(stdout, L"> prev%ld : %ls\n", n, wc->word);
    }
}

unsigned int dictHash(wchar_t *w)
{
    unsigned int h = 2166136261u;

    for (int i = 0; w[i] != L'\0'; i++) {
        h ^= (unsigned int)w[i];
        h *= 16777619u;
    }

    return h % DICTSIZE;
}

int loadDictionary(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "> Failed to open dictionary file (%s).\n", path);
        return EXIT_FAILURE;
    }

    wchar_t buf[128];
    while (fgetws(buf, sizeof(buf) / sizeof(buf[0]), fp) != NULL) {
        size_t len = wcslen(buf);
        if (len > 0 && buf[len - 1] == L'\n') buf[--len] = L'\0';
        if (len == 0) continue;

        unsigned int h = dictHash(buf);

        wardsChain *t = (wardsChain*)calloc(1, sizeof(wardsChain));
        if (t == NULL) {
            fprintf(stderr, "> Failed to execute calloc in function (loadDictionary).\n");
            fclose(fp);
            return EXIT_FAILURE;
        }

        t->word = wcsdup(buf);
        if (t->word == NULL) {
            free(t);
            fprintf(stderr, "> Failed to execute wcsdup in function (loadDictionary).\n");
            fclose(fp);
            return EXIT_FAILURE;
        }

        t->next = dict[h];
        dict[h] = t;
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

int checkWard_InDictionary(wchar_t *w)
{
    unsigned int h = dictHash(w);
    wardsChain *wc = dict[h];

    while (wc != NULL) {
        if (!wcscmp(wc->word, w)) return DWR_FND;
        wc = wc->next;
    }

    return DWR_NFND;
}

void free_dictionary()
{
    for (int i = 0; i < DICTSIZE; i++) {
        wardsChain *wc = dict[i];
        while (wc != NULL) {
            wardsChain *next = wc->next;
            free(wc->word);
            free(wc);
            wc = next;
        }
        dict[i] = NULL;
    }
}

void free_table()
{
    for (int i = 0; i < WHTSIZE; i++) {
        wardsChain *wc = wht[i];
        while (wc != NULL) {
            wardsChain *next = wc->next;
            free(wc->word);
            free(wc);
            wc = next;
        }
        wht[i] = NULL;
    }
    whist = NULL;
}