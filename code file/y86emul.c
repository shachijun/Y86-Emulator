//
//  y86emul.c
//  Assign 4!
//
//  Created by 沙驰钧 on 2016/12/4.
//  Copyright © 2016年 沙驰钧. All rights reserved.
//

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include "y86emul.h"
#include "convert.c"
#include <math.h>
#include <stdlib.h>

int Register[8];
int memoryCount;
unsigned char * memory;
int memorySize;
int OF, ZF, SF;
ProgramStatus status = AOK;


int findInt(char*file,int index,int number){
    if((file[index]=='\0')||(file[index]=='\n')){
        return number;
    }
    int num=single_digit_hex_to_dec(file[index]);
    if (num==-1) {
        number=-1;
    }else{
        if (number==-1) {
            number=num;
        }else{
            number=number*16+num;
        }
    }
    return findInt(file,(index+1),number);
}


int getsize(FILE *fh,int size){
    if ((int)ftell(fh)==size) {
        return -1;
    }
    char buf[size];
    fgets(buf, size, fh);
    if (buf[3]=='z'){
        return findInt(buf,5,-1);
    }
    return getsize(fh,size);
}


char *openfile(FILE *fh){
    char *buffer=NULL;
    fseek(fh, 0L, SEEK_END);
    long s = ftell(fh);
    rewind(fh);
    buffer = malloc(s);
    if ( buffer != NULL ){
        fread(buffer, s, 1, fh);
    }
    return buffer;
}


int main(int argc, char ** argv){
    if (argc <= 1){
        printf("Error: Not enough statement!\n");
        return 0;
    }
    char help[] = "-h\0";
    if (strcmp((argv[1]), help) == 0){
        printf("please input: \n");
        printf("./y86emul (your y86 file with.y86)\n");
        printf("so you can compile your y86 file\n");
        return 0;
    }
    int m = 0;
    int n = 0;
    FILE *fh=fopen(argv[1],"r");
    if ( fh == NULL ){
        printf("Error, no such file");
        return 0;
    }
    char * read=openfile(fh);
    char * CopyReading = copy(read);
    char * findTxt;
    fseek(fh, 0L, SEEK_END);
    int filesize=(int)ftell(fh);
    fseek(fh, 0L, SEEK_SET);
    int size=getsize(fh, filesize);
    memorySize = size;
    memory = (unsigned char *) malloc((size + 1) * sizeof(unsigned char));
    memoryCount = -1;
    for (m = 0; m < size; m++)
    {//memory initial
        memory[m] = '\0';
    }
    free(CopyReading);
    CopyReading = copy(read);
    findTxt = strtok(CopyReading, "\n\t\r");
    char * getText;
    char * ADD;
    int addIndex = 0;
    do{
        if (strcmp(findTxt, ".size") == 0){
            findTxt = strtok(NULL, "\n\t\r");
        }else if (strcmp(findTxt, ".text") == 0){
            ADD = copy(strtok(NULL, "\n\t\r"));
            getText = copy(strtok(NULL, "\n\t\r"));
            addIndex = hextodec(ADD);
            if (memoryCount == -1){
                memoryCount = addIndex;
            }else if (memoryCount != -1){
                printf("ERROR: Too much .text instruction\n");
                return 0;
            }
            n = 0;
            while (n < strlen(getText)){
                memory[addIndex] = (unsigned char) FindAByte(getText, n);
                addIndex++;
                n += 2;
            }
            free(ADD);
            free(getText);
        }else if (strcmp(findTxt, ".byte") == 0){
            ADD = copy(strtok(NULL, "\n\t\r"));
            getText = copy(strtok(NULL, "\n\t\r"));
            memory[hextodec(ADD)] = (unsigned char) hextodec(getText);
            free(ADD);
            free(getText);
        }else if (strcmp(findTxt, ".long") == 0){
            ADD = copy(strtok(NULL, "\n\t\r"));
            getText = copy(strtok(NULL, "\n\t\r"));
            union CharToInt ChangeInt;
            ChangeInt.ConInt = atoi(getText);
            for(m = 0; m < 4; m++){
                memory[m+hextodec(ADD)] = ChangeInt.Byt[m];
            }
            free(ADD);
            free(getText);
        }else if (strcmp(findTxt, ".string") == 0){
            ADD = copy(strtok(NULL, "\n\t\r"));
            getText = copy(strtok(NULL, "\n\t\r"));
            int length = strlen(getText);
            m = hextodec(ADD);
            for(n = 1; n < length - 1; n++){
                memory[m] = (unsigned char)getText[n];
                m++;
            }
            free(getText);
            free(ADD);
        }else if(strcmp(findTxt, ".bss") == 0){
            //	No such
        }else if (findTxt[0] == '.'){
            status = INS;
        }
        findTxt = strtok(NULL, "\n\t");
    } while (findTxt != NULL);
    free(CopyReading);
    if (status == INS){
        printf("ERROR: Invalid directive encountered: %s\n", findTxt);
        return 0;
    }
    Execute();
    CheckStatus();
    free(memory);
    free(read);
    return 0;
}

