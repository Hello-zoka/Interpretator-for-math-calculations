#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "UGL.H"

void UpdateKeyword( TOK *T )
{
    struct
    {
        char *Name;
        KEYWORDS KeyW;
    } Kws[] =
            {
                    {"if", KW_IF},
                    {"else", KW_ELSE},
                    {"while", KW_WHILE},
                    {"print", KW_PRINT}
            };
    int i;

    for (i = 0; i < sizeof(Kws) / sizeof(Kws[0]); i++)
        if (strcmp(Kws[i].Name, T->Name) == 0)
        {
            T->Id = TOK_KEYW;
            T->KeyW = Kws[i].KeyW;
            return;
        }
}

void Scanner( QUEUE *Q, char *S )
{
    TOK T;
    int i;

    while(*S != 0)
    {
        switch(*S)
        {
            case ' ':
            case '\n':
                S++;
                continue;
            case '+':
            case '-':
            case '*':
            case ',':
            case '=':
            case '<':
            case '>':
            case '!':
            case '/':
            case '%':
            case '^':
            case '(':
            case ')':
                T.Id = TOK_OP;
                T.Op = *S++;
                if (*S == '=' && (T.Op == '<' || T.Op == '=' || T.Op == '>' || T.Op == '!'))
                    T.Op += TOK_OP_ADD_EQ, S++;
                break;
            case ';':
            case '{':
            case '}':
                T.Id = TOK_SYMBOL;
                T.Op = *S++;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                T.Id = TOK_NUM;
                T.Num = 0;
                while (*S == '0' || *S == '1' || *S == '2' || *S == '3' || *S == '4' || *S == '5' || *S == '6' || *S == '7' || *S == '8' || *S == '9')
                    T.Num = T.Num * 10 + *S++ - '0';
                if (*S == '.')
                {
                    double denum = 1;

                    S++;
                    while (*S == '0' || *S == '1' || *S == '2' || *S == '3' || *S == '4' || *S == '5' || *S == '6' || *S == '7' || *S == '8' || *S == '9')
                        T.Num += (*S++ - '0') / (denum *= 10);
                }
                break;
            default:
                if (isalpha((unsigned char)*S))
                {
                    T.Id = TOK_NAME;
                    i = 0;
                    do
                    {
                        if (i < MAX - 1)
                            T.Name[i++] = *S;
                        S++;
                    } while (isalpha((unsigned char)*S) || isdigit((unsigned char)*S));
                    T.Name[i] = 0;
                    UpdateKeyword(&T);
                    break;
                }
                Error("Unrecognized character '%c'\n", *S);
                break;
        }
        Put(Q, T);
    }
}
