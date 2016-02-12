
#ifndef y86emul_executeInstr_h
#define y86emul_executeInstr_h

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

/*Write memory to console*/
int writew  (unsigned char* mem, int pc, int* reg) {
    char bit[12], bit2[12], bit3[12];
    int rA, rB, val, D, i;
    pc++;
    val = mem[pc];
    sprintf(bit, "%.02x", val);
    rA = bit[0] - '0';
    pc++;
    sprintf(bit2, "0x%.02x%.02x%.02x%.02x", mem[pc+3], mem[pc+2], mem[pc+1], mem[pc+0]);
    D = (int)strtol(bit2, NULL, 16);
    rB = reg[rA] + D;
    sprintf(bit3, "0x%.02x%.02x%.02x%.02x", mem[rB+3], mem[rB+2], mem[rB+1], mem[rB+0]);
    i = (int)strtol(bit3, NULL, 16);
    printf("%i\n", i);
    return pc+4;
    
}

/* Reads a 32 bit dec from console to memory*/
int readw (unsigned char* mem, int pc, int* reg, int* flags){
    char value[20];
    char text[4], bit[12], bit2[12], bit3[18];
    int rA, D, val, i, count, hex, count2;
    scanf("%s", value);
    if (text[0] == '\0') {
        flags[ZF] = 1;
    } else {
        flags[ZF] = 0;
    }
    if (testModeRead) {
        printf("This was value given %s\n", value);
    }
    pc++;
    val = mem[pc];
    sprintf(bit, "%.02x", val);
    rA = bit[0] - '0';
    pc++;
    sprintf(bit2, "0x%.02x%.02x%.02x%.02x", mem[pc+3], mem[pc+2], mem[pc+1], mem[pc]);
    D = (int)strtol(bit2, NULL, 16);
    if (testModeRead){
        printf("The displacement: %i\n", D);
    }
    i = (int)strtol(value, NULL, 10);
    
    if (testModeRead){
        printf("The int Value converted to Int: %i\n",i);
    }
    
    /*(printf("THIS IS A THE INT; %i\n", i);*/
    
    sprintf(bit3, "%.08x", i);
    if(testModeRead){
        printf("The given value in hex 0x%.08x\n", i);
    }
    rA = reg[rA];
    count = 0;
    count2 = 0;
    text[2] = '\0';
    while (bit3[count] != '\0') {
        text[0] = bit3[count];
        count++;
        text[1] = bit3[count];
        hex = (int)strtol(text, NULL, 16);
        count++;
        mem[rA + D +3 - count2] = hex;
        count2++;
    }
    
    if (testModeRead) {
        for(i =0; i <4; i++) {
            count = rA + D +3 -i;
            printf("The value in spot %i = 0x%.02x\n", count, mem[count] );
        }
    }
    
    return pc+4;
}

/* Reads one bite from console to memory*/
int readb(unsigned char* mem, int pc, int* reg, int* flags){
    char text[4], bit[12], bit2[12];
    int rA, D, value, val;
    scanf("%s", text);
    if (text[0] == '\0') {
        flags[ZF] = 1;
    } else {
        flags[ZF] = 0;
    }
    pc++;
    value = mem[pc];
    sprintf(bit, "%.02x", value);
    rA = bit[0] - '0';
    pc++;
    sprintf(bit2, "0x%.02x%.02x%.02x%.02x", mem[pc+3], mem[pc+2], mem[pc+1], mem[pc+0]);
    D = (int)strtol(bit2, NULL, 16);
    val = (int)strtol(text, NULL, 10);
    if (testMode) {
        printf("THIS IS A THE INT; %i\n", val);
    }
    mem[D+reg[rA]] = val;
    return pc+4;
}

/*Moves a 32 bit value from register to memory*/
int rmmovl (unsigned char* mem, int pc, int* reg) {
    char bit[12], bit2[12];
    int rA, rB, D, value;
    pc++;
    value = mem[pc];
    sprintf(bit, "%.02x", value);
    rA = bit[0] - '0';
    rB = bit[1] - '0';
    pc++;
    sprintf(bit2, "0x%.02x%.02x%.02x%.02x", mem[pc+3], mem[pc+2], mem[pc+1], mem[pc+0]);
    D = (int)strtol(bit2, NULL, 16);
    if (testMode) {
        printf("Memory at reg[rA]: %i\n", reg[rA]);
        printf("This is the displacement: %i\n", D);
        printf("This is the rB: %i\n", reg[rB]);
    }
    mem[reg[rB]+D] = reg[rA];
    if (testMode) {
        printf("The final memory at spot mem[%i] = %i\n", reg[rB]+D, mem[reg[rB]+D]);
        
        printf("The returning mem[pc] is: 0x%.02x\n", mem[pc+4]);
    }
    return pc+4;
}

