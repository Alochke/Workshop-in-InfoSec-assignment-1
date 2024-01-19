#include <string.h> // For strcmp

#define SHOW
#define STORE
#define CORRECT_INPUT "0" // Only correct non-empty input.
#define ERROR_MSG "Wrong input, you've entered."
#define MAX_INPUTS 1 // Maximal number of parameters to the program.
#define EQ 0 // The value strcmp returns when the compared strings are equel.
#define SHOW_TRANSFER 2 * sizeof(unsigned int) // The 

int main(char** argv, int argc)
{

    // Checking input.
    if(argc > MAX_INPUTS || ((argc == MAX_INPUTS) && (strcmp(&CORRECT_INPUT, argv[MAX_INPUTS]) != 0)))
    {
        printf("%d", ERROR_MSG);
    }


}