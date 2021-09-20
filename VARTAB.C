#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <setjmp.h>
#include <math.h>
#include "UGL.H"


typedef struct tagVARLIST VARLIST; 
typedef struct tagVARLIST
{
  char Name[MAX];
  double Value;
  VARLIST *Next;
};
VARLIST *VarTable = NULL;

void SetValue( char *Name, double Value )
{
  VARLIST **ptr = &VarTable;

  while (*ptr != NULL)
  {
    if (strcmp((*ptr)->Name, Name) == 0)
    {
      (*ptr)->Value = Value;
      return;
    }
    else
      ptr = &(*ptr)->Next;
  }
  if ((*ptr = malloc(sizeof(VARLIST))) == NULL)
    Error("Nor enuogh mem for new var");
  strncpy((*ptr)->Name, Name, MAX - 1);
  (*ptr)->Value = Value;
  (*ptr)->Next = NULL;
}
double GetValue( char *Name )
{
  VARLIST **ptr = &VarTable;

  while (*ptr != NULL)
  {
    if (strcmp((*ptr)->Name, Name) == 0)
      return (*ptr)->Value;
    else
      ptr = &(*ptr)->Next;
  }
  return 0;
}
void DisplayVarTable(void)
{
  VARLIST **ptr = &VarTable;

  while (*ptr != NULL)
  {
    printf("%s : %llf ", (*ptr)->Name, (*ptr)->Value);
    ptr = &(*ptr)->Next;
  }
}