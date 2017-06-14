#ifndef KERNEL_DRIVERS_MATRIX_DRIVER_MATRIX_DRIVER_H_
#define KERNEL_DRIVERS_MATRIX_DRIVER_MATRIX_DRIVER_H_

/* MATRIX 0 CONFIGURATION
 * ------------------------------------
 *
 * GPIO PINS
 * ------------------------------------
 * NAME     PIN NO      GPIO NO
 * A        15          133
 * B        14          162
 * C        13          134
 * D        12          158
 * R1       23          183
 * R2       19          131
 * G1       18          159
 * G2       16          161
 * B1       21          130
 * B2       17          132
 * CLK      11          135
 * LAT      10          145
 * OE       9           136
 *
 * SECTIONS
 * ------------------------------------
 * INDEX    NAME    START ROW   END ROW
 * 0        UPPER   0           15
 * 1        LOWER   16          31
 */
void matrix_driver_init(void);
void matrix_driver_update(void);

#endif /* KERNEL_DRIVERS_MATRIX_DRIVER_MATRIX_DRIVER_H_ */
