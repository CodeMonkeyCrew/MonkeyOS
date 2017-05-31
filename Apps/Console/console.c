#include <stdio.h>
#include <string.h>
#include "console.h"
char monkey[] =
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

char writeSign[1] = ">";
char** split(char* buffer, int bufferSize)
{
    char *result[2];
    char first[10];

    int i;
    for (i = 0; i < bufferSize; i++)
    {
        if (buffer[i] != ' ' && buffer[i] != '\0' && buffer[i] != '\n'
                && buffer[i] != '\r')
        {
            first[i] = buffer[i];
        }
        else
        {
            break;
        }
    }
    first[i] = '\0';
    result[0] = first;
    i++;
    int k;
    char second[10];
    for (k = i; k < bufferSize; k++)
    {
        if (buffer[k] != ' ' && buffer[k] != '\0' && buffer[k] != '\n'
                && buffer[k] != '\r')
        {
            second[k] = buffer[k];
        }
        else
        {
            break;
        }
    }
    second[i] = '\0';
    result[1] = second;
    return result;
}
void intepretMessage(char* buffer, int bufferSize, int uart_fd)
{
    char first[10];
    int i;

    for (i = 0; i < bufferSize; i++)
    {
        if (buffer[i] != ' ' && buffer[i] != '\0' && buffer[i] != '\n'
                && buffer[i] != '\r')
        {
            first[i] = buffer[i];
        }
        else
        {
            break;
        }
    }
    first[i] = '\0';
    if (strcmp(first, "execute") == 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            printf("child process\n");
            //we are the child process
            //change its behavior-> call: int execve(const char *filename, char *const argv[], char *const envp[]);
            //may change signature to: int execve(const char *filename, char *const argv[], int argc);
            //execve(secondWord, thirdWord, 1);
        }
        else if (pid > 0)
        {
            //we are parent process. what to do??
            printf("parent process. New child process has pid: %i\n", pid);
        }
        else
        {

            //error occurred!
        }
    }
    else if (strcmp(first, "pc") == 0)
    {
        //do show all processes
        printf("Show all processes\n");
    }
    else
    {
        char* notFound = "command not found\n\r";
        write(uart_fd, notFound, strlen(notFound));
        printf(notFound);
    }
    write(uart_fd, writeSign, 1);

}
void console_run(void)
{
    //main function
    int uart_fd = open("uart3");

    write(uart_fd, monkey, strlen(monkey));
    write(uart_fd, writeSign, 1);
    int buffSize = 50;
    char readBuff[50];
    while (1)
    {
        read(uart_fd, readBuff, buffSize);
        intepretMessage(readBuff, buffSize, uart_fd);
        readBuff[0] = '\0';
    }
}