/*Move memory  32 bit value from memory to register*/
int mrmovl (unsigned char* mem, int pc, int* reg) {
    char bit[12], bit2[12];
    int rA, rB, D, value;
    pc++;
    value = mem[pc];
    sprintf(bit, "%.02x", value);
    rA = bit[0] - '0';
    rB = bit[1] - '0';
    pc++;
    sprintf(bit2, "0x%.02x%.02x%.02x%.02x", mem[pc+3], mem[pc+2], mem[pc+1], mem[pc+0]);
    D = (int)strtol(bit2, NULL, 16);
    
    if (testMode) {
        printf("This is the rB mrmovl: %i\n", reg[rB]);
        printf("This is the displacement for mrmovl: %i\n", D);
    }
    reg[rA] = mem[D+reg[rB]];
    if (testMode) {
        printf("This what is in rB after movl: %i\n", reg[rB]);
        printf("The returning mem[pc] is: 0x%.02x\n", mem[pc+4]);
    }
    
    return pc+4;
}
/*Pushes the program counter onto stack and returns the next program counter given*/
int call(unsigned char* mem, int pc, int* reg){
    char bit2[12], car;
    int location, rB, value, count;
    
    if (testMode) {
        printf("Calls's OpCode: %.02x\n", mem[pc]);
        printf("Calls's PC: %.02x\n", pc);
    }
    
    rB = reg[esp];
    reg[esp] = reg[esp] - 4;
    
    sprintf(bit2, "%.08x", pc+5);
    if (testMode) {
        printf("The PC that will be put on top of the stack %i\n", pc+5);
        printf("The PC in string(%s) = hex(%.08x)", bit2, pc+5);
    }
    
    count = 0;
    car = bit2[count];
    value = 0;
    while (isxdigit(car)){
        value = convertHexToInt(car);
        value = value * 16;
        count = count+1;
        car = bit2[count];
        value = value + convertHexToInt(car);
        mem[rB] = value;
        count++;
        car = bit2[count];
        rB--;
        value = 0;
    }
    
    pc++;
    sprintf(bit2, "0x%.02x%.02x%.02x%.02x", mem[pc+3], mem[pc+2], mem[pc+1], mem[pc]);
    if (testMode) {
        printf("The PC for after Return(int): %i\n", pc+4);
    }
    location = (int)strtol(bit2, NULL, 16);
    if (testMode) {
        printf("The new location (The place in memory it is going to next): %i\n", location);
        printf("The returning mem[(location)] is: 0x%.02x\n", mem[location]);
    }
    return location;
}
/*The following returns to last program counter before the call*/
int ret(unsigned char* mem, int pc, int* reg){
    int rB;
    char bit2[14];
    reg[esp] = reg[esp]+4;
    rB = reg[esp];
    
    sprintf(bit2, "0x%.02x%.02x%.02x%.02x", mem[rB], mem[rB-1], mem[rB-2], mem[rB-3]);
    pc = (int)strtol(bit2, NULL, 16);
    
    return pc;
}



/* The following is All the Jump Operations*/
int jle (unsigned char* mem, int pc, int* reg, int* flags) {
    if ((flags[SF] ^ flags[OF])| flags[ZF]){
        return jmp(mem, pc, reg);
    } else {
        return pc + 5;
    }
}
int jl (unsigned char* mem, int pc, int* reg, int* flags) {
    if (flags[SF] ^ flags[OF]){
        return jmp(mem, pc, reg);
    } else {
        return pc + 5;
    }
}

int je (unsigned char* mem, int pc, int* reg, int* flags) {
    if (flags[ZF] == 1){
        return jmp(mem, pc, reg);
    } else {
        return pc + 5;
    }
}

