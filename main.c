#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*All code was made by me, and lines with an AI comment were adjusted due to feedback from AI.
  Any code blocks with mainly AI produced code (checked by me) are clearly labeled as such.
  Please see the AIDisclosure.txt file for prompt information.*/

int extractIPv4(const char* str, unsigned long* outAddress, int* outPort);
int toInt(char c);

int main(void) {
    const char* str;           //Will be used to store the user's input
    unsigned long outAddress; //AI recommended change (remove '*')
    int outPort;             //AI recommended change (remove '*')
    int validAddress; //used to check whether a valid address was returned from extractIPv4 (1 if valid, 0 if not)
    char input[1024]; //inputs larger need to be split.

    while(1) {

        printf("Enter a string (or 'END' to quit): ");
        int result = scanf("%1023[^\n]%*c", input);     //AI recommended adding the int result to store the value of the scanf (see following if statement)

        /*The following if statement was recommended by AI.
        By checking the return value of the scanf call, we
        can detect and safely handle a null string entry: */
        if (result != 1) {
            getchar();
            continue;
        }

        /**Checking whether or not the user wants to continue or END: */
        if (input[0] == 'E' && input[1] == 'N' && input[2] == 'D' && input[3] == '\0') {
            printf("Program terminated.\n");
            break;
        }
        
        //Storing the user's input into str
        str = input;
        
        //Variable tracks whether a valid address was returned (1), or not (0)
        validAddress = extractIPv4(str, &outAddress, &outPort);  //AI suggested change (pass pointer addresses in memory)

        /*AI code begins:*/
        //AI suggested building each octet out this way, so I didn't need the extra string that I built for the IPv4 address.
        /*Because I'm returning my octets in a single 32-bit value, this process allows me to extract them individually:*/
        int a = (outAddress >> 24) & 255; //Shift 24 bits to the right, and bitwise AND (extract only that byte)
        int b = (outAddress >> 16) & 255; //Shift 16 bits to the right, and bitwise AND (extract only that byte)
        int c = (outAddress >> 8) & 255;  //Shift 8 bits to the right, and bitwise AND (extract only that byte)
        int d = outAddress & 255;        //No shifts needed, and bitwise AND (extract only that byte)
        /*AI code ends*/

        if (validAddress == 0) {
            //Address is not valid
            printf("Invalid input: no valid IPv4 address found\n");
        }  else {
            //is a valid IPv4 address
            if (outPort == -1) {
                //No Port present, print with "none":
                printf("Extracted IPv4 address: %d.%d.%d.%d (decimal value: %ld, port: none)\n", a, b, c, d, outAddress);
            } else {
                //There is a port, print out the value:
                printf("Extracted IPv4 address: %d.%d.%d.%d (decimal value: %ld, port: %d)\n", a, b, c, d, outAddress, outPort);
            }
            
        }

    }

    return 0;
}


