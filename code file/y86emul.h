//
//  y86emul.h
//  Assign 4!
//
//  Created by 沙驰钧 on 2016/12/4.
//  Copyright © 2016年 沙驰钧. All rights reserved.
//

#ifndef Y86EMUL_H_
#define Y86EMUL_H_
#include <math.h>
typedef enum {
	AOK, /*Everything is fine; no errors*/
	HLT, /*Halt instruction has occurred; program ends normally*/
	INS,
    ADR,
	} ProgramStatus;
union CharToInt {
	char Byt[4];
	int ConInt;
};
void Execute();
char * copy (char * str);
int FindAByte(char * str, int position);
void CheckStatus ();
void getargs(unsigned char * arg1, unsigned char * arg2);
#endif 
