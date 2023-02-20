#include <stdio.h>
#include "calc3.h"
#include "y.tab.h"

/* Variable to hold label value */
int lbl = 0;

/* Function to handle print */
void output() {
printf("\tmovq\t$format, %%rdi\n");
printf("\tpopq\t%%rsi\n");
printf("\txor\t%%rax, %%rax\n");
printf("\tcall\tprintf\n");
}

/* Function to handle unary minus */
void output() {
  printf("\tpopq\t%%rsi\n");
  printf("\tmovq\t$format, %%rdi\n");
  printf("\txor\t%%rax, %%rax\n");
  printf("\tcall\tprintf\n");
}

void negation() {
  printf("\tpopq %%rax\n");
  printf("\tcmp $0, %%rax\n");
  printf("\tjge .L1\n");
  printf("\tneg %%rax\n");
  printf(".L1:\n");
  printf("\tpushq %%rax\n");
}

/* Function to handle factorial */
void factorial() {
}

/* Function to handle log base 2 */
void logarithm_base2() {
}

/* Function to handle GCD */
void greatest_common_divisor() {
printf("\tpopq\t%%rbx\n");
printf("\tpopq\t%%rax\n");
printf("\tcall gcd\n");
printf("\tpushq\t%%rax\n");
}

/* Function to handle addition */
void sum() {
printf("\tpopq\t%%rbx\n");
printf("\tpopq\t%%rax\n");
printf("\tadd \t%%rbx, %%rax\n");
printf("\tpushq\t%%rax\n");
}

/* Function to handle subtraction */
void difference() {
printf("\tpopq\t%%rbx\n");
printf("\tpopq\t%%rax\n");
printf("\tsub \t%%rbx, %%rax\n");
printf("\tpushq\t%%rax\n");
}

/* Function to handle multiplication */
void product() {
printf("\tpopq\t%%rbx\n");
printf("\tpopq\t%%rax\n");
printf("\tmul\t %%rbx\n");
printf("\tpushq\t%%rax\n");
}

/* Function to handle division */
void quotient() {
printf("\tpopq\t%%rbx\n");
printf("\tpopq\t%%rax\n");
printf("\tcqto\n");
printf("\tidiv\t%%rbx\n");
printf("\tpushq\t%%rax\n");
}

/* Function to handle modulus */
void compare_and_set(const char* condition) {
    printf("\tpopq\t%%rbx\n");
    printf("\tpopq\t%%rax\n");
    printf("\tmovq\t$0, %%rcx\n");
    printf("\tcmpq\t%%rax, %%rbx\n");
    printf("\tj%s\t.L1\n", condition);
    printf("\tmovq\t$0, %%rcx\n");
    printf("\tjmp\t.L2\n");
    printf(".L1:\n");
    printf("\tmovq\t$1, %%rcx\n");
    printf(".L2:\n");
    printf("\tpushq\t%%rcx\n");
}

void greater() {
    compare_and_set("g");
}

void le() {
    compare_and_set("le");
}

void ne() {
    compare_and_set("ne");
}

void eq() {
    compare_and_set("e");
}

void lesser() {
    compare_and_set("l");
}

void ge() {
    compare_and_set("ge");
}


int execute(nodeType *p) {
    

    if (!p) return 0;

    int label1, label2;

    switch(p->type) {
        case constant:       
            printf("\tpushq\t$%d\n", p->constant.value);
            break;
        case identifier:        
            printf("\tpushq\t%c\n", p->identifier.i + 'a');
            break;
        case operatorNode:
            switch(p->operator.oper) {
            case WHILE:
                printf("L%03d:\n", label1 = lbl++);
                execute(p->operator.operand[0]);
                printf("\tpopq\t%%rcx\n");
                printf("\ttest\t%%cx, %%cx\n");
                printf("\tje\tL%03d\n", label2 = lbl++);
                execute(p->operator.operand[1]);
                printf("\tjmp\tL%03d\n", label1);
                printf("L%03d:\n", label2);
                break;
            case IF:
                execute(p->operator.operand[0]);
                if (p->operator.numOperands > 2) {
                    /* if else */
                    printf("\tpopq\t%%rcx\n");
                    printf("\tjecxz\tL%03d\n", label1 = lbl++); 
                    execute(p->operator.operand[1]);
                    printf("\tjmp\tL%03d\n", label2 = lbl++);
                    printf("L%03d:\n", label1);
                    execute(p->operator.operand[2]);
                    printf("L%03d:\n", label2);
                } else {
                    /* if */
                    printf("\tpopq\t%%rcx\n");
                    printf("\tjecxz\tL%03d\n", label1 = lbl++); 
                    execute(p->operator.operand[1]);
                    printf("L%03d:\n", label1);
                }
                break;
            case Output:     
                execute(p->operator.operand[0]);
                output();
                break;
            case '=':       
                execute(p->operator.operand[1]);
                printf("\tpopq\t%c\n", p->operator.operand[0]->identifier.i + 'a');
                break;
            case Negation:    
                execute(p->operator.operand[0]);
                negation();
                break;
            case Factorial:
                execute(p->operator.operand[0]);
                factorial();
                break;
            case Logarithm_base2:
                execute(p->operator.operand[0]);
                logarithm_base2();
                break;
            default:
                execute(p->operator.operand[0]);
                execute(p->operator.operand[1]);
                switch(p->operator.oper) {
                    case GCD:
                        greatest_common_divisor();
                        break;
                   case '<':
                        greater();
                        break;
                    case '>':
                        lesser();
                        break;
                    case GE:
                        ge();
                        break;
                    case LE:
                        le();
                        break;
                    case NE:
                        ne();
                        break;
                    case EQ:
                        eq();
                        break;
                    case '+':
                        sum();
                        break;
                    case '-':
                        difference();
                        break;         
                    case '*':
                        product();
                        break;
                    case '/':
                        quotient();
                        break;

                }
            }
    }

    return 0;
}

