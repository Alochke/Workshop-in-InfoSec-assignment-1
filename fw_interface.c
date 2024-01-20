#include <string.h> // For strcmp.
#include <fcntl.h>  // For openning the sysfs attributes file.
#include <unistd.h> // For writing to and reading from the sysfs attributes file.
#include <stdio.h>  // For printing.


#define ATTRIBUTE_PATH "/sys/class/fw_class/fw_class_fw_device/sysfs_att"
#define CORRECT_INPUT "0"                                       // Only correct non-empty input.
#define INPUT_ERR "Wrong input, you've entered.\n"              // The error message in case of wrong input.
#define PERMISSION_ERR "As a sudo, you must run the program.\n" // The error message in case the program wasn't run with the right permissions.
#define MAX_INPUTS 2                                            // Maximal number of parameters to the program, including program name
#define EQ 0                                                    // The value strcmp returns when the compared strings are equel.
#define SHOW_TRANSFER 2 * sizeof(unsigned int)                  // The amount data transfered from the kernel module to a buffer by reading from ATTRIBUTE_PATH.
#define ERROR -1                                                // Will be returned in case of an error (which could happen only if wrong paarameters were given.)
#define SUCCES 1                                                // Will be returned in case the program was executed successfuly.
#define CLI_PARAM_IDX 1                                         // The index of the only parameter, given to the command line when one was given in argv.
#define MINIMAL_TRANSFER 1                                      // The minimal amount of bytes it's guaranteed that read will transfer.

int main( int argc, char* argv[] )
{
    int fd;                     // A file descriptor that will be used to interact with the sysfs attribute.
    unsigned int accepted;      // The number of accepted packetes.
    unsigned int dropped;       // The number of dropped packetes.
    char buf[SHOW_TRANSFER];    // Will be used to store the data transfered from the module in case no arguments were given to the program.

    // Checking input.
    if(argc > MAX_INPUTS || ((argc == MAX_INPUTS) && (strcmp(CORRECT_INPUT, argv[CLI_PARAM_IDX]) != EQ)))
    {
        printf(INPUT_ERR);
        return ERROR;
    }

    fd = open(ATTRIBUTE_PATH, O_RDWR);
    if(fd == ERROR)
    {
        printf(PERMISSION_ERR);
        return ERROR;
    }

    if(argc == MAX_INPUTS)
        write(fd, NULL, 0);
    else{
        int i;
        for(i = 0; i < SHOW_TRANSFER; i++)
            read(fd, buf + i, MINIMAL_TRANSFER);
        accepted = (unsigned int) *buf;
        dropped = (unsigned int) *(buf + sizeof(unsigned int));
        printf("Firewall Packets Summary:\n");
        printf("Number of accepted packets: %d\n", accepted);
        printf("Number of dropped packets: %d \n", dropped);
        printf("Total number of packets: %d \n", accepted + dropped);
    }

    close(fd);
}