int jne (unsigned char* mem, int pc, int* reg, int* flags) {
    if (flags[ZF] == 0){
        return jmp(mem, pc, reg);
    } else {
        return pc + 5;
    }
}
int jge (unsigned char* mem, int pc, int* reg, int* flags) {
    if (!(flags[SF] ^ flags[OF])){
        return jmp(mem, pc, reg);
    } else {
        return pc + 5;
    }
}
int jg (unsigned char* mem, int pc, int* reg, int* flags) {
    if (!(flags[SF] ^ flags[OF]) & !flags[ZF]){
        return jmp(mem, pc, reg);
    } else {
        return pc + 5;
    }
}

/*Copies value from first register to next register*/
int rrmovl(unsigned char* mem, int pc, int* reg) {
    char bit[12];
    int reg1, reg2, value;
    pc++;
    value = mem[pc];
    sprintf(bit, "%.02x", value);
    reg1 = bit[0] - '0';
    reg2 = bit[1] - '0';
    reg[reg2] = reg[reg1];
    pc++;
    if (testMode) {
        printf("What is in rA(%i): %i\n ",reg1, reg[reg1]);
        printf("What is in rB(%i): %i\n ",reg2, reg[reg2]);
        printf("The returning mem[pc] is: 0x%.02x\n", mem[pc]);
    }
    
    return pc;
    
}


/*Pushes memory onto stack*/
int push(unsigned char* mem, int pc, int* reg) {
    int rA, rB, value, count;
    char bit[3], car;
    char bit2[12];
    rB = reg[esp];
    reg[esp] = reg[esp] - 4;
    
    pc++;
    value = mem[pc];
    sprintf(bit, "%.02x", value);
    rA = bit[0] - '0';
    if (testMode) {
        printf("The reg[%i] to be inputed to top of stack\n", rA);
    }
    
    sprintf(bit2, "%.08x", reg[rA]);
    if (testMode) {
        printf("Number to be put on top of the stack %s\n", bit2);
    }
    
    count = 0;
    car = bit2[count];
    value = 0;
    while (isxdigit(car)){
        value = convertHexToInt(car);
        value = value * 16;
        count = count+1;
        car = bit2[count];
        value = value + convertHexToInt(car);
        mem[rB] = value;
        count++;
        car = bit2[count];
        rB--;
        value = 0;
    }
    pc++;
    if (testMode) {
        for (count = 0; count <=16; count++){
            printf("Memory @ spot %i = 0x%.02x\n",(6144-count),mem[6144-count]);
        }
        
        printf("The returning mem[pc] is: 0x%.02x\n", mem[pc]);
    }
    return pc;
}
/* Pops the memory from stack and stores it into a register*/
int pop(unsigned char* mem, int pc, int* reg) {
    int rA, rB, value;
    char bit[3];
    char bit2[14];
    reg[esp] = reg[esp]+4;
    rB = reg[esp];
    pc++;
    value = mem[pc];
    sprintf(bit, "%.02x", value);
    rA = bit[0] - '0';
    sprintf(bit2, "0x%.02x%.02x%.02x%.02x", mem[rB], mem[rB-1], mem[rB-2], mem[rB-3]);
    reg[rA] = (int)strtol(bit2, NULL, 16);
    pc++;
    if (testMode) {
        printf("The popped value in hex 0x%.08x\n", reg[rA]);
        printf("The next program counter 0x%.08x\n", mem[pc]);
    }
    return pc;
    
}
/*Moves a 32-Bit value into given register*/
int irmovl(unsigned char* mem, int pc, int* reg) {
    char bit[12], bit2[12];
    int value, reg2, newLoc;
    pc++;
    value = mem[pc];
    sprintf(bit, "%.02x", value);
    reg2 = bit[1] - '0';
    pc++;
    sprintf(bit2, "0x%.02x%.02x%.02x%.02x", mem[pc+3], mem[pc+2], mem[pc+1], mem[pc+0]);
    newLoc = (int)strtol(bit2, NULL, 16);
    if (testMode) {
        printf("The value about to be moved to rB(register[%i]): %i\n", reg2 ,newLoc);
    }
    
    reg[reg2] = newLoc;
    return pc+4;
    
}
/* Chance PC based on the given 4 Byte Call Address*/
int jmp(unsigned char* mem, int pc, int* reg){
    char bit2[12];
    pc++;
    sprintf(bit2, "%.02x%.02x%.02x%.02x", mem[pc+3], mem[pc+2], mem[pc+1], mem[pc+0]);
    pc = (int)strtol(bit2, NULL, 16);
    return pc;
}

