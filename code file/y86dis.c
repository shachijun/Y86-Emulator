//
//  y86dis.c
//  Assign 4!
//
//  Created by 沙驰钧 on 2016/12/4.
//  Copyright © 2016年 沙驰钧. All rights reserved.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "convert.c"





char * getRegister(char num){
    char * Reg = (char*) malloc(sizeof(char) * 5);
    Reg[0] = '%';
    Reg[1] = 'e';
    Reg[4] = '\0';
    switch(num){
        case '0':
            Reg[2] = 'a';
            Reg[3] = 'x';
            break;
        case '1':
            Reg[2] = 'c';
            Reg[3] = 'x';
            break;
        case '2':
            Reg[2] = 'd';
            Reg[3] = 'x';
            break;
        case '3':
            Reg[2] = 'b';
            Reg[3] = 'x';
            break;
        case '4':
            Reg[2] = 's';
            Reg[3] = 'p';
            break;
        case '5':
            Reg[2] = 'b';
            Reg[3] = 'p';
            break;
        case '6':
            Reg[2] = 's';
            Reg[3] = 'i';
            break;
        case '7':
            Reg[2] = 'd';
            Reg[3] = 'i';
            break;
    }
    return Reg;
}

long hexToIntLittleEndien(char * num, bool canBeNegative){
    char number[strlen(num) + 1];
    number[sizeof(number)-1] = '\0';
    
    int i;
    int j = sizeof(number) - 2;
    for(i = 0; i<sizeof(number)-1; i++, j--){
        number[i] = num[j];
    }
    if(!canBeNegative){
        return hexToInt(number, false);
    }
    return hexToInt(number, true);
}

char * FindHex(char * input){
    char * hex = (char*) malloc(sizeof(char) * 11);
    hex[0]= '0';
    hex[1]= 'x';
    int j;
    int g = 0;
    bool copy = false;
    for(j=2;j<10;j++){
        if(copy){
            hex[g] = input[j-2];
            g++;
        }else if(input[j-2] != '0'){
            hex[g] = input[j-2];
            copy = true;
            g++;
        }
    }
    hex[g] = '\0';
    return hex;
}



void printHex(int decimalNumber){
    int remainder;
    int quotient;
    int i=1,j;
    char hexadecimalNumber[100];
    
    quotient = decimalNumber;
    
    while(quotient!=0){
        remainder = quotient % 16;
        
        if( remainder < 10)
            remainder = remainder + 48;
        else
            remainder = remainder + 55;
        hexadecimalNumber[i++]= remainder;
        quotient = quotient / 16;
    }
    hexadecimalNumber[i++] = '\0';
    printf("0x");
    for(j=8-(i-1)+1;j>0;j--){
        printf("0");
    }
    
    for(j = i -1 ;j> 0;j--)
        printf("%c", hexadecimalNumber[j]);
    printf(" ");
}

