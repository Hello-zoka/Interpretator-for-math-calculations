#include <stdio.h>
#include <string.h>
#include "UGL.H"

QUEUE Queue1;
STACK Stack2;
QUEUE TokList;
TOK TokCurrent;
int IsEOTL;
#define NextTok() \
  (IsEOTL ? Error("Unexpected end-of-data") : Get(&TokList, &TokCurrent) ? (void)0 : (void)(IsEOTL = 1))
#define IsTokAnyOp() \
  (!IsEOTL && TokCurrent.Id == TOK_OP)
#define IsTokAnySym() \
  (!IsEOTL && TokCurrent.Id == TOK_SYMBOL)
#define IsTokAnyNum() \
  (!IsEOTL && TokCurrent.Id == TOK_NUM)
#define IsTokAnyName() \
  (!IsEOTL && TokCurrent.Id == TOK_NAME)
#define IsTokOp(C) \
  (IsTokAnyOp() && TokCurrent.Op == (C))
#define IsTokSym(C) \
  (IsTokAnySym() && TokCurrent.Op == (C))
#define IsTokNum(C) \
  (IsTokAnyNum() && TokCurrent.Op == (C))
#define IsTokName(C) \
  (IsTokAnyName() && TokCurrent.Op == (C))
#define IsTokKw(Keyword) \
  (!IsEOTL && TokCurrent.Id == TOK_KEYW && TokCurrent.KeyW == KW_ ## Keyword)
#define ParseAnyTok() \
  NextTok();
#define ParseAnyOp() \
  (IsTokAnyOp ? NextTok() : Error("Expecte any operator"))
#define ParseOp(C) \
  (IsTokOp(C) ? NextTok() : Error("Expected oprator %c", (C)))
#define ParseSym(C) \
  (IsTokSym(C) ? NextTok() : Error("Expected symbol %c", (C)))
#define ParseNum(X) \
  (IsTokAnyNum(C) ? (*(X) = TokCurrent.Num, NextTok()) : Error("Expected number"))
#define ParseKw(Keyword) \
  (IsTokKw(Keyword) ? NextTok() : Error("Expected keyword: \"" #Keyword "\""))



int GetPrior( OPERS Op )
{
    switch (Op)
    {
        case '(':
            return 0;
        case ')':
            return 1;
        case OP_LT:
        case OP_GT:
        case OP_EQ:
        case OP_NE:
        case OP_LE:
        case OP_GE:
            return 2;
        case ',':
            return 3;
        case '=':
            return 4;
        case OP_PLUS:
        case OP_MINUS:
            return 5;
        case OP_MULT:
        case OP_DIV:
        case OP_MOD:
            return 6;
        case OP_POW:
            return 7;
        case OP_UNAR:
            return 8;
    }
    Error("Unknown operator: '%c'", Op);

    return -1;
}

int CheckAssoc( OPERS Op1, OPERS Op2 )
{
    int p1 = GetPrior(Op1), p2 = GetPrior(Op2);

    if (Op2 == '=')
        return p1 > p2;
    return p1 >= p2;
}
void DropOpers( OPERS Op )
{
    while (Stack2.Top != NULL && CheckAssoc(Stack2.Top->Data.Op, Op))
    {
        TOK T;
        Pop(&Stack2, &T);
        Put(&Queue1, T);
    }
}

void ParseExpr( LIST **Expr )
{
    TOK T;
    enum
    {
        STATE_PREFIX, STATE_SUFFIX, STATE_DONE, STATE_END
    } state = STATE_PREFIX;
    Queue1.Head = NULL;
    Stack2.Top = NULL;

    while (state != STATE_END)
    {
        switch (state)
        {
            case STATE_PREFIX:
                if (IsTokAnyNum() || IsTokAnyName())
                    Put(&Queue1, TokCurrent), state = STATE_SUFFIX, NextTok();
                else if (IsTokOp('('))
                    Push(&Stack2, TokCurrent), NextTok();
                else if (IsTokOp('-')) {
                    TokCurrent.Op = OP_UNAR;
                    Push(&Stack2, TokCurrent), NextTok();
                }
                else
                    Error("Wait for number or '(' \n");
                break;
            case STATE_SUFFIX:
                if (IsTokAnyOp() && !IsTokOp('('))
                {
                    DropOpers(TokCurrent.Op);
                    if (!IsTokOp(')'))
                    {
                        Push(&Stack2, TokCurrent);
                        state = STATE_PREFIX;
                    }
                    else
                    {
                        if (!Pop(&Stack2, &TokCurrent)) {
                            state = STATE_DONE;
                            break;
                        }
                    }
                    ParseAnyTok();

                }
                else
                    state = STATE_DONE;
                break;
            case STATE_DONE:
                DropOpers(')');
                if (Stack2.Top != NULL)
                    Error("Missing ')'");
                *Expr = Queue1.Head;
                Queue1.Head = NULL;
                Stack2.Top = NULL;
                state = STATE_END;
                break;
        }
    }
}
void CmdInit( CMD **C )
{
    if ((*C = malloc(sizeof(CMD))) == NULL)
        Error("No memory");
    (*C)->Id = CMD_EXPR;
    (*C)->Expr = NULL;
    (*C)->C1 = (*C)->C2 = (*C)->Next = NULL;
}

void ParseCmd( CMD **C )
{
    *C = NULL;
    if (IsTokKw(IF))
    {
        CmdInit(C);
        (*C)->Id = CMD_IF;
        ParseKw(IF);
        ParseOp('(');
        ParseExpr(&(*C)->Expr);
        ParseOp(')');
        ParseCmd(&(*C)->C1);
        if (IsTokKw(ELSE))
        {
            ParseKw(ELSE);
            ParseCmd(&(*C)->C2);
        }
    }
    else if (IsTokKw(WHILE))
    {
        CmdInit(C);
        (*C)->Id = CMD_WHILE;
        ParseKw(WHILE);
        ParseOp('(');
        ParseExpr(&(*C)->Expr);
        ParseOp(')');
        ParseCmd(&(*C)->C1);
    }
    else if (IsTokSym('{'))
    {
        ParseSym('{');
        while (!IsTokSym('}'))
        {
            CmdInit(C);
            ParseCmd(C);
            C = &(*C)->Next;
        }
        ParseSym('}');
    }
    else if (IsTokKw(PRINT))
    {
        CmdInit(C);
        (*C)->Id = CMD_PRINT;
        ParseKw(PRINT);
        ParseOp('(');

        ParseExpr(&(*C)->Expr);

        ParseOp(')');
        ParseSym(';');
    }
    else
    {
        CmdInit(C);
        (*C)->Id = CMD_EXPR;
        ParseExpr(&(*C)->Expr);
        ParseSym(';');
    }
}

void ParseProgram( void )
{
    CMD **cmd = &Proga;

    Proga = NULL;
    IsEOTL = 0;

    NextTok();
    while (!IsEOTL)
    {
        ParseCmd(cmd);
        cmd = &(*cmd)->Next;
    }
}