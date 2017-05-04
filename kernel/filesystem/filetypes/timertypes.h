#ifndef KERNEL_FILESYSTEM_FILETYPES_TIMERTYPES_H_
#define KERNEL_FILESYSTEM_FILETYPES_TIMERTYPES_H_

#include "generictype.h"
#include "../../drivers/timer_driver/timermodes.h"
#include <inttypes.h>

typedef struct {
    int timer_number;
} timer_info_t;

typedef struct {
    generic_file_t header;
    timer_info_t info;
} timer_file_t;

typedef struct {
    generic_file_t header;
    timer_info_t info;
} timer_interrupt_file_t;


typedef struct {
    generic_file_t header;
    timer_info_t info;
    timer_mode_t timer_mode;
    uint8_t compareregister;
} timer_mode_file_t;

#endif /* KERNEL_FILESYSTEM_FILETYPES_TIMERTYPES_H_ */
