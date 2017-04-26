#ifndef KERNEL_FILESYSTEM_FILETYPES_H_
#define KERNEL_FILESYSTEM_FILETYPES_H_
#include <inttypes.h>

//Direction, Value
typedef enum {GPIO_DIR, GPIO_VAL} file_types_t;
#define NO_OF_FILE_TYPES 2

#define FILE_NAME_SIZE 16

typedef enum { false, true } bool;

typedef struct {
    file_types_t f_type;
    bool is_open;
    int size;
    char name[FILE_NAME_SIZE];
} generic_file_t;

typedef struct {
    int number;
    int port;
    int start_bit;
    int shift;
    uint32_t* mux_mode_addr;
} gpio_info_t;

typedef struct {
    generic_file_t header;
    gpio_info_t* gpio_info;
    uint32_t* OE;
} gpio_direction_file_t;

typedef struct {
    generic_file_t header;
    gpio_info_t* gpio_info;
    uint32_t* data_out;
} gpio_value_file_t;


#endif /* KERNEL_FILESYSTEM_FILETYPES_H_ */
