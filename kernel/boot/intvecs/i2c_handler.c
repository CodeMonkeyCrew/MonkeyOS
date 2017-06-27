
#define i2c_1_stat 0x48070008
#include <inttypes.h>

void handle_i2c() {
    uint32_t clear_field = (0 << 31);
    unsigned int* pI2C_1_stat = (unsigned int*)i2c_1_stat;

    *pI2C_1_stat &= clear_field;
}
