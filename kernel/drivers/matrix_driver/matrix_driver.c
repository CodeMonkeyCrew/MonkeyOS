#include "matrix_driver.h"
#include "../drivertypes.h"
#include "../../filesystem/filesystem.h"
#include "../../filesystem/filesystemregister.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <kernel/filesystem/filetypes/matrixtype.h>

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

#define NUMBER_OF_MATRICES 1
#define BUFFER_SIZE(numberOfSections, rowsPerSection, ledsPerRow) (numberOfSections * rowsPerSection * ledsPerRow / 2)

#define MATRIX0_NUMBER_OF_SECTIONS 2
#define MATRIX0_ROWS_PER_SECTION 16
#define MATRIX0_LEDS_PER_ROW 32
#define MATRIX0_BUFFER_SIZE BUFFER_SIZE(MATRIX0_NUMBER_OF_SECTIONS, MATRIX0_ROWS_PER_SECTION, MATRIX0_LEDS_PER_ROW)

typedef struct
{
    uint8_t number;
    int dirFd;
    int valFd;
} GPIO_t;

typedef struct
{
    GPIO_t clk;
    GPIO_t lat;
    GPIO_t oe;
} ControlGPIO_t;

typedef struct
{
    GPIO_t a;
    GPIO_t b;
    GPIO_t c;
    GPIO_t d;
} AddressGPIO_t;

