#ifndef KERNEL_DRIVERS_MATRIX_DRIVER_MATRIX_TYPEDEF_H_
#define KERNEL_DRIVERS_MATRIX_DRIVER_MATRIX_TYPEDEF_H_
#include <inttypes.h>

static const uint8_t LOW = 0;
static const uint8_t HIGH = 1;

#define NUMBER_OF_MATRICES 1
#define BUFFER_SIZE(numberOfSections, rowsPerSection, ledsPerRow) (numberOfSections * rowsPerSection * ledsPerRow / 2)

#define MATRIX0_NUMBER_OF_SECTIONS 2
#define MATRIX0_ROWS_PER_SECTION 16
#define MATRIX0_LEDS_PER_ROW 32
#define MATRIX0_BUFFER_SIZE BUFFER_SIZE(MATRIX0_NUMBER_OF_SECTIONS, MATRIX0_ROWS_PER_SECTION, MATRIX0_LEDS_PER_ROW)



typedef struct
{
    uint8_t clk;
    uint8_t lat;
    uint8_t oe;
} ControlGPIO_t;

typedef struct
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
} AddressGPIO_t;

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} SectionGPIO_t;

typedef struct
{
    ControlGPIO_t controlPins;
    AddressGPIO_t addressPins;
    SectionGPIO_t* sectionPins;
    uint8_t numberOfSections;
    uint8_t rowsPerSection;
    uint8_t ledsPerRow;
    uint8_t* buffer;
    uint16_t bufferSize;
    int16_t curRow;
} Matrix_t;
static SectionGPIO_t matrix0SectionPins[MATRIX0_NUMBER_OF_SECTIONS] = {
    {
        .r = { 183 },
        .g = { 159 },
        .b = { 130 }
    },
    {
        .r = { 131 },
        .g = { 161 },
        .b = { 132 }
    }
};

static uint8_t matrix0Buffer[MATRIX0_BUFFER_SIZE];

static Matrix_t matrices[NUMBER_OF_MATRICES] = {
    {
      .controlPins = {
          .clk =  { 135 },
          .lat =  { 145 },
          .oe =   { 136 }
      },
      .addressPins = {
          .a =    { 133 },
          .b =    { 162 },
          .c =    { 134 },
          .d =    { 158 }
       },
       .sectionPins = matrix0SectionPins,
       .numberOfSections = MATRIX0_NUMBER_OF_SECTIONS,
       .rowsPerSection = MATRIX0_ROWS_PER_SECTION,
       .ledsPerRow = MATRIX0_LEDS_PER_ROW,
       .buffer = matrix0Buffer,
       .bufferSize = MATRIX0_BUFFER_SIZE,
       .curRow = -1
    }
};

#endif /* KERNEL_DRIVERS_MATRIX_DRIVER_MATRIX_TYPEDEF_H_ */