int extractIPv4(const char* str, unsigned long* outAddress, int* outPort) {
    int validAddress = 0; //returns 1 if valid address, 0 else
    int periodCount = 0; //all entries need exactly 3 periods
    int colonCount = 0; //all entries need exactly 0 or 1 colons
    int portValue = 0; //used to track the port value for later.
    int portDigitCount = 0; //used to track how many digits are present in the port number
    char firstPortDigit = '\0'; //used to remember the first port digit
    char temp[256]; //temporary character array to build the IPv4 address
    int tempIndex = 0; //temporary Index for the temp character array
    
    //Goal: only copy numerical & digit values to temp array (ignore spaces and letters):
    for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] == '.' || str[i] == ':' || str[i] == '0' || str[i] == '1'|| str[i] == '2'|| str[i] == '3'|| str[i] == '4'|| str[i] == '5'|| str[i] == '6'|| str[i] == '7' || str[i] == '8'|| str[i] == '9') { //AI update: added missing str[i] == '7'
                if (tempIndex < 255) { //AI suggested this check so I don't write beyond the array.
                    temp[tempIndex] = str[i]; //Safe to copy character into the temp array
                    tempIndex++;  //incrementing the index for the temp array
                }
            } else {//Means we encountered a garbage character, and need to validate the token here.
                /*The following if statement block was recommended by AI, and checked by me.*/
                if (tempIndex > 0) {
                    temp[tempIndex] = '\0'; //Add null terminator to convert to a string (doesn't copy garbage character over)
                }
                //Clearing the tempIndex for the next canidate string
                tempIndex = 0;
                
            }
    }

    if (tempIndex > 0) {  //AI suggested change, add null terminator to the string (so long as it is not null)
        temp[tempIndex] = '\0'; //Creates a canidate token that can then be validated.
        //The below commented line can be uncommented--very helpful for testing!
        //printf("Canidate token (after loop) is: %s\n", temp);
    }

    //Count Periods in the temp string (need exactly 3 to be valid):
    for (int i = 0; temp[i] != '\0'; i++) {
        if (temp[i] == '.') {
            periodCount++; //increment for each period encountered.
        }
    }

    //printf("Period count is %d\n", periodCount); //useful for testing

    /*Since a valid IPv4 address has exactly 3 periods, we can return with an invalid address if we don't have exactly 3:*/
    if (periodCount != 3) {
        validAddress = 0;
        *outAddress = 0;
        *outPort = -1;
        return validAddress;
    }

    //Count Colons in the temp string (need exactly 1 or 0 to be valid):
    for (int i = 0; temp[i] != '\0'; i++) {
        if(temp[i] == ':') {
            colonCount++; //increment the number of colons found
        }
    }

    /*if we have more than one colon, we know this is not a valid IPv4 address:*/
    if (colonCount > 1) {
        validAddress = 0;
        *outAddress = 0;
        *outPort = -1;
        return validAddress;
    }

    /**AI code to help with positioning of the colon */
    int colonPosition = -1;  //Initializing to -1, and we will increment through to find the location of the colon:
    for (int i = 0; temp[i] != '\0'; i++) {
        if (temp[i] == ':') {
            colonPosition = i; //record the position of the colon.
            break;
        }
    }

    /*Since we have established that there is a colon, we can reasonably presume at this point that there may be a valid port number (we don't want it set to -1)--will update later with actual port value*/
    portValue = 0;

    //If the colon has a position in the temporary character array:
    if (colonPosition != -1) {

        temp[colonPosition] = '\0'; //Places null terminator at the colon position, to separate the IP address from the port number

        //Loop through from just past the colon position to the end, to check the port number
        for (int i = colonPosition + 1; temp[i] != '\0'; i++) {

            /*If the element being checked is less than 0 or greater than 9, it isn't a valid digit*/
            if (temp[i] < '0' || temp[i] > '9') {
                validAddress = 0;
                *outAddress = 0;
                *outPort = -1;
                return validAddress; //Return, not a valid IP address
            }

            //Converting the char into an int value and appending it to the number being built
            portValue = portValue * 10 + (temp[i] - '0');

            /*We only want to accept port digits [0-65535], so checking to ensure we aren't exceeding the upper bound here:*/
            if (portValue > 65535) {
                validAddress = 0;
                *outAddress = 0;
                *outPort = -1;
                return validAddress;
            }

            if (portDigitCount == 0) { //If there weren't any previous port digits accounted for...
                firstPortDigit = temp[i]; //...then create the first port digit.
            }
            portDigitCount++; //increment the port digit count
            *outPort = portValue; //update the port value
        }

        if (portDigitCount > 1 && firstPortDigit == '0') { //If there is a port number detected AND the first digit is 0...
            validAddress = 0;
            *outAddress = 0;
            *outPort = -1;
            return validAddress; //...return as invalid (we don't want anny leading zeros!)
        }
        if (portDigitCount == 0) { //Means there were no digits detected for the port number, so it doesn't exist.
            validAddress = 0;
            *outAddress = 0;
            *outPort = -1;
            return validAddress;
        }
    } else {
        *outPort = -1; //There is no valid port number for this IPv4 address
    }
    
    /**End of AI code */

    /*Next, need to check each octet (AI code begin)*/
    int octets[4]; //Used to build each of the IPv4 octets to check them
    int octetIndex = 0; //...and the index to track them.

    int value = 0;       //used to track the value of each octet
    int digitCount = 0;  //used to count the number of digits (cannot have more than 3 in an octet)
    char firstDigit = '\0'; //Used to track the first digit.
 
    for (int i = 0; ; i++) { //Keep looping through until we return or break:

        /* End of octet */
        if (temp[i] == '.' || temp[i] == '\0') {

            /* Empty octet, no digits found between ".", such as: 1..2.3.4 */
            if (digitCount == 0) {
                validAddress = 0;
                *outAddress = 0;
                *outPort = -1;
                return validAddress;
            }

            /* Leading zero check */
            if (digitCount > 1 && firstDigit == '0') { //there is a digit, and the first digit is zero
                validAddress = 0;
                *outAddress = 0;
                *outPort = -1;
                return validAddress; //Invalid, cannot have leading zeros!
            }

            /* Range check: value for each octet needs to be within [0, 255]: */
            if (value > 255) { //if greater than 255...
                validAddress = 0;
                *outAddress = 0;
                *outPort = -1;
                return validAddress; //...invalid!
            }

            octets[octetIndex] = value; //Store the current value into the octet array to save it
            octetIndex++; //increment the index

            /* Reset for next octet */
            value = 0;
            digitCount = 0;
            firstDigit = '\0';

            /*If we ever reach a null terminator, break.  This stops us from continuing on, as we've processed the final octet.*/
            if (temp[i] == '\0') {
                break;
            }
        }

        else {

            /* Must be a digit */
            if (temp[i] < '0' || temp[i] > '9') { //This checks if we encounter something that falls outside of [0,9] (not a digit)
                validAddress = 0;
                *outAddress = 0;
                *outPort = -1;
                return validAddress; //Return as an invalid address
            }

            //First digit of this octet:
            if (digitCount == 0) {
                firstDigit = temp[i];
            }

            //Converting the char into an int value and appending it to the number being built
            value = value * 10 + (temp[i] - '0');
            digitCount++; //Increment, there may be additional digits to be recorded.
        }
    }

    //We must have 4 octets to be valid address:
    if (octetIndex == 4) {
        //Valid IPv4 candidate was found!
        validAddress = 1;

        /*Here, we are converting the octets that were previously found into a 32 bit decimal value to be passed back to main().
        Within main(), we will convert these back into the IPv4 format before printing.  Each octet is shifted into its proper 
        byte position and combined with a bitwise OR. The bitwise OR allows me to preserve all the previous octets while adding in the new ones.*/
        *outAddress =
        ((unsigned long)octets[0] << 24) | //Shift 24 bits to the left, and bitwise OR (store that bite in the first/leftmost slot)
        ((unsigned long)octets[1] << 16) | //Shift 16 bits to the left, and bitwise OR (store that byte in the second (from the left) slot)
        ((unsigned long)octets[2] << 8)  | //Shift 8 bits to the left, and bitwise OR (store that byte in the third (from the left) slot)
        ((unsigned long)octets[3]);        //No shifts necessary, stored in the rightmost slot (fourth octet)

        return validAddress;
    }
    /*AI code end*/
}