void readI(char * address, char * text){
    int i = 0;
    while(i < strlen(text) - 1){
        printHex(hexToInt(address, false) + (i/2));
        if(text[i]=='3' && text[i+1]=='0'){
            char * Reg = getRegister(text[i+3]);
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+4+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("irmovl $%d, %s\n", (int)strtol(numberD, NULL, 16), Reg);
            free(Reg);
            i += 12;
        }else if(text[i]=='4' && text[i+1]=='0'){
            char * reg1 = getRegister(text[i+2]);
            char * reg2 = getRegister(text[i+3]);
            char dis[9];
            dis[sizeof(dis) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                dis[j] = text[i+4+j];
            }
            char te1 = dis[strlen(dis)-1];
            char te2 = dis[strlen(dis)-2];
            char te3 = dis[strlen(dis)-3];
            char te4 = dis[strlen(dis)-4];
            dis[strlen(dis)-1] = dis[1];
            dis[strlen(dis)-2] = dis[0];
            dis[strlen(dis)-3] = dis[3];
            dis[strlen(dis)-4] = dis[2];
            dis[1] = te1;
            dis[0] = te2;
            dis[3] = te3;
            dis[2]= te4;
            printf("rmmovl %s, %d(%s)\n",reg1, (int)strtol(dis, NULL, 16),reg2);
            free(reg1);
            free(reg2);
            i += 12;
        }else if(text[i]=='5' && text[i+1]=='0'){
            char * reg1 = getRegister(text[i+2]);
            char * reg2 = getRegister(text[i+3]);
            char dis[9];
            dis[sizeof(dis) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                dis[j] = text[i+4+j];
            }
            char te1 = dis[strlen(dis)-1];
            char te2 = dis[strlen(dis)-2];
            char te3 = dis[strlen(dis)-3];
            char te4 = dis[strlen(dis)-4];
            dis[strlen(dis)-1] = dis[1];
            dis[strlen(dis)-2] = dis[0];
            dis[strlen(dis)-3] = dis[3];
            dis[strlen(dis)-4] = dis[2];
            dis[1] = te1;
            dis[0] = te2;
            dis[3] = te3;
            dis[2]= te4;
            printf("mrmovl %d(%s), %s\n",(int)strtol(dis, NULL, 16), reg2,reg1);
            free(reg1);
            free(reg2);
            i += 12;
        }else if(text[i]=='2' && text[i+1]=='0'){
            char * reg1 = getRegister(text[i+2]);
            char * reg2 = getRegister(text[i+3]);
            printf("rrmovl %s, %s\n", reg1, reg2);
            free(reg1);
            free(reg2);
            i += 4;
        }else if(text[i]=='0' && text[i+1]=='0'){
            printf("nop\n");
            i += 2;
        }else if(text[i]=='1' && text[i+1]=='0'){
            printf("halt\n");
            i += 2;
        }else if(text[i]=='6' && text[i+1]=='5'){
            char * reg1 = getRegister(text[i+2]);
            char * reg2 = getRegister(text[i+3]);
            printf("cmpl %s, %s\n", reg1, reg2);
            free(reg1);
            free(reg2);
            i += 4;
        }else if(text[i]=='6' && text[i+1]=='0'){
            char * reg1 = getRegister(text[i+2]);
            char * reg2 = getRegister(text[i+3]);
            printf("addl %s, %s\n", reg1, reg2);
            free(reg1);
            free(reg2);
            i += 4;
        }else if(text[i]=='6' && text[i+1]=='1'){
            char * reg1 = getRegister(text[i+2]);
            char * reg2 = getRegister(text[i+3]);
            printf("subl %s, %s\n", reg1, reg2);
            free(reg1);
            free(reg2);
            i += 4;
        }else if(text[i]=='6' && text[i+1]=='2'){
            char * reg1 = getRegister(text[i+2]);
            char * reg2 = getRegister(text[i+3]);
            printf("andl %s, %s\n", reg1, reg2);
            free(reg1);
            free(reg2);
            i += 4;
        }else if(text[i]=='6' && text[i+1]=='3'){
            char * reg1 = getRegister(text[i+2]);
            char * reg2 = getRegister(text[i+3]);
            printf("xorl %s, %s\n", reg1, reg2);
            free(reg1);
            free(reg2);
            i += 4;
        }else if(text[i]=='6' && text[i+1]=='4'){
            char * reg1 = getRegister(text[i+2]);
            char * reg2 = getRegister(text[i+3]);
            printf("mull %s, %s\n", reg1, reg2);
            free(reg1);
            free(reg2);
            i += 4;
        }else if(text[i]=='7' && text[i+1]=='0'){
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+2+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("jmp $0x%s\n", FindHex(numberD));
            i += 10;
        }else if(text[i]=='7' && text[i+1]=='1'){
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+2+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("jle $0x%s\n", FindHex(numberD));
            i += 10;
        }else if(text[i]=='7' && text[i+1]=='2'){
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+2+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("jl $0x%s\n", FindHex(numberD));
            i += 10;
        }else if(text[i]=='7' && text[i+1]=='3'){
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+2+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            
            printf("je $0x%s\n", FindHex(numberD));
            i += 10;
        }else if(text[i]=='7' && text[i+1]=='4'){
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+2+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("jne $0x%s\n", FindHex(numberD));
            i += 10;
        }else if(text[i]=='7' && text[i+1]=='5'){
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+2+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("jge $0x%s\n", FindHex(numberD));
            i += 10;
        }else if(text[i]=='7' && text[i+1]=='6'){
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+2+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("jg $0x%s\n", FindHex(numberD));
            i += 10;
        }else if((text[i]=='A' || text[i]=='a') && text[i+1]=='0'){
            char * reg1 = getRegister(text[i+2]);
            printf("pushl %s\n", reg1);
            free(reg1);
            i += 4;
        }else if((text[i]=='B' || text[i]=='b') && text[i+1]=='0'){
            char * reg1 = getRegister(text[i+2]);
            printf("popl %s\n", reg1);
            free(reg1);
            i += 4;
        }else if(text[i]=='9' && text[i+1]=='0'){
            printf("ret\n");
            i += 2;
        }else if(text[i]=='8' && text[i+1]=='0'){
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+2+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("call $0x%s\n", FindHex(numberD));
            i += 10;
        }else if((text[i]=='D' || text[i]=='d') && text[i+1]=='0'){
            char * Reg = getRegister(text[i+2]);
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+4+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("writeb %d(%s)\n", (int)strtol(numberD, NULL, 16), Reg);
            i += 12;
        }else if((text[i]=='D' || text[i]=='d') && text[i+1]=='1'){
            char * Reg = getRegister(text[i+2]);
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+4+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("writel %d(%s)\n", (int)strtol(numberD, NULL, 16), Reg);
            i += 12;
        }else if((text[i]=='C' || text[i]=='c') && text[i+1]=='0'){
            char * Reg = getRegister(text[i+2]);
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+4+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("readb %d(%s)\n", (int)strtol(numberD, NULL, 16), Reg);
            i += 12;
        }else if((text[i]=='C' || text[i]=='c') && text[i+1]=='1'){
            char * Reg = getRegister(text[i+2]);
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+4+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            
            printf("readl %d(%s)\n", (int)strtol(numberD, NULL, 16), Reg);
            i += 12;
        }else if((text[i]=='E' || text[i]=='e') && text[i+1]=='0'){
            char * reg1 = getRegister(text[i+2]);
            char * reg2 = getRegister(text[i+3]);
            char numberD[9];
            numberD[sizeof(numberD) - 1] = '\0';
            int j;
            for(j=0;j<8;j++){
                numberD[j] = text[i+4+j];
            }
            char te1 = numberD[strlen(numberD)-1];
            char te2 = numberD[strlen(numberD)-2];
            char te3 = numberD[strlen(numberD)-3];
            char te4 = numberD[strlen(numberD)-4];
            numberD[strlen(numberD)-1] = numberD[1];
            numberD[strlen(numberD)-2] = numberD[0];
            numberD[strlen(numberD)-3] = numberD[3];
            numberD[strlen(numberD)-4] = numberD[2];
            numberD[1] = te1;
            numberD[0] = te2;
            numberD[3] = te3;
            numberD[2]= te4;
            printf("movsbl %d(%s) %s\n", (int)strtol(numberD, NULL, 16), reg2,reg1);
            i += 12;
        }
    }
}