void Execute(){
    unsigned char arg1;
    unsigned char arg2;
    int number;
    int number1, number2;
    union CharToInt ChangeInt;
    status = AOK;
    int wrong;
    char inputB;
    int inputW;
    Register[7] =0, Register[6] =0, Register[5] =0, Register[4] =0, Register[3] =0, Register[2] =0, Register[1] =0, Register[0] = 0;
    OF = 0,ZF =0, SF = 0;
    while (status == AOK){
//        switch (memory[memoryCount]){
            if(memory[memoryCount]== 0x00){
                memoryCount++;
            }else if(memory[memoryCount]== 0x10){
                status = HLT;
                
            }else if(memory[memoryCount]== 0x20){
                getargs(&arg1, &arg2);
                Register[arg2] = Register[arg1];
                memoryCount += 2;
                
            }else if(memory[memoryCount]== 0x30){
                getargs(&arg1, &arg2);
                if (arg1 < 0x08){
                    status = ADR;
                    printf("ERROR: just need one register for irmovl");
                    break;
                }
                ChangeInt.Byt[0] = memory[memoryCount + 2];
                ChangeInt.Byt[1] = memory[memoryCount + 3];
                ChangeInt.Byt[2] = memory[memoryCount + 4];
                ChangeInt.Byt[3] = memory[memoryCount + 5];
                number = ChangeInt.ConInt;
                Register[arg2] = number;
                memoryCount += 6;
                
            }else if(memory[memoryCount]== 0x40){
                getargs(&arg1, &arg2);
                ChangeInt.Byt[0] = memory[memoryCount + 2];
                ChangeInt.Byt[1] = memory[memoryCount + 3];
                ChangeInt.Byt[2] = memory[memoryCount + 4];
                ChangeInt.Byt[3] = memory[memoryCount + 5];
                number = ChangeInt.ConInt;
                ChangeInt.ConInt = Register[arg1];
                if ((number + Register[arg2] + 3) > memorySize){
                    status = ADR;
                    printf("ERROR: address exceed the memory");
                }
                memory[number + Register[arg2] + 0] = ChangeInt.Byt[0];
                memory[number + Register[arg2] + 1] = ChangeInt.Byt[1];
                memory[number + Register[arg2] + 2] = ChangeInt.Byt[2];
                memory[number + Register[arg2] + 3] = ChangeInt.Byt[3];
                memoryCount += 6;
                
            }else if(memory[memoryCount]== 0x50){
                getargs(&arg1, &arg2);
                ChangeInt.Byt[0] = memory[memoryCount + 2];
                ChangeInt.Byt[1] = memory[memoryCount + 3];
                ChangeInt.Byt[2] = memory[memoryCount + 4];
                ChangeInt.Byt[3] = memory[memoryCount + 5];
                number = ChangeInt.ConInt;
                if ((number + Register[arg2] + 3) > memorySize){
                    status = ADR;
                    printf("ERROR: address exceed the memory");
                }
                ChangeInt.Byt[0] = memory[number + Register[arg2] + 0];
                ChangeInt.Byt[1] = memory[number + Register[arg2] + 1];
                ChangeInt.Byt[2] = memory[number + Register[arg2] + 2];
                ChangeInt.Byt[3] = memory[number + Register[arg2] + 3];
                Register[arg1] = ChangeInt.ConInt;
                memoryCount += 6;
                
            }else if(memory[memoryCount]== 0x60){
                ZF = 0;
                SF = 0;
                OF = 0;
                getargs(&arg1, &arg2);
                number1 = Register[arg1];
                number2 = Register[arg2];
                number = number1 + number2;
                if (number == 0){
                    ZF = 1;
                }
                if (number < 0){
                    SF = 1;
                }
                if ((number > 0 && number1 < 0 && number2 < 0) || (number < 0 && number1 > 0 && number2 > 0)){
                    OF = 1;
                }
                Register[arg2] = number;
                memoryCount += 2;
                
            }else if(memory[memoryCount]== 0x61){
                ZF = 0;
                SF = 0;
                OF = 0;
                getargs(&arg1, &arg2);
                number1 = Register[arg1];
                number2 = Register[arg2];
                number = number2 - number1;
                if (number == 0){
                    ZF = 1;
                }
                if (number < 0){
                    SF = 1;
                }
                if ((number > 0 && number1 > 0 && number2 < 0) || (number < 0 && number1 < 0 && number2 > 0)){
                    OF = 1;
                }
                Register[arg2] = number;
                memoryCount += 2;
                
            }else if(memory[memoryCount]== 0x62){
                SF = 0;
                ZF = 0;
                getargs(&arg1, &arg2);
                number1 = Register[arg1];
                number2 = Register[arg2];
                number = number1 & number2;
                Register[arg2] = number;
                if (number == 0){
                    ZF = 1;
                }
                if (number < 0){
                    SF = 1;
                }
                memoryCount += 2;
                
            }else if(memory[memoryCount]== 0x63){
                ZF = 0;
                SF = 0;
                getargs(&arg1, &arg2);
                number1 = Register[arg1];
                number2 = Register[arg2];
                number = number1 ^ number2;
                Register[arg2] = number;
                if (number == 0){
                    ZF = 1;
                }
                if (number < 0){
                    SF = 1;
                }
                memoryCount += 2;
                
            }else if(memory[memoryCount]== 0x64){
                ZF = 0;
                SF = 0;
                OF = 0;
                getargs(&arg1, &arg2);
                number1 = Register[arg1];
                number2 = Register[arg2];
                number = number1 * number2;
                if (number == 0){
                    ZF = 1;
                }
                if (number < 0){
                    SF = 1;
                }
                if ((number < 0 && number1 < 0 && number2 < 0) ||
                    (number < 0 && number1 > 0 && number2 > 0) ||
                    (number > 0 && number1 < 0 && number2 > 0) ||
                    (number > 0 && number1 > 0 && number2 < 0)){
                    OF = 1;
                }
                Register[arg2] = number;
                memoryCount += 2;
                
            }else if(memory[memoryCount]== 0x65){
                ZF = 0;
                SF = 0;
                OF = 0;
                getargs(&arg1, &arg2);
                number1 = Register[arg1];
                number2 = Register[arg2];
                number = number2 - number1;
                if (number == 0){
                    ZF = 1;
                }
                if (number < 0){
                    SF = 1;
                }
                if ((number > 0 && number1 > 0 && number2 < 0) || (number < 0 && number1 < 0 && number2 > 0)){
                    OF = 1;
                }
                memoryCount += 2;
                
            }else if(memory[memoryCount]== 0x70){
                ChangeInt.Byt[0] = memory[memoryCount + 1];
                ChangeInt.Byt[1] = memory[memoryCount + 2];
                ChangeInt.Byt[2] = memory[memoryCount + 3];
                ChangeInt.Byt[3] = memory[memoryCount + 4];
                number = ChangeInt.ConInt;
                memoryCount = number;
                
            }else if(memory[memoryCount]== 0x71){
                ChangeInt.Byt[0] = memory[memoryCount + 1];
                ChangeInt.Byt[1] = memory[memoryCount + 2];
                ChangeInt.Byt[2] = memory[memoryCount + 3];
                ChangeInt.Byt[3] = memory[memoryCount + 4];
                number = ChangeInt.ConInt;
                if (ZF == 1 || (SF ^ OF)){
                    memoryCount = number;
                }else{
                    memoryCount += 5;
                }
              
            }else if(memory[memoryCount]== 0x72){
                ChangeInt.Byt[0] = memory[memoryCount + 1];
                ChangeInt.Byt[1] = memory[memoryCount + 2];
                ChangeInt.Byt[2] = memory[memoryCount + 3];
                ChangeInt.Byt[3] = memory[memoryCount + 4];
                number = ChangeInt.ConInt;
                if (ZF == 0 && (SF ^ OF)){
                    memoryCount = number;
                }else{
                    memoryCount += 5;
                }
                
            }else if(memory[memoryCount]== 0x73){
                ChangeInt.Byt[0] = memory[memoryCount + 1];
                ChangeInt.Byt[1] = memory[memoryCount + 2];
                ChangeInt.Byt[2] = memory[memoryCount + 3];
                ChangeInt.Byt[3] = memory[memoryCount + 4];
                number = ChangeInt.ConInt;
                if (ZF == 1){
                    memoryCount = number;
                }else{
                    memoryCount += 5;
                }
                
            }else if(memory[memoryCount]== 0x74){
                ChangeInt.Byt[0] = memory[memoryCount + 1];
                ChangeInt.Byt[1] = memory[memoryCount + 2];
                ChangeInt.Byt[2] = memory[memoryCount + 3];
                ChangeInt.Byt[3] = memory[memoryCount + 4];
                number = ChangeInt.ConInt;
                if (ZF == 0){
                    memoryCount = number;
                }else{
                    memoryCount += 5;
                }
                
            }else if(memory[memoryCount]== 0x75){
                ChangeInt.Byt[0] = memory[memoryCount + 1];
                ChangeInt.Byt[1] = memory[memoryCount + 2];
                ChangeInt.Byt[2] = memory[memoryCount + 3];
                ChangeInt.Byt[3] = memory[memoryCount + 4];
                number = ChangeInt.ConInt;
                if (!(ZF == 0 && (SF ^ OF))){
                    memoryCount = number;
                }else{
                    memoryCount += 5;
                }
                
            }else if(memory[memoryCount]== 0x76){
                ChangeInt.Byt[0] = memory[memoryCount + 1];
                ChangeInt.Byt[1] = memory[memoryCount + 2];
                ChangeInt.Byt[2] = memory[memoryCount + 3];
                ChangeInt.Byt[3] = memory[memoryCount + 4];
                number = ChangeInt.ConInt;
                if (!(ZF == 1 || (SF ^ OF))){
                    memoryCount = number;
                }else{
                    memoryCount += 5;
                }
                
            }else if(memory[memoryCount]== 0x80){
                ChangeInt.Byt[0] = memory[memoryCount + 1];			
                ChangeInt.Byt[1] = memory[memoryCount + 2];			
                ChangeInt.Byt[2] = memory[memoryCount + 3];			
                ChangeInt.Byt[3] = memory[memoryCount + 4];			
                number = ChangeInt.ConInt;
                Register[4] -= 4;
                ChangeInt.ConInt = memoryCount + 5;
                memory[Register[4] + 0] = ChangeInt.Byt[0];
                memory[Register[4] + 1] = ChangeInt.Byt[1];	
                memory[Register[4] + 2] = ChangeInt.Byt[2];	
                memory[Register[4] + 3] = ChangeInt.Byt[3];	
                memoryCount = number;
                
            }else if(memory[memoryCount]== 0x90){
                ChangeInt.Byt[0] = memory[Register[4] + 0];
                ChangeInt.Byt[1] = memory[Register[4] + 1];	
                ChangeInt.Byt[2] = memory[Register[4] + 2];	
                ChangeInt.Byt[3] = memory[Register[4] + 3]; 
                memoryCount = ChangeInt.ConInt;
                Register[4] += 4;
                
            }else if(memory[memoryCount]== 0xA0){
                getargs(&arg1, &arg2);
                Register[4] -= 4;
                ChangeInt.ConInt = Register[arg1];
                memory[Register[4] + 0] = ChangeInt.Byt[0];
                memory[Register[4] + 1] = ChangeInt.Byt[1];	
                memory[Register[4] + 2] = ChangeInt.Byt[2];	
                memory[Register[4] + 3] = ChangeInt.Byt[3];	
                memoryCount += 2;
                
            }else if(memory[memoryCount]== 0xB0){
                getargs(&arg1, &arg2);
                ChangeInt.Byt[0] = memory[Register[4] + 0];
                ChangeInt.Byt[1] = memory[Register[4] + 1];
                ChangeInt.Byt[2] = memory[Register[4] + 2];
                ChangeInt.Byt[3] = memory[Register[4] + 3];
                number = ChangeInt.ConInt;
                Register[arg1] = number;
                Register[4] += 4;
                memoryCount += 2;
                
            }else if(memory[memoryCount]== 0xC0){
                ZF = 0;
                getargs(&arg1, &arg2);
                ChangeInt.Byt[0] = memory[memoryCount + 2];
                ChangeInt.Byt[1] = memory[memoryCount + 3];			
                ChangeInt.Byt[2] = memory[memoryCount + 4];			
                ChangeInt.Byt[3] = memory[memoryCount + 5];			
                number = ChangeInt.ConInt;
                if (1 > scanf("%c", &inputB)){
                    ZF = 1;
                }
                memory[Register[arg1] + number] = inputB;
                memoryCount += 6;
                
            }else if(memory[memoryCount]== 0xC1){
                ZF = 0;
                getargs(&arg1, &arg2);
                ChangeInt.Byt[0] = memory[memoryCount + 2];
                ChangeInt.Byt[1] = memory[memoryCount + 3];			
                ChangeInt.Byt[2] = memory[memoryCount + 4];			
                ChangeInt.Byt[3] = memory[memoryCount + 5];
                number = ChangeInt.ConInt;
                wrong = scanf("%d", &inputW);
                if (wrong < 1){
                    ZF = 1;
                }
                ChangeInt.ConInt = inputW;
                memory[Register[arg1]+ number + 0] = ChangeInt.Byt[0];	
                memory[Register[arg1]+ number + 1] = ChangeInt.Byt[1];	
                memory[Register[arg1]+ number + 2] = ChangeInt.Byt[2];	
                memory[Register[arg1]+ number + 3] = ChangeInt.Byt[3];	
                memoryCount += 6;
                
            }else if(memory[memoryCount]== 0xD0){
                getargs(&arg1, &arg2);
                ChangeInt.Byt[0] = memory[memoryCount + 2];
                ChangeInt.Byt[1] = memory[memoryCount + 3];			
                ChangeInt.Byt[2] = memory[memoryCount + 4];			
                ChangeInt.Byt[3] = memory[memoryCount + 5];			
                number = ChangeInt.ConInt;
                printf("%c", (char)memory[Register[arg1] + number]);
                memoryCount += 6;
                
            }else if(memory[memoryCount]== 0xD1){
                getargs(&arg1, &arg2);
                ChangeInt.Byt[0] = memory[memoryCount + 2];
                ChangeInt.Byt[1] = memory[memoryCount + 3];			
                ChangeInt.Byt[2] = memory[memoryCount + 4];			
                ChangeInt.Byt[3] = memory[memoryCount + 5];			
                number = ChangeInt.ConInt;
                ChangeInt.Byt[0] = memory[number + Register[arg1] + 0];
                ChangeInt.Byt[1] = memory[number + Register[arg1] + 1];			
                ChangeInt.Byt[2] = memory[number + Register[arg1] + 2];			
                ChangeInt.Byt[3] = memory[number + Register[arg1] + 3];			
                number1 = ChangeInt.ConInt;
                printf("%d", number1);
                memoryCount += 6;
                
            }else if(memory[memoryCount]== 0xE0){
                getargs(&arg1, &arg2);
                ChangeInt.Byt[0] = memory[memoryCount + 2];
                ChangeInt.Byt[1] = memory[memoryCount + 3];			
                ChangeInt.Byt[2] = memory[memoryCount + 4];			
                ChangeInt.Byt[3] = memory[memoryCount + 5];			
                number = ChangeInt.ConInt;
                ChangeInt.ConInt = Register[arg2];
                inputB = ChangeInt.Byt[3];
                if ((inputB >> 7 & 1) == 0){
                    ChangeInt.Byt[0] = inputB;
                    ChangeInt.Byt[1] = 0x00;
                    ChangeInt.Byt[2] = 0x00;
                    ChangeInt.Byt[3] = 0x00;
                }else{
                    ChangeInt.Byt[0] = inputB;
                    ChangeInt.Byt[1] = 0xff;
                    ChangeInt.Byt[2] = 0xff;
                    ChangeInt.Byt[3] = 0xff;
                }
                ChangeInt.Byt[0] = memory[Register[arg2]+ number + 0];	
                ChangeInt.Byt[1] = memory[Register[arg2]+ number + 1];	
                ChangeInt.Byt[2] = memory[Register[arg2]+ number + 2];	
                ChangeInt.Byt[3] = memory[Register[arg2]+ number + 3];	
                Register[arg1] = ChangeInt.ConInt;
                memoryCount += 6;
            }else{
                status = INS;
                
        }
        number = 0;
        arg1 = arg2 = 0;
    }
}


char * copy (char * str){
    char * ret = (char *) malloc((strlen(str) + 1) * sizeof(char));
    strcpy(ret, str);
    return ret;
}


int FindAByte(char * str, int position){
    char * twobytes = (char *) malloc(3*sizeof(char));
    twobytes[2] = '\0';
    twobytes[0] = str[position];
    twobytes[1] = str[position + 1];
    int ret = hextodec(twobytes);
    free(twobytes);
    return ret;
}


void CheckStatus (){
    printf("\nStatus:");
    switch (status)
    {
        case AOK:
            printf("AOK, works but do not have return or stop sign\n");
            break;
            
        case INS:
            printf("INS, invalid instruction appears\n");
            break;
            
        case ADR:
            printf("ADR, invalid address in the instruction\n");
            break;
            
        case HLT:
            printf("HLT, return and stop sign found\n");
            break;
    }
}



void getargs(unsigned char * arg1, unsigned char * arg2){
    *arg1 = (memory[memoryCount + 1] & 0xf0) >> 4;
    *arg2 = (memory[memoryCount + 1] & 0x0f);
}



