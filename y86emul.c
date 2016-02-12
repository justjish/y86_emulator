#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "decodeInstr.h"
#include "executeInstr.h"

#define eax 0
#define ecx 1
#define edx 2
#define ebx 3
#define esp 4
#define ebp 5
#define esi 6
#define edi 7
#define OF 0
#define ZF 1
#define SF 2
#define testMode 0
#define testModeRead 0


/* Input the instructions as the first argument at run time
 * try "prog1.y86" or "prog2.y86"

int main(int argc, const char * argv[])
{
    /* Sets Variables */
    FILE *file;
    char directive[80], stringNum[10000], bit1, bit2;
    int registers[8], pc, size, index, value, count, flags[2];
    unsigned char* mem;


    /*Used to Find Current Directory for Xcode*/
    /*char pwd[512];
    if (getcwd(pwd, sizeof pwd) != -1) {
        printf("In directory %s\n", pwd);
    }*/

    /* Opens File */
    file = fopen(argv[1], "r");

    /* Does error check for file not there */
    if (file == NULL){
        fprintf(stderr, "File not found\n");
        return 1;
    }

    /*Gets Size Directive*/
    getDirective(file, directive);

    /*Error if Size Directive is not there*/
    if (strcmp("size",directive) != 0) {
        fprintf(stderr, "Error '.size' not the first directive in file\n");
        return 1;
    }

    /*Sets the size of the memory based on the given size*/
    size = getSize(file);
    mem = malloc((size+1) * sizeof(unsigned char));


    /*Fetch + Decode Part of Code: This part is small and streamedlined using functions.
     Thus there is no need to seperate it out to another function*/
    while (feof(file) == 0) {
        getDirective(file, directive);
        if (strcmp("text", directive) == 0){
            pc = getSize(file);
            addTextToMemory(file, mem, pc);

        } else if (strcmp("byte", directive) == 0) {
            /*Complete*/
            index = getSize(file);
            value = getInt(file);
            mem[index] = value;
            /*printf("The Byte is: %.02x\n",mem[index]);*/

        } else if (strcmp("long", directive) == 0){
            index = getSize(file);
            value = getIntDec(file);
            sprintf(stringNum, "%.08x", value);
            count = 0;

            while (stringNum[count] != '\0') {
                bit1 = stringNum[count];
                count++;
                bit2 = stringNum[count];
                value = convertHexToInt(bit1);
                value = value * 16;
                value = value + convertHexToInt(bit2);
                mem[index] = value;
                index++;
                count++;
            }

        } else if (strcmp("string", directive) == 0){
            index = getInt(file);
            getString(file, stringNum);
            count = 0;
            while (stringNum[count] != '\0') {
                mem[index] = stringNum[count];
                count++;
                index++;

            }

        }

    }
    /*nt arrayLoop=0;
    for(arrayLoop = 256; arrayLoop<=435; arrayLoop++) {
        printf("%i. %.02x\n", arrayLoop, mem[arrayLoop]);

    }*/


    /* This part executes instructions*/
    while (mem[pc] != 0x10) {
        if (testMode) {
            printf("PC: %i @ OP: %.02x\n\n", pc, mem[pc]);
        }
        switch(mem[pc]) {
            case 0x00:
                pc++;
                break;
            case 0x20:
                pc = rrmovl(mem, pc, registers);
                break;
            case 0x30:
                pc = irmovl(mem, pc, registers);
                break;
            case 0x40:
                pc = rmmovl(mem, pc, registers);
                break;
            case 0x50:
                pc = mrmovl(mem, pc, registers);
                break;

            /*Uses the switch function to funnel all of these cases into one function*/
            case 0x60:
            case 0x61:
            case 0x62:
            case 0x63:
            case 0x64:
                pc = op1(mem, pc, registers, flags);
                break;
            case 0x70:
                pc = jmp(mem, pc, registers);
                break;
            case 0x71:
                pc = jle(mem, pc, registers, flags);
                break;
            case 0x72:
                pc = jl(mem, pc, registers, flags);
                break;
            case 0x73:
                pc = je(mem, pc, registers, flags);
                break;
            case 0x74:
                pc = jne(mem, pc, registers, flags);
                break;
            case 0x75:
                pc = jge(mem, pc, registers, flags);
                break;
            case 0x76:
                pc = jg(mem, pc, registers, flags);
                break;
            case 0x80:
                pc = call(mem, pc, registers);
                break;
            case 0x90:
                pc = ret(mem, pc, registers);
                break;
            case 0xa0:
                pc = push(mem, pc, registers);
                break;
            case 0xb0:
                pc = pop(mem, pc, registers);
                break;
            case 0xc0:
                pc = readb(mem, pc, registers, flags);
                break;
            case 0xc1:
                pc = readw(mem, pc, registers, flags);
                break;
            case 0xd0:
                pc = writeb(mem, pc, registers);
                break;
            case 0xd1:
                pc = writew(mem, pc, registers);
                break;
        }
    }
    if (mem[pc] == 0x10){
        return 1;
    }

    return 0;

}
