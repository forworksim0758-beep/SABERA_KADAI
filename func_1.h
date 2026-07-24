#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#ifndef FUNC1_H
#define FUNC1_H

typedef enum {
    GWR_ERR = -1,
    GWR_OK,
    GWR_EOF,

}GWR;

typedef enum {
    FNHR_ERR = -1,
    FNHR_ALW,
    FNHR_DNY,

}FNHR;

typedef enum {
    CWR_ERR = -1,
    CWR_FND ,
    CWR_NFND,

}CWR;

typedef enum {
    DWR_ERR = -1,
    DWR_FND,
    DWR_NFND,

}DWR;

typedef enum {
    ARR_ERR = -1,
    ARR_ALW,
    ARR_DNY,

}ARR;

typedef enum {
    JCR_ERR = -1,
    JCR_CONTINUE,
    JCR_END,

}JCR;

typedef enum {
    GM_EASY,
    GM_NORMAL,
    GM_DIFFICULT,

}GM;

typedef enum {
    SDR_ERR = -1,
    SDR_OK,
    SDR_QUIT,

}SDR;

typedef enum {
    CCR_NONE,
    CCR_CONTINUE,
    CCR_RESTART,

}CCR;


#define WHTSIZE 4096
typedef struct wardsChain {
    wchar_t *word;
    struct wardsChain *next;
    struct wardsChain *prev;
}wardsChain;
extern wardsChain *wht[WHTSIZE];
extern wardsChain *whist;

#define DICTSIZE 262144
extern wardsChain *dict[DICTSIZE];

typedef struct {
    unsigned int hp;
    GM mode;
}gameStatus;

int selectDifficulty(gameStatus *gs);
int gameLoop(gameStatus *gs);
wchar_t normalizeKana(wchar_t c);
wchar_t effectiveLastChar(wchar_t *w);
int applyRules(gameStatus *gs, wchar_t *prev, wchar_t *w);
int judgeContinue(gameStatus *gs);

int getWord(wchar_t **word);
int findNo_Hiragana(wchar_t *w);

int checkWard_Existed(wchar_t *w);
int createChain(wardsChain **wc, wchar_t *w);
unsigned int hashValue (wchar_t *w);

int isRetryCommand(wchar_t *w);
int isPrevCommand(wchar_t *w, unsigned int *n);
void showPrev(unsigned int n);
int commandControl(wchar_t *word);

unsigned int dictHash(wchar_t *w);
int loadDictionary(const char *path);
int checkWard_InDictionary(wchar_t *w);
void free_dictionary();

void free_table();


#endif
