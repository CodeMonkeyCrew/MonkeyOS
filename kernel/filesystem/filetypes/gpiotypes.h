#ifndef KERNEL_FILESYSTEM_FILETYPES_GPIOTYPES_H_
#define KERNEL_FILESYSTEM_FILETYPES_GPIOTYPES_H_

#include "generictype.h"
#include <inttypes.h>

typedef struct {
    int number;
    int port;
    int start_bit;
    int shift;
    uint32_t* mux_mode_addr;
} gpio_info_t;

typedef struct {
    generic_file_t header;
    gpio_info_t gpio_info;
    uint32_t* OE;
} gpio_direction_file_t;

typedef struct {
    generic_file_t header;
    gpio_info_t gpio_info;
    uint32_t* data_out;
} gpio_value_file_t;




#endif /* KERNEL_FILESYSTEM_FILETYPES_GPIOTYPES_H_ */
