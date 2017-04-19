#ifndef SRC_FILESYSTEM_FILETYPES_H_
#define SRC_FILESYSTEM_FILETYPES_H_
#include <inttypes.h>


typedef enum {GPIO, LEDM} driver_type_enum;
#define NO_OF_DRIVER_TYPES 2

//Direction, Value
typedef enum {DIR, VAL} gpio_file_type_t;

#define FILE_NAME_SIZE 16

typedef enum { false, true } bool;

typedef struct {
    char name[FILE_NAME_SIZE];
    driver_type_enum d_type;
    bool is_open;
    int size;
} generic_file_type;

typedef struct {
    generic_file_type header;
    gpio_file_type_t f_type;
    int number;
    int port;
    int start_bit;
    int shift;
    uint32_t mux_mode_addr;
} gpio_file_type;

typedef struct {
    gpio_file_type gpio_file;
    uint32_t OE;
} gpio_direction_file_type;

typedef struct {
    gpio_file_type gpio_file;
    uint32_t data_out;
} gpio_value_file_type;


#endif /* SRC_FILESYSTEM_FILETYPES_H_ */
