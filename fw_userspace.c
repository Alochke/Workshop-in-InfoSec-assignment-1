#include <string.h> // For strcmp.
#include <fcntl.h>  // For openning the sysfs attributes file.
#include <unistd.h> // For writing to and reading from the sysfs attributes file.
#include <stdio.h>  // For printing.


#define ATTRIBUTE_PATH "/sys/class/fw_class/fw_class_fw_device/sysfs_att"
#define CORRECT_INPUT "0" // Only correct non-empty input.
#define INPUT_ERR "Wrong input, you've entered.\n" // The error message in case of wrong input.
#define OPEN_ERR "As a sudo, you didn't run the program, or the module, you didn't install.\n" // The error message in case the program wasn't run with the right permissions or the module wasn't installed before running the program.
#define MAX_ARGC 2 // Maximal correct value of argc.
#define EQ 0 // The value strcmp returns when the compared strings are equel.
#define SHOW_TRANSFER 2 * sizeof(unsigned int) // The amount of data that should be transferred from the kernel module in case no arguments were given to the program.
#define ERROR -1 // Will be returned in case of an error.
#define SUCCESS 0 // Will be returned in case the program was executed successfuly.
#define PARAM_IDX 1 // The index in argv of the first parameter that was given to the program.
#define MINIMAL_STORE 1 // The minimal amount of bytes we need to write into a sysfs attribute in order for the store function to take action.
#define ERR_CHECK(condition, message)   \
    if(condition){                      \
        fprintf(stderr, message);       \
        return ERROR;                   \
    }                                   \

// The next 4 lines are the output message lines in case no arguments were given to the program.

#define OUT_LINE1 "Firewall Packets Summary: \n"
#define OUT_LINE2 "Number of accepted packets: %u \n", accepted
#define OUT_LINE3 "Number of dropped packets: %u \n", dropped
#define OUT_LINE4 "Total number of packets: %u \n", accepted + dropped

int main(int argc, char* argv[])
{
    int fd;                     // A file descriptor that will be used to interact with the sysfs attribute.
    unsigned int accepted;      // The number of accepted packetes.
    unsigned int dropped;       // The number of dropped packetes.
    char buf[SHOW_TRANSFER];    // Will be used to store the data transfered from the module in case no arguments were given to the program.

    // Checking input.
    ERR_CHECK(argc > MAX_ARGC || ((argc == MAX_ARGC) && (strcmp(CORRECT_INPUT, argv[PARAM_IDX]) != EQ)), INPUT_ERR)

    fd = open(ATTRIBUTE_PATH, O_RDWR);
    ERR_CHECK(fd == ERROR, OPEN_ERR)

    if(argc == MAX_ARGC)
        write(fd, buf, MINIMAL_STORE); // Apparently we have to write something into the attribute in order for the show function to take action.
    else{
        int transfered = 0;
        while(SHOW_TRANSFER != transfered)
        {
            transfered += read(fd, buf + transfered, SHOW_TRANSFER - transfered);
        }
        accepted = (unsigned int) *buf;
        dropped = (unsigned int) *(buf + sizeof(unsigned int));
        printf(OUT_LINE1);
        printf(OUT_LINE2);
        printf(OUT_LINE3);
        printf(OUT_LINE4);
    }

    close(fd);
    return SUCCESS;
}