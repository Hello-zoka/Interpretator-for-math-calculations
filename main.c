#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <stdarg.h>
#include "UGL.H"

jmp_buf ExprJumpBuf;

void Error( char *Str, ... )
{
    va_list ap;
    printf("ERROR: ");
    va_start(ap, Str);
    vprintf(Str, ap);
    va_end(ap);
    printf("\n");
    longjmp(ExprJumpBuf, 1);
}


void GetStr( char *Str, int MaxLen )
{
    int i = 0;
    char ch;

    while ((ch = getchar()) != '\n')
        if (Str != NULL && i < MaxLen - 1)
            Str[i++] = ch;
    if (Str != NULL && i < MaxLen)
        Str[i] = 0;
}


int main( void )
{
    char str[MAX];
    FILE *F;
    QUEUE Q = {NULL, NULL};
    QUEUE QRes = {NULL, NULL};

    if (setjmp(ExprJumpBuf))
    {
        ClearStack(&StackEval);
        ClearQueue(&Q);
        ClearQueue(&QRes);
        ClearQueue(&Queue1);
        ClearStack(&Stack2);
//    _getch();
        return 0;
    }

    printf("Input file name: ");
    GetStr(str, sizeof(str));
    F = fopen(str, "r");
    if (F != NULL)
    {
        static char Buf[10000];
        while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
            Scanner(&TokList, Buf);
        fclose(F);
    }
//    DisplayQueue(&TokList);
    ParseProgram();
    DoCmd(Proga);

    DisplayVarTable();

    ClearQueue(&QRes);
}