#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEVMAX 3 /* maximum depth of block nesting */
#define CXMAX 200 /* size of code array */
#define STACKESIZE 500

typedef enum {LIT, OPR, LOD, STO, CAL, INT, JMP, JPC} fct;

typedef struct {
  fct f;
  int l;
  int a;
} instruction;

/* global variables */
instruction *code; /* code */
int p, b, t; /* program register, base register, topstack register */
instruction i; /* instruction register */
int s[STACKESIZE] = {0}; /* datastore */

fct fct_to_enum(char *fct_str) {
  if(strcmp(fct_str, "LIT") == 0) {
    return LIT;
  }
  if(strcmp(fct_str, "OPR") == 0) {
    return OPR;
  }
  if(strcmp(fct_str, "LOD") == 0) {
    return LOD;
  }
  if(strcmp(fct_str, "STO") == 0) {
    return STO;
  }
  if(strcmp(fct_str, "CAL") == 0) {
    return CAL;
  }
  if(strcmp(fct_str, "INT") == 0) {
    return INT;
  }
  if(strcmp(fct_str, "JMP") == 0) {
    return JMP;
  }
  if(strcmp(fct_str, "JPC") == 0) {
    return JPC;
  }
}

void fct_to_string(fct f, char *fct_str) {
  switch(f) {
    case LIT:
      strcpy(fct_str, "LIT");
      break;

    case OPR:
      strcpy(fct_str, "OPR");
      break;
    
    case LOD:
      strcpy(fct_str, "LOD");
      break;

    case STO:
      strcpy(fct_str, "STO");
      break;

    case CAL:
      strcpy(fct_str, "CAL");
      break;
    
    case INT:
      strcpy(fct_str, "INT");
      break;
    
    case JMP:
      strcpy(fct_str, "JMP");
      break;

    case JPC:
      strcpy(fct_str, "JPC");
      break;
  }
}

int base(int l) {
  int b1 = b;

  while(l > 0) {
    b1 = s[b1];
    l -= 1;
  }

  return b1;
}

void interpret() {
  char f[4];
  int code_len = 0;

  t = 0; b = 1; p = 0;
  s[1] = 0; s[2] = 0; s[3] = 0;

  int row_counter = 1;
  int iterator;

  printf("start pl/0");
  printf("\n");

  do {
    code_len += 1;
    code = (instruction *) realloc(code, sizeof(instruction) * code_len);

    scanf("%s %d %d\n", f, &(code[code_len - 1].l), &(code[code_len - 1].a));
    code[code_len - 1].f = fct_to_enum(f);

    i = code[p]; p += 1;

    printf("\n\t%d.", p);
    fct_to_string(i.f, f);
    printf(" Instruction => %s %d %d\t|", f, i.l, i.a);

    switch(i.f) {
      case LIT:
        t += 1;
        s[t] = i.a;
        break;

      case OPR:
        switch(i.a) {
          case 0:
            t = b - 1;
            p = s[t + 3];
            b = s[t + 2];
            break;

          case 1:
            s[t] *= -1;
            break;

          case 2:
            t -= 1;
            s[t] += s[t + 1];
            break;

          case 3:
            t -= 1;
            s[t] -= s[t + 1];
            break;

          case 4:
            t -= 1;
            s[t] *= s[t + 1];
            break;

          case 5:
            t -= 1;
            s[t] /= s[t + 1];
            break;

          case 6:
            s[t] = s[t] % 2;
            break;

          case 8:
            t -= 1;
            s[t] = s[t] == s[t + 1];
            break;

          case 9:
            t -= 1;
            s[t] = s[t] != s[t + 1];
            break;

          case 10:
            t -= 1;
            s[t] = s[t] < s[t + 1];
            break;

          case 11:
            t -= 1;
            s[t] = s[t] >= s[t + 1];
            break;

          case 12:
            t -= 1;
            s[t] = s[t] > s[t + 1];
            break;

          case 13:
            t -= 1;
            s[t] = s[t] <= s[t + 1];
            break;
        }
        break;
      
      case LOD:
        t += 1;
        s[t] = s[base(i.l) + i.a];
        break;

      case STO:
        s[base(i.l) + i.a] = s[t];
        t -= 1;
        break;

      case CAL:
        s[t + 1] = base(i.l);
        s[t + 2] = b;
        s[t + 3] = p;
        b = t + 1;
        p = i.a;
        break;
      
      case INT:
        t = t + i.a;
        break;
      
      case JMP:
        p = i.a;
        break;

      case JPC:
        if(s[t] == 0) {
          p = i.a;
          t -= 1;
        }
        break;
    }

    printf(" P => %d; B => %d; T => %d;\t|", p, b, t);
    printf(" Stack => ");
    for(iterator = 1; iterator <= t; ++iterator) {
      printf("[%d]", s[iterator]);
    }
    printf("\n");
  } while(p != 0);
  pipes_counter = 0;
  while(pipes_counter < (LEVMAX - 1) && iterator != pipes[pipes_counter]) {
    printf("%d\n", pipes[pipes_counter]);
    pipes_counter += 1;
  }

  printf("\nend pl/0\n");
}

int main(void) {
  interpret();

  return 0;
}