/*Writes one byte to console*/
int writeb(unsigned char* mem, int pc, int* reg) {
    char bit[12], bit2[12];
    int rA, D, value;
    pc++;
    value = mem[pc];
    sprintf(bit, "%.02x", value);
    rA = bit[0] - '0';
    rA = reg[rA];
    pc++;
    sprintf(bit2, "0x%.02x%.02x%.02x%.02x", mem[pc+3], mem[pc+2], mem[pc+1], mem[pc+0]);
    D = (int)strtol(bit2, NULL, 16);
    printf("%c",(int)mem[rA+D]);
    return pc+4;
    
}

/* Does all the 6x Operations*/
int op1(unsigned char* mem, int pc, int* reg, int* flags) {
    char bit[12], opCode[12];
    int reg1, reg2, value, operand, sum, difference, product;
    value = mem[pc];
    sprintf(opCode, "%.02x", value);
    operand = opCode[1] - '0';
    /*printf("The Operator is: %i\n", operand);*/
    pc++;
    value = mem[pc];
    sprintf(bit, "%.02x", value);
    reg1 = bit[0] - '0';
    reg2 = bit[1] - '0';
    
    if (testMode) {
        printf("Int @ rA (reg[%i]): %i\n",reg1, reg[reg1]);
        printf("Int @ rB (reg[%i]): %i\n",reg2, reg[reg2]);
    }
    
    
    flags[OF] = 0;
    flags[ZF] = 0;
    flags[SF] = 0;
    /*Does the addl operation*/
    if (operand == 0) {
        sum = reg[reg1] + reg[reg2];
        reg[reg2] = sum;
        
        if (testMode) {
            printf("The Number after being added: %i\n", reg[reg2]);
        }
        
        
        /*Overflow Flags*/
        flags[OF] = (unsigned int) reg[reg2] < (unsigned int) reg[reg1];
        /*Zero Flag*/
        if (sum == 0) {
            flags[1] = 1;
        }
        /*Negative Flags*/
        if (sum < 0) {
            flags[2] = 1;
        }
        
        /*Does the subl operation*/
    } else if (operand == 1) {
        difference = reg[reg2] - reg[reg1];
        reg[reg2] = difference;
        if (testMode) {
            printf("The Number after being subtracted: %i\n", reg[reg2]);
        }
        /*Overflow Flags*/
        if ((reg[reg1] < 0 && reg[reg2] > 0 && difference < 0)||
            (reg[reg1] > 0 && reg[reg2] < 0 && difference > 0)) {
            flags[0] = 1;
        }
        
        /*Zero Flag*/
        if (difference == 0) {
            flags[ZF] = 1;
        }
        
        /*Negative Flag*/
        if (difference < 0) {
            flags[2] = 1;
        }
        
        
        /* Does the & operation*/
    } else if (operand == 2){
        sum = reg[reg1]&reg[reg2];
        reg[reg2] = sum;
        
        /*Zero Flag*/
        if (sum == 0) {
            flags[ZF] = 1;
        }
        
        /*Negative Flag*/
        if (sum < 0) {
            flags[SF] = 1;
        }
        
        
        /* Does the ^ operation*/
    } else if (operand == 3) {
        
        sum = reg[reg1] ^ reg[reg2];
        reg[reg2] = sum;
        
        /*Zero Flag*/
        if (sum == 0) {
            flags[ZF] = 1;
        }
        
        /*Negative Flag*/
        if (sum < 0) {
            flags[SF] = 1;
        }
        
    } else if (operand == 4) {
        product = reg[reg1] * reg[reg2];
        reg[reg2] = product;
        
        /*Overflow Flags*/
        if ((reg[reg1] > 0 && reg[reg2] > 0 && product < 0) ||
            (reg[reg1] < 0 && reg[reg2] < 0 && product > 0) ||
            (reg[reg1] < 0 && reg[reg2] > 0 && product > 0) ||
            (reg[reg1] < 0 && reg[reg2] < 0 && product > 0)) {
            flags[0] = 1;
        }
        
        /*Zero Flag*/
        if (product == 0) {
            flags[1] = 1;
        }
        
        /*Negative Flags*/
        if (product < 0) {
            flags[2] = 1;
        }
        
    }
    
    /*printf("Now reg2 = %i after adding reg1 %i\n",reg[reg2], reg[reg1]);*/
    pc++;
    if (testMode) {
        printf("The returning mem[pc] is: 0x%.02x\n", mem[pc]);
    }
    return pc;
}


#endif
