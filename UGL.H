/* Debug memory allocation support */
#ifndef NDEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* _DEBUG */
#include <stdlib.h>
#include <stdio.h>
#define MAX 100
#define TOK_OP_ADD_EQ 300
typedef enum 
{
  TOK_OP, TOK_NUM, TOK_NAME, TOK_KEYW, TOK_SYMBOL
} TOKID;
typedef enum 
{
  KW_IF, KW_ELSE, KW_WHILE, KW_PRINT, KW_SCAN
} KEYWORDS;
typedef enum 
{
  OP_PLUS = '+',
  OP_MINUS = '-',
  OP_MULT  = '*',
  OP_DIV = '/',
  OP_POW = '^',
  OP_MOD = '%',
  OP_LT = '<',
  OP_GT = '>',
  OP_EQ = '=' + TOK_OP_ADD_EQ,
  OP_NE = '!' + TOK_OP_ADD_EQ,
  OP_GE = '>' + TOK_OP_ADD_EQ,
  OP_LE = '<' + TOK_OP_ADD_EQ,
} OPERS;
typedef struct
{
  TOKID Id;
  OPERS Op;
  double Num;
  char Name[MAX];
  KEYWORDS KeyW;
} TOK;
typedef struct tagLIST LIST;
struct tagLIST
{
  TOK Data;
  LIST *Next;
};
typedef struct
{
  LIST *Head, *Tail;
} QUEUE;
typedef struct
{
  LIST *Top;
} STACK;
typedef enum
{
  CMD_EXPR, CMD_IF, CMD_WHILE, CMD_PRINT
} CMDID;
typedef struct tagCMD CMD;
struct tagCMD
{
  CMDID Id;
  LIST *Expr;
  CMD *C1, *C2, *Next;
};
CMD *Proga;

extern STACK StackEval;
extern QUEUE QRes;
extern QUEUE TokList;
extern TOK TokCurrent;
void DisplayList( LIST *L );
void ClearList( LIST **L );
void Put( QUEUE *Q, TOK NewData );
int Get( QUEUE *Q, TOK *OldData );
void Push( STACK *S, TOK NewData );
int Pop( STACK *S, TOK *OldData );
void DisplayStack( STACK *S );
void DisplayQueue( QUEUE *Q );
void ClearStack( STACK *S );
void ClearQueue( QUEUE *Q );
void PrintTok( TOK T );
void Scanner( QUEUE *Q, char *S );
void Error( char *Str, ... );
double Eval( LIST *Expr );
void DropOpers( char Op );
extern QUEUE Queue1;
extern STACK Stack2;
int GetPrior( char Op );
void ParseExpr( LIST **Expr );
int CheckAssoc( char Op1, char Op2);
void SetValue( char *Name, double Value );
double GetValue( char *Name );
void DisplayVarTable( void );
void UpdateKeyword( TOK *T );
void DoCmd( CMD *C );
void ParseCmd( CMD **C );
void ParseProgram( void );














