#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int extractIPv4(const char* str, unsigned long* outAddress, int* outPort);

int main(void) {

    const char* str;
    unsigned long* outAddress;
    int* outPort;
    int validAddress;

    char input[1024]; //inputs larger need to be split.

    while(1) {

        printf("Enter a string (or 'END' to quit): ");
        scanf("%1023[^\n]%*c", input);
        if (input[0] == 'E' && input[1] == 'N' && input[2] == 'D' && input[3] == '\0') {
            printf("Program terminated.\n");
            break;
        }
    
        str = input;
        
        validAddress = extractIPv4(str, outAddress, outPort);

        if (validAddress == 0) {
            //Address is not valid
            printf("Invalid input: no valid IPv4 address found\n");
        }  else {
            //is a valid IPv4 address
            printf("Extracted IPv4 address: ");
        }

    }

    return 0;
}


int extractIPv4(const char* str, unsigned long* outAddress, int* outPort) {
    int validAddress = 0; //returns 1 if valid address, 0 else
    char temp[256];
    int tempIndex = 0;

    //printf("from str: %s\n", str);
    
    //Goal: only copy numerical & digit values to temp array (ignore spaces and letters):
    for (int i = 0; str[i] != '\0'; i++) {
            //printf("%c\n", str[i]);
            if (str[i] == '.' || str[i] == ':' || str[i] == '0' || str[i] == '1'|| str[i] == '2'|| str[i] == '3'|| str[i] == '4'|| str[i] == '5'|| str[i] == '6'|| str[i] == '8'|| str[i] == '9') {
                temp[tempIndex] = str[i];
                tempIndex++;
            }
    }

    printf("Items added to the temp array: ");
    for (int i = 0; i < tempIndex; i++) {
        printf("%c", temp[i]);
    }
    printf("\n");

    return validAddress;
}