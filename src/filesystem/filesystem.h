#ifndef SRC_FILESYSTEM_FILESYSTEM_H_
#define SRC_FILESYSTEM_FILESYSTEM_H_

#define FILE_NAME_SIZE 16

//LED, Button, Timer
typedef enum {LED, BTN, TMR} file_ext_type;

typedef struct {
    char name[FILE_NAME_SIZE];
    file_ext_type type;
    int size;
} FileDescriptorHeader_t;

typedef struct {
    FileDescriptorHeader_t header;
    int gpioPinNumber; //149
    int gpioPort; //5

} FileDescriptorGPIO_t;

typedef struct{
    FileDescriptorHeader_t header;

} FileDescriptor_t;

//app:
write(numberOfPointerOfGPIO149DirectionFile, p, count);
write(numberOfPointerOfGPIO149File, p, count);


int mos_fs_write_GPIO (int GPIO_Number, int IO, int State);

//source: http://codewiki.wikidot.com/system-calls
int mos_fs_create (const char* file_name, file_ext_type file_type); //creates a file with the given file_name and type
int mos_fs_open (const char* file_name);  //opens a file and returns a file descriptor (simple integer which is increased with each new opened file)
int mos_fs_close (int file_descriptor);   //closes a file, returns 0 on success, -1 on error

//int mos_fs_read (int file_descriptor, char* buf, int buffer_size); //returns the number of bytes that were read. If negative an error occurred
int mos_fs_read(int file_descriptor);
int mos_fs_write (int file_descriptor, const char* data); //writes the content of the buffer into the file with the correct file_descriptor

#endif /* SRC_FILESYSTEM_FILESYSTEM_H_ */
