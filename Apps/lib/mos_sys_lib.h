/*===============================================================================*
 *  __  __             _               ___  ____    ____            _ _ _        *
 * |  \/  | ___  _ __ | | _____ _   _ / _ \/ ___|  / ___| _   _ ___| (_) |__     *
 * | |\/| |/ _ \| '_ \| |/ / _ \ | | | | | \___ \  \___ \| | | / __| | | '_ \    *
 * | |  | | (_) | | | |   <  __/ |_| | |_| |___) |  ___) | |_| \__ \ | | |_) |   *
 * |_|  |_|\___/|_| |_|_|\_\___|\__, |\___/|____/  |____/ \__, |___/_|_|_.__/    *
 *                              |___/                     |___/                  *
 *  Syslib for MonkeyOS. Uses UNIX standarts for system calls.                   *
 *  http://www.tutorialspoint.com/unix_system_calls/                             *
 *===============================================================================*/
#ifndef MOS_SYS_LIB__H
#define MOS_SYS_LIB__H

/*===============================================================================*
 * FileSystem System Calls*
 *===============================================================================*/
/*
 * Given a pathname for a file, open() returns a file descriptor.
 * The parameter flags must include one of the following access modes: O_RDONLY, O_WRONLY, or O_RDWR.
 * These request opening the file read-only, write-only, or read/write, respectively.
 */
#pragma SWI_ALIAS(open, 0);
int open(const char *pathname);
/*
 * close() closes a file descriptor, so that it no longer refers to any file and may be reused.
 * returns:
 *       0 on success
 *       -1 on error
 */
#pragma SWI_ALIAS(close,1)
int close(int fd);
/*
 * write() writes up to count bytes to the file referenced by the file descriptor fd from the buffer starting at buf.
 * returns:
 *       On success, the number of bytes written are returned (zero indicates nothing was written)
 *       On error, -1 is returned
 */
#pragma SWI_ALIAS(write,2)
int write(int fd, const void *buf, int count);

/**
 * read() reads from a file and saves the read chars in the buffer buf
 * returns:
 *      the number of read bytes
 */
#pragma SWI_ALIAS(read, 3)
int read(int file_descriptor, void* buf, int buffer_size);
/**
 * fork() duplicates a Process an saves its Context. The new Child-Process is set to ready.
 * returns:
 *      0 if the child Process is calling
 *      PID of childProcess if parent process is calling
 */
#pragma SWI_ALIAS(fork, 4)
int fork(void);
#endif
