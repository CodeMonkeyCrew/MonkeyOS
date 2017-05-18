#include "console.h"
#include <stdio.h>
#include <string.h>
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

void intepretMessage(char* buffer, int bufferSize, int uart_fd)
{
    char first[10];
    int i;

    for (i = 0; i < bufferSize; i++)
    {
        if (buffer[i] != ' ' && buffer[i] != '\0' && buffer[i] != '\n' && buffer[i] != '\r')
        {
            first[i] = buffer[i];
        }
        else
        {
            break;
        }
    }
    first[i] = '\0';
    if(strcmp(first, "execute") == 0){
        //do execute file
        mos_fs_write(uart_fd, writeSign, 1);
        printf("Execute a file\n");
    }else if(strcmp(first, "pc") == 0){
        //do show all processes
        mos_fs_write(uart_fd, writeSign, 1);
        printf("Show all processes\n");
    }else{
        char* notFound = "command not found\n\r";
        mos_fs_write(uart_fd, notFound, strlen(notFound));
        mos_fs_write(uart_fd, writeSign, 1);
        printf(notFound);
    }
}
void console_run(void)
{
    //main function
    int uart_fd = mos_fs_open("uart3");

    mos_fs_write(uart_fd, monkey, strlen(monkey));
    mos_fs_write(uart_fd, writeSign, 1);
    int buffSize = 50;
    char readBuff[50];
    while (1)
    {
        mos_fs_read(uart_fd, readBuff, buffSize);
        intepretMessage(readBuff, buffSize, uart_fd);
    }
}

