#include <stdio.h>
#include <string.h>
#include "console.h"
#define BUFFSIZE 50
static char readBuff[BUFFSIZE];
static char monkey[] =
        "                 __,__\r\n"
                "        .--.  .-\"     \"-.  .--.\r\n"
                "       / .. \\/  .-. .-.  \\/ .. \\\r\n"
                "      | |  '|  /   Y   \\  |'  | |\r\n"
                "      | |  '|  /   Y   \\  |'  | |\r\n"
                "      | \\   \\  \\ 0 | 0 /  /   / |\r\n"
                "       \\ '- ,\\.-\"`` ``\"-./, -' /\r\n"
                "        `'-' /_   ^ ^   _\\ '-'`\r\n"
                "        .--'|  \\._ _ _./  |'--. \r\n"
                "      /`    \\   \\.-.  /   /    `\\\r\n"
                "     /       '._/  |-' _.'       \\\r\n"
                "    /          ;  /--~'   |       \\\r\n"
                "   /        .'\\|.-\\--.     \\       \\\r\n"
                "   /   .'-. /.-.;\\  |\\|'~'-.|\\       \\\r\n"
                "  \\       -./`|_\\_/      `\\'.      \\\r\n"
                "   '.      ;     ___)        '.`;    /\r\n"
                "     '-.,_ ;     ___)          \\/   /\r\n"
                "      \\   `'------'\\       \\     /\r\n"
                "       '.    \\       '.      |   ;/_\r\n"
                "     ___>     '.       \\_ _ _/   ,  '--.\r\n"
                "   .'   '.   .-~~~~~-. /     |--'`~~-.  \\\r\n"
                "  // / .---'/  .-~~-._/ / / /---..__.'  /\r\n"
                " ((_(_/    /  /      (_(_(_(---.__    .'\r\n"
                "           | |     _              `~~`\r\n"
                "           | |     \\'.\r\n"
                "            \\ '....' |\r\n"
                "             '.,___.'\r\n"
                "\r\n"
                " ____    ____                  __                        ___     ______   \r\n"
                "|_   \\  /   _|                [  |  _                  .'   `. .' ____ \\  \r\n"
                "  |   \\/   |   .--.   _ .--.   | | / ] .---.   _   __ /  .-.  \\| (___ \\_| \r\n"
                "  | |\\  /| | / .'`\\ \\[ .-. |  | '' < / /__\\\\ [ \\ [  ]| |   | | _.____.  \r\n"
                " _| |_\\/_| |_| \\__. | | | | |  | |`\\ \\| \\__.,  \\ '/ / \\  `-'  /| \\____) | \r\n"
                "|_____||_____|'.__.' [___||__][__|  \\_]'.__.'[\\_:  /   `.___.'  \\______.' \r\n"
                "                                              \\__.'                       \r\n";

static char writeSign[1] = ">";

void intepretMessage(char* buffer, int bufferSize, int uart_fd)
{
    char* delimiter = " \n\r\t\0";
    char* executableName = strtok(buffer, delimiter);
    char* param1 = strtok(NULL, delimiter);
    char* param2 = strtok(NULL, delimiter);
    char* argv[1];
    argv[0] = param1;
    // argv[1] = param2;

    // int pid = 0;
    int pid = fork();
    if (pid == 0)
    {
        //printf("child process\n");
        //we are the child process
        int fd = execv(executableName, argv);

        //should not come here, only if executable file not existing
        char* notFound = "command not found\n\r";
        write(uart_fd, notFound, strlen(notFound));
        printf(notFound);
        exitProc(0);

    }
    else if (pid > 0)
    {
        //we are parent process. what to do??
        // printf("parent process. New child process has pid: %i\n", pid);
        waitPid(pid);

    }

    write(uart_fd, writeSign, 1);

}
void console_run(void)
{
    //main function
    int uart_fd = open("uart3");

    write(uart_fd, monkey, strlen(monkey));
    write(uart_fd, writeSign, 1);
    int i;
    while (1)
    {
        i = read(uart_fd, readBuff, BUFFSIZE);
        if (i >= 0)
        {
            intepretMessage(readBuff, BUFFSIZE, uart_fd);
            readBuff[0] = '\0';
        }
    }
}