void analyizeInstructions(char * line){
    int i =0;
    int j = 0;
    bool found = false;
    for(;i<strlen(line);i++){
        if(isdigit(line[i]) && !found){
            j = i;
            found = true;
        }else if(found && (line[i] == '	' || line[i] == ' ')){
            break;
        }
    }
    char address[i-j+1];
    address[sizeof(address)-1] = '\0';
    int g = 0;
    while(j<i){
        address[g] = line[j];
        j++;
        g++;
    }
    found = false;
    for(;i<strlen(line);i++){
        if(isdigit(line[i]) && !found){
            j = i;
            found = true;
        }else if(found && (line[i] == '	' || line[i] == ' ')){
            break;
        }
    }
    char text[i-j+1];
    text[sizeof(text)-1] = '\0';
    g = 0;
    while(j<i){
        text[g] = line[j];
        j++;
        g++;
    }
    readI(address, text);
    
}

int main(int argc, const char * argv[]) {
    if(argc > 1 && strcmp(argv[1], "-h") == 0){
        printf("please input: \n");
        printf("./y86dis (your y86 file with.y86)\n");
        printf("so you can compile your y86 file\n");
        return 0;
    }
    if(argc != 2){
        printf("ERROR: This program only takes 1 argument, a .y86 instruction file.\n");
    }
    char *buffer;
    FILE *fh = fopen(argv[1], "rb");
    if ( fh != NULL ){
        fseek(fh, 0L, SEEK_END);
        long s = ftell(fh);
        rewind(fh);
        buffer = malloc(s);
        if ( buffer != NULL ){
            fread(buffer, s, 1, fh);
            fclose(fh); fh = NULL;
            int i = 0;
            int j = 0;
            for(;i<strlen(buffer);i++){
                if(buffer[i] == '\n'){
                    char line[(i-j)+1];
                    line[sizeof(line)-1] = '\0';
                    int g = 0;
                    while(j<i){
                        line[g] = buffer[j];
                        j++;
                        g++;
                    }
                    if(line[1] == 't' && line[2] == 'e' && line[3] == 'x' && line[4] == 't'){
                        analyizeInstructions(line);
                        break;
                    }
                    j++;
                }
            }
            free(buffer);
        }
        if (fh != NULL) fclose(fh);
    }
    return 0;
    
}
