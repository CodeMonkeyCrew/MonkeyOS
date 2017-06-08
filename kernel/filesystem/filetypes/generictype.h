#ifndef KERNEL_FILESYSTEM_FILETYPES_GENERICTYPE_H_
#define KERNEL_FILESYSTEM_FILETYPES_GENERICTYPE_H_

typedef enum {GPIO_DIR, GPIO_VAL, TIMER, TIMER_INT, TIMER_MODE, UART, EXE} file_types_t;
#define NO_OF_FILE_TYPES 16

#define FILE_NAME_SIZE 16

typedef enum { false, true } bool;

typedef struct {
    file_types_t f_type;
    bool is_open;
    int size;
    char name[FILE_NAME_SIZE];
} generic_file_t;


#endif /* KERNEL_FILESYSTEM_FILETYPES_GENERICTYPE_H_ */
