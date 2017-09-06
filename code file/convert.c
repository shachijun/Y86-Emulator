//
//  convert.c
//  Assign 4!
//
//  Created by 沙驰钧 on 2016/12/4.
//  Copyright © 2016年 沙驰钧. All rights reserved.
//

#include "convert.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int single_digit_hex_to_dec(char hex_dig) {
    switch (hex_dig) {
        case '0':
            return 0;
            break;
        case '1':
            return 1;
            break;
            
        case '2':
            return 2;
            break;
            
        case '3':
            return 3;
            break;
            
        case '4':
            return 4;
            break;
            
        case '5':
            return 5;
            break;
            
        case '6':
            return 6;
            break;
            
        case '7':
            return 7;
            break;
            
        case '8':
            return 8;
            break;
            
        case '9':
            return 9;
            break;
        case 'A':
        case 'a':
            return 10;
            break;
        case 'B':
        case 'b':
            return 11;
            break;
        case 'C':
        case 'c':
            return 12;
            break;
        case 'D':
        case 'd':
            return 13;
            break;
        case 'E':
        case 'e':
            return 14;
            break;
        case 'F':
        case 'f':
            return 15;
            break;
        default:
            return -1;
            break;
    }
}

int hextodec(char * num)
{
	// First convert hex string to binary string
	int size = strlen(num);
	char * binstr = (char *) malloc((4*size + 1)*sizeof(char));
	int i;
	for (i = 0; i < 4*size + 1; i++)
	{
		binstr[i] = '\0';
	}
	for (i = 0; i < size; i++)
	{
		strcat(binstr, hextobin(num[i]));
	}
	// Converts from binary to decimal
	int ret = bintodec(binstr);
	free(binstr);
	return ret;
}
	

char * hextobin(char c) 
{
	switch(c)
	{
		case '0':
		return "0000";
		break;
		
		case '1':
		return "0001";
		break;
		
		case '2':
		return "0010";
		break;
		
		case '3':
		return "0011";
		break;
		
		case '4':
		return "0100";
		break;
		
		case '5':
		return "0101";
		break;
		
		case '6':
		return "0110";
		break;
		
		case '7':
		return "0111";
		break;
		
		case '8':
		return "1000";
		break;
		
		case '9':
		return "1001";
		break;
		
		case 'a':
		case 'A':
		return "1010";
		break;
		
		case 'b':
		case 'B':
		return "1011";
		break;
		
		case 'c':
		case 'C':
		return "1100";
		break;
		
		case 'd':
		case 'D':
		return "1101";
		break;
		
		case 'e':
		case 'E':
		return "1110";
		break;
		
		case 'f':
		case 'F':
		return "1111";
		break;
		
		case '\0':
		break;
		
		default:
		printf("Invalid hex character: %c \n", c);
		break;
	}
	return "";
}
	


int bintodec(char * num)
{
	int power = strlen(num) - 1;
	int i, ret = 0;
	for (i = 0; num[i] != '\0'; i++)
	{
		int temp = num[i] - '0';
		ret += temp * (int)pow(2, power);
		power--;
	}
	return ret;
}





char * twosComplementCreation(char * data){
    char sum[strlen(data) + 1];
    char flipedData[strlen(data)];
    int i;
    for(i=0; i<sizeof(flipedData); i++){
        if(data[i] == '1'){
            flipedData[i] = '0';
        }else{
            flipedData[i] = '1';
        }
    }
    
    
    
    char one[strlen(data)];
    
    for(i = 0; i < sizeof(one) - 1; i++){
        one[i] = '0';
    }
    one[sizeof(one) - 1] = '1';
    bool carry = false;
    
    for(i = sizeof(one) - 1; i >= 0; i--){
        if(one[i] == '1' && flipedData[i] == '1' && carry){
            sum[i] = '1';
        }
        else if((one[i] == '1' && flipedData[i] == '1') || (one[i] == '1' && carry) || (flipedData[i] == '1' && carry)){   // imagine that it is checking if there is a carry in the if statement
            sum[i] = '0';
            carry = true;
        }
        else if(one[i] == '1' || flipedData[i] == '1' || carry){
            carry = false;
            sum[i] = '1';
        }else{
            sum[i] = '0';
        }
    }
    
    sum[sizeof(sum) - 1] = '\0';
    char * copy = malloc(sizeof(sum) + 1);
    strcpy(copy, sum);
    return copy;
}

long hexToInt(char * input, bool couldBetwosComplement){
    char bianary[16][4] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
    char number[(4*strlen(input)) + 1];
    number[sizeof(number)-1] = '\0';
    int i;
    int j = 0;
    for(i=0; i<strlen(input);i++){
        int pos;
        switch(toupper(input[i])){
            case 'F':
                pos = 15;
                break;
            case 'E':
                pos = 14;
                break;
            case 'D':
                pos = 13;
                break;
            case 'C':
                pos = 12;
                break;
            case 'B':
                pos = 11;
                break;
            case 'A':
                pos = 10;
                break;
            case '9':
                pos = 9;
                break;
            case '8':
                pos = 8;
                break;
            case '7':
                pos = 7;
                break;
            case '6':
                pos = 6;
                break;
            case '5':
                pos = 5;
                break;
            case '4':
                pos = 4;
                break;
            case '3':
                pos = 3;
                break;
            case '2':
                pos = 2;
                break;
            case '1':
                pos = 1;
                break;
            case '0':
                pos = 0;
                break;
            default :
                printf("ERROR: Invalad '%c' in '%s'\n", input[i], input);
                return -1;
        }
        number[j] = bianary[pos][0];
        number[j + 1] = bianary[pos][1];
        number[j + 2] = bianary[pos][2];
        number[j + 3] = bianary[pos][3];
        j += 4;
    }
    char * num = number;
    
    if(couldBetwosComplement){
        if(number[0] == '1'){
            num = twosComplementCreation(number);
            int result = -1 * bintodec(num);
            free(num);
            return result;
        }
    }
    return bintodec(num);
}




























