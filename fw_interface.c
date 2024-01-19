#include <string.h> // For strcmp.
#include <stdio.h>  // For reading from and writing to files and for printing.


#define ATTRIBUTE_PATH "/sys/class/fw_class/fw_class_fw_device/sysfs_att"
#define CORRECT_INPUT "0" // Only correct non-empty input.
#define ERROR_MSG "Wrong input, you've entered.\n"
#define MAX_INPUTS 2 // Maximal number of parameters to the program, including program name
#define EQ 0 // The value strcmp returns when the compared strings are equel.
#define SHOW_TRANSFER 2 * sizeof(unsigned int) // The amount data transfered from the kernel module to a buffer by reading from ATTRIBUTE_PATH.
#define ERROR -1 // Will be returned in case of an error (which could happen only if wrong paarameters were given.)
#define SUCCES 1 // Will be returned in case the program was executed successfuly.
#define CLI_PARAM_IDX 1 // The index of the only parameter, given to the command line when one was given in argv.

int main( int argc, char* argv[] )
{
    FILE* fptr;                 // Will be used to interact with the sysfs attribute.
    unsigned int accepted;      // The number of accepted packetes.
    unsigned int dropped;       // The number of dropped packetes.
    char buf[SHOW_TRANSFER];   // Will be used to store the data transfered from the module in case no arguments were given to the program.

    // Checking input.
    if(argc > MAX_INPUTS || ((argc == MAX_INPUTS) && (strcmp(CORRECT_INPUT, argv[CLI_PARAM_IDX]) != EQ)))
    {
        printf(ERROR_MSG);
        return ERROR;
    }

    fptr = fopen(ATTRIBUTE_PATH, "r+");

    // if(argc == MAX_INPUTS)
    //     fputs("", fptr);
    // else{
    //     fgets(buf, SHOW_TRANSFER, fptr);
    //     accepted = (unsigned int) *buf;
    //     dropped = (unsigned int) *(buf + sizeof(unsigned int));
    //     printf("Firewall Packets Summary:\n");
    //     printf("Number of accepted packets: %d\n", accepted);
    //     printf("Number of dropped packets: %d \n", dropped);
    //     printf("Total number of packets: %d \n", accepted + dropped);
    // }

    // fclose(fptr);
}