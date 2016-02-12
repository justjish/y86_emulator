#ifndef y86emul_decodeInstr_h
#define y86emul_decodeInstr_h

/* Complete*/
void getDirective (FILE * fp, char* directive) {
    char bit;
    int index = 0;
    
    /*Gets first bit*/
    bit = fgetc(fp);
    
    /*If bit is the . then it gets next bit*/
    if (bit =='.'){
        bit = fgetc(fp);
        
    }
    
    while (isalpha(bit)){
        directive[index] = bit;
        bit = fgetc(fp);
        index++;
    }
    directive[index] = '\0';
}



void getString (FILE * fp, char* string) {
    char bit;
    int index = 0;
    
    /*Gets first bit*/
    bit = fgetc(fp);
    
    /*Skips first "*/
    if (bit =='"'){
        bit = fgetc(fp);
    }
    
    while (bit != '"'){
        string[index] = bit;
        bit = fgetc(fp);
        index++;
    }
    string[index] = '\0';
}

void addTextToMemory(FILE * fp, unsigned char * mem, int start) {
    char bit;
    int value;
    bit = fgetc(fp);
    
    while (isalnum(bit)){
        value = convertHexToInt(bit);
        value = value * 16;
        bit = fgetc(fp);
        value = value + convertHexToInt(bit);
        mem[start] = value;
        bit = fgetc(fp);
        start++;
    }
}

int convertHexToInt(int hexNum) {
    char hex = (char)hexNum;
    if (hex == 'a' || hex == 'A'){
        return 10;
    } else if (hex == 'b' || hex == 'B'){
        return 11;
    } else if (hex == 'c' || hex == 'C'){
        return 12;
    } else if (hex == 'd' || hex == 'D'){
        return 13;
    } else if (hex == 'e' || hex == 'E'){
        return 14;
    } else if (hex == 'f' || hex == 'F'){
        return 15;
    } else if (hex == '0') {
        return 0;
    } else {
        return (int)(hex - '0');
    }
}

int getSize(FILE* fp) {
    char bit, hex[10];
    int number, index = 0;
    bit = fgetc(fp);
    
    while (isxdigit(bit)) {
        hex[index] = bit;
        bit = fgetc(fp);
        index++;
    }
    hex[index] = '\0';
    
    number = (int)strtol(hex, NULL, 16);
    
    return number;
}

int getInt(FILE* fp) {
    char bit;
    int value;
    
    bit = fgetc(fp);
    
    while (isxdigit(bit)) {
        value = convertHexToInt(bit);
        value = value * 16;
        bit = fgetc(fp);
        value = value + convertHexToInt(bit);
        bit = fgetc(fp);
    }
    
    return value;
}
int getIntDec(FILE* fp) {
    char bit;
    char dec[10];
    int number;
    int index = 0;
    
    bit = fgetc(fp);
    
    while (isxdigit(bit)) {
        dec[index] = bit;
        bit = fgetc(fp);
        index++;
    }
    dec[index] = '\0';
    
    number = (int)strtol(dec, NULL, 16);
    return number;
}


#endif