typedef struct
{
    GPIO_t r;
    GPIO_t g;
    GPIO_t b;
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

static const uint8_t LOW = 0;
static const uint8_t HIGH = 1;

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

void matrix_driver_init(void);
void matrix_driver_update(void);

static void createMatrixFiles(void);
static void setGPIODir(GPIO_t* gpio, uint8_t dir);
static void setGPIOVal(GPIO_t* gpio, uint8_t val);
static void initGPIO(GPIO_t* gpio, uint8_t dir, uint8_t val);
static void selectNextRow(Matrix_t* matrix);
static void updateDisplay(Matrix_t* matrix);
static int open(generic_file_t* pFile);
static int read(void* pBuffer, int bufferSize, generic_file_t* pFile);
static int write(const void* pBuffer, int bufferSize, generic_file_t* pFile);

static driver_t matrixDriver = { .driver_open = open, .driver_read = read,
                                 .driver_write = write };

void matrix_driver_init(void)
{
    register_driver(MATRIX, &matrixDriver);
    createMatrixFiles();
}

void matrix_driver_update(void) {
    int matrixId;
    for(matrixId = 0; matrixId < NUMBER_OF_MATRICES; ++matrixId) {
        updateDisplay(&matrices[matrixId]);
    }
}

static void createMatrixFiles(void)
{
    MatrixFile_t* pMatrix0File = (MatrixFile_t*) mos_fs_create_file(MATRIX);
    if (pMatrix0File != NULL)
    {
        strcpy(pMatrix0File->header.name, "matrix0");
        pMatrix0File->header.is_open = false;
        pMatrix0File->header.f_type = MATRIX;
        pMatrix0File->header.size = 0;
        pMatrix0File->id = 0;
    }
}

static void setGPIODir(GPIO_t* gpio, uint8_t dir)
{
    mos_fs_write(gpio->dirFd, &dir, 1);
}

static void setGPIOVal(GPIO_t* gpio, uint8_t val)
{
    mos_fs_write(gpio->valFd, &val, 1);
}

static void initGPIO(GPIO_t* gpio, uint8_t dir, uint8_t val)
{
    char strBuffer[12];

    sprintf(strBuffer, "gpio%i_dir", gpio->number);
    gpio->dirFd = mos_fs_open(strBuffer);

    sprintf(strBuffer, "gpio%i_val", gpio->number);
    gpio->valFd = mos_fs_open(strBuffer);

    setGPIODir(gpio, dir);
    setGPIOVal(gpio, val);
}

static void selectNextRow(Matrix_t* matrix)
{
    // get next row
    matrix->curRow = ++matrix->curRow % matrix->rowsPerSection;

    if (matrix->curRow & (1 << 0))
    {
        setGPIOVal(&matrix->addressPins.a, HIGH);
    }
    else
    {
        setGPIOVal(&matrix->addressPins.a, LOW);
    }

    if (matrix->curRow & (1 << 1))
    {
        setGPIOVal(&matrix->addressPins.b, HIGH);
    }
    else
    {
        setGPIOVal(&matrix->addressPins.b, LOW);
    }

    if (matrix->curRow & (1 << 2))
    {
        setGPIOVal(&matrix->addressPins.c, HIGH);
    }
    else
    {
        setGPIOVal(&matrix->addressPins.c, LOW);
    }

    if (matrix->curRow & (1 << 3))
    {
        setGPIOVal(&matrix->addressPins.d, HIGH);
    }
    else
    {
        setGPIOVal(&matrix->addressPins.d, LOW);
    }
}

static void updateDisplay(Matrix_t* matrix)
{
    // disable output during section switchover
    setGPIOVal(&matrix->controlPins.oe, HIGH);

    // latch data loaded during prior interrupt
    setGPIOVal(&matrix->controlPins.lat, HIGH);

    // select next row
    selectNextRow(matrix);

    // enable output
    setGPIOVal(&matrix->controlPins.oe, LOW);

    // latch down
    setGPIOVal(&matrix->controlPins.lat, LOW);

    int ledIndex;
    int rMask, gMask, bMask;
    for (ledIndex = 0; ledIndex < matrix->ledsPerRow; ++ledIndex)
    {
        if ((ledIndex % 2) == 0)
        {
            // use lower values
            rMask = (1 << 0);
            gMask = (1 << 1);
            bMask = (1 << 2);
        }
        else
        {
            // use upper values
            rMask = (1 << 4);
            gMask = (1 << 5);
            bMask = (1 << 6);
        }

        // load data for next row of each section
        int sectionIndex;
        for (sectionIndex = 0; sectionIndex < matrix->numberOfSections; ++sectionIndex)
        {
            int rowIndex = matrix->curRow + (sectionIndex * matrix->rowsPerSection);
            uint8_t* rowBuffer = matrix->buffer + (rowIndex * matrix->ledsPerRow / 2);
            uint8_t* ledBuffer = rowBuffer + (ledIndex / 2);

            if (*ledBuffer & rMask)
            {
                setGPIOVal(&matrix->sectionPins[sectionIndex].r, HIGH);
            }
            else
            {
                setGPIOVal(&matrix->sectionPins[sectionIndex].r, LOW);
            }

            if (*ledBuffer & gMask)
            {
                setGPIOVal(&matrix->sectionPins[sectionIndex].g, HIGH);
            }
            else
            {
                setGPIOVal(&matrix->sectionPins[sectionIndex].g, LOW);
            }

            if (*ledBuffer & bMask)
            {
                setGPIOVal(&matrix->sectionPins[sectionIndex].b, HIGH);
            }
            else
            {
                setGPIOVal(&matrix->sectionPins[sectionIndex].b, LOW);
            }
        }
        // pulse clock
        setGPIOVal(&matrix->controlPins.clk, HIGH);
        setGPIOVal(&matrix->controlPins.clk, LOW);
    }
}

static int open(generic_file_t* pFile)
{
    if (pFile != NULL)
    {
        MatrixFile_t* pMatrixFile = (MatrixFile_t*) pFile;
        if (pMatrixFile->id < NUMBER_OF_MATRICES)
        {
            Matrix_t* matrix = &matrices[pMatrixFile->id];

            // initialize control pins
            initGPIO(&matrix->controlPins.clk, HIGH, LOW);
            initGPIO(&matrix->controlPins.lat, HIGH, LOW); // latch down
            initGPIO(&matrix->controlPins.oe, HIGH, HIGH); // disable output

            // initialize address pins
            initGPIO(&matrix->addressPins.a, HIGH, LOW);
            initGPIO(&matrix->addressPins.b, HIGH, LOW);
            initGPIO(&matrix->addressPins.c, HIGH, LOW);
            initGPIO(&matrix->addressPins.d, HIGH, LOW);

            // initialize section pins
            int sectionIndex;
            for (sectionIndex = 0; sectionIndex < matrix->numberOfSections;
                    ++sectionIndex)
            {
                initGPIO(&matrix->sectionPins[sectionIndex].r, HIGH, LOW);
                initGPIO(&matrix->sectionPins[sectionIndex].g, HIGH, LOW);
                initGPIO(&matrix->sectionPins[sectionIndex].b, HIGH, LOW);
            }
        }
    }
    return 0;
}

static int read(void* pBuffer, int bufferSize, generic_file_t* pFile)
{
    if ((pBuffer != NULL) && (bufferSize > 0) && (pFile != NULL))
    {
        MatrixFile_t* pMatrixFile = (MatrixFile_t*) pFile;
        if (pMatrixFile->id < NUMBER_OF_MATRICES)
        {
            Matrix_t* matrix = &matrices[pMatrixFile->id];

            int copySize = bufferSize;
            if (copySize > matrix->bufferSize)
            {
                copySize = matrix->bufferSize;
            }

            memcpy(pBuffer, matrix->buffer, copySize);

            // number of bytes read
            return copySize;
        }
    }
    // no bytes read
    return 0;
}

static int write(const void* pBuffer, int bufferSize, generic_file_t* pFile)
{
    if ((pBuffer != NULL) && (bufferSize > 0) && (pFile != NULL))
    {
        MatrixFile_t* pMatrixFile = (MatrixFile_t*) pFile;
        if (pMatrixFile->id < NUMBER_OF_MATRICES)
        {
            Matrix_t* matrix = &matrices[pMatrixFile->id];

            int copySize = bufferSize;
            if (copySize > matrix->bufferSize)
            {
                copySize = matrix->bufferSize;
            }

            memcpy(matrix->buffer, pBuffer, copySize);

            // number of bytes written
            return copySize;
        }
    }
    // no bytes written
    return 0;
}
