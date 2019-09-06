#include "mop.h"

int main(int argc, char *argv[]) {
    if(!initialise()) {
        fprintf(stderr, "Error initialising global variables\n");
        exit(EXIT_FAILURE);
    }
    if(argc == 1) {
        print_usage();
        exit(EXIT_FAILURE);
    }
    if(!parse_cmd(argc, argv)) {
        fprintf(stderr, "Error parsing command line arguments.\n");
        exit(EXIT_FAILURE);
    }
    print_config();
    if(!operation_main()) {
        fprintf(stderr, "Unable to perform matrix operation.\n");
        exit(EXIT_FAILURE);
    }
    printf(" ... Program complete.\n");
    exit(EXIT_SUCCESS);
}
