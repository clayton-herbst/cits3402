#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <sys/time.h>

#define NUM_OPERATIONS 5
#define NUM_OPTIONS 7
#define MAX_MTX_DIMENSIONS 10
#define BUFFER_SIZE 250
#define STUDENT_NUMBER "22245091"
#define NUMBER_OF_INPUT_FILES 2
#define DEFAULT_THREAD_COUNT 4

// -- STRUCTURES --
typedef struct {
    struct timeval start;
    struct timeval end;
    float delta; //change in time
} TIME_STAT;

typedef struct {
    int  operation;
    char *op_str;
    float sm; //scalar number
    int  num_threads;
    int num_files;
    FILE **fd;
    char **filename;
    FILE *log_fd;
    char *log_filename;
    TIME_STAT *time;
    struct tm *exec_time;
    time_t rawtime;
    bool sync;
} CONFIG;

typedef struct {
    bool is_int;
    int **mtxi;
    float **mtxf;
    int row;
    int col;
    long int size;
} COO;

typedef struct {
    bool is_int;
    int *mtxi; //non-zero values
    float *mtxf; //non-zero values
    int *mtx_offset;
    int *mtx_col;
    int size;
    int row;
    int col;
} CSR;

typedef struct {
    bool is_int;
    int *mtxi; //non-zero values
    float *mtxf; //non-zero values
    int *mtx_offset;
    int *mtx_row;
    int size;
    int row;
    int col;
} CSC;

//GLOBAL VARIABLES
CONFIG  config;
char    *arg_options[NUM_OPTIONS];
int     (*op_func[NUM_OPERATIONS])(); //pointer to operation functions


// -- FUNCTION DECLARATIONS --
extern int main(int, char**);
extern int parse_cmd(int, char**);
extern int get_operation(char*);
extern bool config_is_setup(void);
extern int set_input_files(int, char*);
extern int set_logger(void);
extern int set_config_threads(char*);
extern int set_config_sm(char*);

// -- HELPER FUNCTIONS --
extern void print_config(void);
extern int initialise(void);
extern char *op_to_string(void);
extern void print_usage(void);
extern char *str_clean(char*);
extern int get_non_zero_csc(CSC*, bool**, int, int);
extern int process_stat(void);

// -- OPERATIONS --
extern int operation_main(void);
extern int scalar(void);
extern int trace(void);
extern int addition(void);
extern int transpose_matrix(void);
extern int matrix_mp(void);

// -- FILE READING --
extern int read_to_coo(COO **, int, int);
extern int read_to_csr(CSR **, int, int);
extern int read_to_csc(CSC **, int, int);
extern int read_coo_filei(COO**,int, int);
extern int read_coo_filef(COO**, int, int);
extern int add_int_coo(COO*,int,int,int,long int);
extern int add_float_coo(COO*,float,int,int,long int);
extern int coo2csc(COO*, CSC*);
extern int coo2csr(COO*, CSR*);


// -- SYNCHRONOUS --
extern int process_scalar(COO*, float);
extern int process_trace(COO*,int*,float*);
extern int process_addition(CSR**, COO**);
extern int process_transpose(CSR*, CSC*);
extern int process_multiplication(COO*, CSR*, CSC*);

// -- ASYNCHRONOUS --
extern int process_scalar_async(COO*, float);
extern int process_trace_async(COO*,int*,float*);
extern int process_addition_async(CSR**, COO**);
extern int process_transpose_async(CSR*, CSC*);
extern int process_multiplication_async(COO*, CSR*, CSC*);


// -- LOGGER --
extern int log_coo_result(COO*, FILE*);
extern int log_trace_result(COO*, int*, float*, FILE*);
extern int log_csr_result(CSR *, FILE *);
extern int log_csc_ts_result(CSC *, FILE*);

// -- DEALLOC --
extern void dealloc_coo(COO**,int);
extern void dealloc_config(void);
extern void dealloc_csr(CSR**, int);
extern void dealloc_csc(CSC**, int);
