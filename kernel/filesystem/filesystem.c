#include "filetypes/generictype.h"
#include <apps/echo/echo.h>
#include "filesystem.h"
#include "filesystemregister.h"
#include "../../apps/ps/ps.h"
#include <stdlib.h>

#define false 0
#define true !false

static int currentFileNo = 0;
static generic_file_t* files[MAX_NO_OF_FILES];
static int open_files[MAX_NO_OF_OPEN_FILES];

//size depending on number of elements in file_types_t enum in generictype.h!
static driver_t* drivers[NO_OF_FILE_TYPES];

void createExecutableFile(void)
{
    exe_file_t* pExe = (exe_file_t*) mos_fs_create_file(EXE);
    if (pExe != NULL)
    {
        strcpy(pExe->header.name, "echo");
        pExe->header.is_open = false;
        pExe->header.f_type = EXE;
        pExe->header.size = 0;
        pExe->entryPoint = echo;
    }
}
void createProcessFile(void)
{
    exe_file_t* pExe = (exe_file_t*) mos_fs_create_file(EXE);
    if (pExe != NULL)
    {
        strcpy(pExe->header.name, "ps");
        pExe->header.is_open = false;
        pExe->header.f_type = EXE;
        pExe->header.size = 0;
        pExe->entryPoint = ps;
    }
}
void createStdFiles(void)
{
    std_file_t* pStdin = (std_file_t*) mos_fs_create_file(STD);
    if (pStdin != NULL)
    {
        strcpy(pStdin->header.name, "stdin");
        pStdin->header.is_open = false;
        pStdin->header.f_type = STD;
        pStdin->header.size = 0;
        pStdin->buffer = "";
    }
    std_file_t* pStdout = (std_file_t*) mos_fs_create_file(STD);
    if (pStdout)
    {
        strcpy(pStdout->header.name, "stdout");
        pStdout->header.is_open = false;
        pStdout->header.f_type = STD;
        pStdout->header.size = 0;
        pStdout->buffer = "";
    }
}

int mos_fs_init(void)
{
    int i;

    //-1 to signal that there is no file mapped yet
    for (i = 0; i < MAX_NO_OF_OPEN_FILES; ++i)
    {
        open_files[i] = -1;
    }
    createStdFiles();
    mos_fs_open("stdin");
    mos_fs_open("stdout");
    createExecutableFile();
    createProcessFile();
    return 0;
}

static int add_new_file(generic_file_t* file)
{
    if (currentFileNo < MAX_NO_OF_FILES)
    {
        files[currentFileNo++] = file;
    }
    else
    {
        return -1;
    }
    return 0;
}

/*
 * Checks if a new file is valid and sets basic fields.
 */
static generic_file_t* check_if_file_is_valid(generic_file_t* pFile,
                                              file_types_t file_type)
{
    if (pFile != NULL)
    {
        if (add_new_file(pFile) >= 0)
        {
            pFile->f_type = file_type;
            pFile->is_open = false;
            pFile->size = 0;
            return pFile;
        }
        else
        {
            free(pFile);
        }
    }
    return NULL;
}

generic_file_t* mos_fs_create_file(file_types_t file_type)
{

    generic_file_t* pNewFile;

    switch (file_type) {
        case GPIO_DIR:
            pNewFile = (generic_file_t*)malloc(sizeof(gpio_direction_file_t));
            break;
        case GPIO_VAL:
            pNewFile = (generic_file_t*)malloc(sizeof(gpio_value_file_t));
            break;
        case TIMER:
            pNewFile = (generic_file_t*)malloc(sizeof(timer_file_t));
            break;
        case TIMER_INT:
            pNewFile = (generic_file_t*)malloc(sizeof(timer_interrupt_file_t));
            break;
        case TIMER_MODE:
            pNewFile = (generic_file_t*)malloc(sizeof(timer_mode_file_t));
            break;
        case MATRIX:
            pNewFile = (generic_file_t*)malloc(sizeof(MatrixFile_t));
            break;
        case UART:
            pNewFile = (generic_file_t*)malloc(sizeof(uart_file_t));
            break;
        case EXE:
             pNewFile = (generic_file_t*) malloc(sizeof(exe_file_t));
             break;
         case STD:
             pNewFile = (generic_file_t*) malloc(sizeof(std_file_t));
             break;
    }
    return check_if_file_is_valid(pNewFile, file_type);
}

int mos_fs_open(const char* file_name)
{
    int i;
    for (i = 0; i < currentFileNo; ++i)
    {
        if (strcmp(files[i]->name, file_name) == 0)
        {
            //open file only once for now..
            if (files[i]->is_open)
            {
                return -1;
            }
            int openFileIndex = create_entry(i); //returns the file descriptor; -3 if there is no space for another file
            if (openFileIndex >= 0)
            {
                driver_t* driver = drivers[files[i]->f_type];
                if (driver != NULL)
                {
                    driver->driver_open(files[i]);
                }
            }
            return openFileIndex;
        }
    }
    //file not found
    return -2;
}

static int create_entry(int fileIndex)
{
    int i;
    for (i = 0; i < MAX_NO_OF_OPEN_FILES; ++i)
    {
        if (open_files[i] == -1 || !files[open_files[i]]->is_open)
        {
            open_files[i] = fileIndex;
            files[open_files[i]]->is_open = true;
            return i;
        }
    }
    //no free entry found
    return -3;
}

int mos_fs_close(int file_descriptor)
{
    generic_file_t* pFile = fs_get_open_file(file_descriptor);
    if (pFile != NULL)
    {
        pFile->is_open = false;
    }
    return 0;
}

int mos_fs_read(int file_descriptor, void* buf, int buffer_size)
{
    generic_file_t* pFile = fs_get_open_file(file_descriptor);
    if (pFile != NULL)
    {
        return drivers[pFile->f_type]->driver_read(buf, buffer_size, pFile);
    }
    //no valid file_descriptor
    return -1;
}

int mos_fs_write(int file_descriptor, const void* buf, int buffer_size)
{
    generic_file_t* pFile = fs_get_open_file(file_descriptor);
    if (pFile != NULL)
    {
        return drivers[pFile->f_type]->driver_write(buf, buffer_size, pFile);
    }
    //no valid file_descriptor
    return -1;
}

int register_driver(file_types_t f_type, driver_t* pDriver)
{
    drivers[f_type] = pDriver;
    return 0;
}

generic_file_t* fs_get_open_file(int file_descriptor)
{
    if (file_descriptor >= 0 && file_descriptor <= MAX_NO_OF_OPEN_FILES)
    {
        generic_file_t* pFile = files[open_files[file_descriptor]];
        if (pFile != NULL)
        {
            return pFile;
        }
    }
    return NULL;
}
