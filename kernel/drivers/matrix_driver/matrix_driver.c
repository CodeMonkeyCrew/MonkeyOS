#include "matrix_driver.h"
#include "../drivertypes.h"
#include "../../filesystem/filesystem.h"
#include "../../filesystem/filesystemregister.h"
#include "matrix_typedef.h"
#include "hal/gpiohal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <kernel/filesystem/filetypes/matrixtype.h>

static void createMatrixFiles(void);
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

static void initUsedGpio(uint8_t gpio, uint8_t dir, uint8_t val)
{
    //set clock and mux
    matrix_gpiohal_pinInit(gpio);
    //set direction to output
    matrix_gpiohal_pinSetDirection(gpio, dir);
    //set output value
    matrix_gpiohal_pinSetValue(gpio, val);

}

static void selectNextRow(Matrix_t* matrix)
{
    // get next row
    matrix->curRow = ++matrix->curRow % matrix->rowsPerSection;

    if (matrix->curRow & (1 << 0))
    {
        matrix_gpiohal_pinSetValue(matrix->addressPins.a, HIGH);
    }
    else
    {
        matrix_gpiohal_pinSetValue(matrix->addressPins.a, LOW);
    }

    if (matrix->curRow & (1 << 1))
    {
        matrix_gpiohal_pinSetValue(matrix->addressPins.b, HIGH);
    }
    else
    {
        matrix_gpiohal_pinSetValue(matrix->addressPins.b, LOW);
    }

    if (matrix->curRow & (1 << 2))
    {
        matrix_gpiohal_pinSetValue(matrix->addressPins.c, HIGH);
    }
    else
    {
        matrix_gpiohal_pinSetValue(matrix->addressPins.c, LOW);
    }

    if (matrix->curRow & (1 << 3))
    {
        matrix_gpiohal_pinSetValue(matrix->addressPins.d, HIGH);
    }
    else
    {
        matrix_gpiohal_pinSetValue(matrix->addressPins.d, LOW);
    }
}

static void updateDisplay(Matrix_t* matrix)
{
    // disable output during section switchover
    matrix_gpiohal_pinSetValue(matrix->controlPins.oe, HIGH);

    // latch data loaded during prior interrupt
    matrix_gpiohal_pinSetValue(matrix->controlPins.lat, HIGH);

    // select next row
    selectNextRow(matrix);

    // enable output
    matrix_gpiohal_pinSetValue(matrix->controlPins.oe, LOW);

    // latch down
    matrix_gpiohal_pinSetValue(matrix->controlPins.lat, LOW);

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
                matrix_gpiohal_pinSetValue(matrix->sectionPins[sectionIndex].r, HIGH);
            }
            else
            {
                matrix_gpiohal_pinSetValue(matrix->sectionPins[sectionIndex].r, LOW);
            }

            if (*ledBuffer & gMask)
            {
                matrix_gpiohal_pinSetValue(matrix->sectionPins[sectionIndex].g, HIGH);
            }
            else
            {
                matrix_gpiohal_pinSetValue(matrix->sectionPins[sectionIndex].g, LOW);
            }

            if (*ledBuffer & bMask)
            {
                matrix_gpiohal_pinSetValue(matrix->sectionPins[sectionIndex].b, HIGH);
            }
            else
            {
                matrix_gpiohal_pinSetValue(matrix->sectionPins[sectionIndex].b, LOW);
            }
        }
        // pulse clock
        matrix_gpiohal_pinSetValue(matrix->controlPins.clk, HIGH);
        matrix_gpiohal_pinSetValue(matrix->controlPins.clk, LOW);
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
            initUsedGpio(matrix->controlPins.clk, HIGH, LOW);
            initUsedGpio(matrix->controlPins.lat, HIGH, LOW); // latch down
            initUsedGpio(matrix->controlPins.oe, HIGH, HIGH); // disable output

            // initialize address pins
            initUsedGpio(matrix->addressPins.a, HIGH, LOW);
            initUsedGpio(matrix->addressPins.b, HIGH, LOW);
            initUsedGpio(matrix->addressPins.c, HIGH, LOW);
            initUsedGpio(matrix->addressPins.d, HIGH, LOW);

            // initialize section pins
            int sectionIndex;
            for (sectionIndex = 0; sectionIndex < matrix->numberOfSections;
                    ++sectionIndex)
            {
                initUsedGpio(matrix->sectionPins[sectionIndex].r, HIGH, LOW);
                initUsedGpio(matrix->sectionPins[sectionIndex].g, HIGH, LOW);
                initUsedGpio(matrix->sectionPins[sectionIndex].b, HIGH, LOW);
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
