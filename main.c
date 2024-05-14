#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{NUM=128};
char *src=NULL;
char *line=NULL;
int token;
int token_val;
char *last_pos;
/* 词法分析器 */
void next(){
    while(*src==' '||*src=='\t') src++;
    token = *src++;
    if(token>='0'&&token<='9'){
        token_val = token - '0';
        token = NUM;
        while(*src>='0'&&*src<='9'){
            token_val = token_val*10 + *src - '0';
            src++;
        }
        return;
    }
}

/* 文法 */
/*
S:=GS'
S':=+GS'
    |-GS'
    |ε
G:=EG'
G':=*EG'
    |/EG'
    |ε
E:=NUM
    |(S)
 */

/* 语法分析器 */
enum{EXPR,EXPRTAIL,TERM,TERMTAIL,FACTOR};
int now=EXPR;
void parser(){
    switch(now){
        case FACTOR:
            next();
            if(token==NUM){
                printf("E:=NUM\n");
            }else
            if(token=='('){
                printf("E:=(S)\n");
                now = EXPR;
                parser();
                next();
                if(token!=')'){
                    printf("error,should get ')'\n");
                    printf("token=%c\n",token);
                    exit(1);
                }
            }else{
                printf("error, should get num or '('\n");
                exit(1);
            }
            break;
        case TERMTAIL:
            last_pos = src;
            next();
            if(token=='*'){
                printf("G':=*EG'\n");
                now = FACTOR;
                parser();
                now = TERMTAIL;
                parser();
                break;
            }
            else if(token=='/'){
                printf("G':=/EG'\n");
                now = FACTOR;
                parser();
                now = TERMTAIL;
                parser();
                break;
            }
            else{
                printf("G':=empty \n");
                src = last_pos;
                break;
            }
            break;
        case TERM:
            printf("G:=EG'\n");
            now = FACTOR;
            parser();
            now = TERMTAIL;
            parser();
            break;
        case EXPRTAIL:
            last_pos = src;
            next();
            if(token=='+'){
                printf("S':=+GS'\n");
                now = TERM;
                parser();
                now = EXPRTAIL;
                parser();
                break;
            }
            else if(token=='-'){
                printf("S':=-GS'\n");
                now = TERM;
                parser();
                now = EXPRTAIL;
                parser();
                break;
            }else {
                printf("S':=empty\n");
                src = last_pos;
                break;
            }
            break;
        case EXPR:
            printf("S:=GS'\n");
            now = TERM;
            parser();
            now = EXPRTAIL;
            parser();
            break;
        default:
            printf("error\n");
            exit(1);
    }
}

void program(){
    while(1){
        parser();
        if(token==0) break;
    }
}

int main(int argc, char *argv[]) {
//    src= "(1+2)*3";
    src= "9+5*2/3-6";
    printf("src=%s\n",src);
    program();
    system("pause");
    return 0;
}
