#include <stdio.h>
#include <string.h>
#include "UGL.H"

void DisplayList( LIST *L )
{
    if (L == NULL)
        printf("<Empty list>\n");
    else
        while (L != NULL)
        {
            PrintTok(L->Data);
            printf((L = L->Next) == NULL ? "\n" : ", ");
        }
}


void ClearList( LIST **L )
{
    LIST *tmp;

    while (*L != NULL)
    {
        tmp =  *L;
        *L = (*L)->Next;
        free(tmp);
    }
}

void Put( QUEUE *Q, TOK NewData )
{
    LIST *NewElement = malloc(sizeof(LIST));

    if (NewElement == NULL)
        Error("No memory in put ");
    NewElement->Data = NewData;
    NewElement->Next = NULL;
    if (Q->Head == NULL)
        Q->Head = Q->Tail = NewElement;
    else
        Q->Tail->Next = NewElement, Q->Tail = NewElement;
}

int Get( QUEUE *Q, TOK *OldData )
{
    LIST *NewElement = malloc(sizeof(LIST));
    if (Q->Head == NULL)
        return 0;
    NewElement =  Q->Head->Next;
    *OldData =  Q->Head->Data;
    free(Q->Head);
    Q->Head = NewElement;
    return 1;
}
void Push( STACK *S, TOK NewData )
{
    LIST *NewElement = malloc(sizeof(LIST));
    if (NewElement == NULL)
        Error("No memory");
    NewElement->Data = NewData;
    NewElement->Next = S->Top;
    S->Top = NewElement;
}
int Pop (STACK *S, TOK *OldData)
{
    LIST *NewElement = malloc(sizeof(LIST));
    if (S->Top == NULL)
        return 0;
    NewElement =  S->Top->Next;
    *OldData =  S->Top->Data;

    S->Top = NewElement;
    return 1;
}
void DisplayStack( STACK *S )
{
    DisplayList(S->Top);
}
void DisplayQueue( QUEUE *Q )
{
    DisplayList(Q->Head);
}
void ClearStack( STACK *S )
{
    ClearList(&S->Top);
}
void ClearQueue( QUEUE *Q )
{
    ClearList(&Q->Head);
}
void PrintTok( TOK T )
{
    if (T.Id == TOK_OP)
        printf("op: %c ", T.Op);
    else if (T.Id == TOK_NUM)
        printf("integer: %f ", T.Num);
    else if (T.Id == TOK_SYMBOL)
        printf("symbol: %c ", T.Op);
    else if (T.Id == TOK_NAME)
        printf("name: %s ", T.Name);
    else if (T.Id == TOK_KEYW)
        printf ("keyword: %s ", T.Name);
    else
        printf("UNKNOWN ");
}