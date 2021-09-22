#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <math.h>
#include "UGL.H"

STACK StackEval;


double Eval( LIST *Expr )
{
    TOK T, A, B;

    while (Expr != NULL)
    {
        T = Expr->Data;
        Expr = Expr->Next;

        if (T.Id == TOK_NUM || T.Id == TOK_NAME) {
            Push(&StackEval, T);
        }
        else
        {
            if (T.Op != '@')
            {
                Pop(&StackEval, &B);
                if (B.Id == TOK_NAME)
                    B.Num = GetValue(B.Name);
            }
            Pop(&StackEval, &A);
            if (A.Id == TOK_NAME && T.Op != '=') {
                A.Num = GetValue(A.Name);
                A.Id = TOK_NUM;
            }
            switch(T.Op)
            {
                case '+':
                    A.Num += B.Num;
                    break;
                case '-':
                    A.Num -= B.Num;
                    break;
                case '*':
                    A.Num *= B.Num;
                    break;
                case '/':
                    A.Num /= B.Num;
                    break;
                case '%':
                    A.Num = fmod(A.Num, B.Num);
                    break;
                case '^':
                    A.Num  = pow(A.Num, B.Num);
                    break;
                case '(':
                case ')':
                    break;
                case '=':
                    SetValue(A.Name, B.Num);
                    A.Id = TOK_NUM;
                    A.Num = B.Num;
                    break;
                case OP_EQ:
                    A.Num = A.Num == B.Num;
                    break;
                case OP_NE:
                    A.Num = A.Num != B.Num;
                    break;
                case OP_GE:
                    A.Num = A.Num >= B.Num;
                    break;
                case OP_LE:
                    A.Num = A.Num <= B.Num;
                    break;
                case ',':
                    A.Id = TOK_NUM;
                    A.Num = B.Num;
                    break;
            }
            Push(&StackEval, A);
        }
    }
    Pop(&StackEval, &T);
    if (T.Id == TOK_NAME) {
        T.Num = GetValue(T.Name);
        T.Id = TOK_NUM;
    }
    return T.Num;
}

void DoCmd( CMD *C )
{
    while (C != NULL)
    {
        if (C->Id == CMD_EXPR)
            Eval(C->Expr);
        else if (C->Id == CMD_IF)
            if (Eval(C->Expr))
                DoCmd(C->C1);
            else
                DoCmd(C->C2);
        else if (C->Id == CMD_WHILE)
            while (Eval(C->Expr))
                DoCmd(C->C1);
        else if (C->Id == CMD_PRINT)
        {
            printf("%f\n", Eval(C->Expr));
        }
        C = C->Next;
    }
}