#include "uartdriver_test.h"
#include "../../drivers/uart/uartdriver.h"
#include "../../filesystem/filesystem.h"
static void test_uart3();
static char monkey[] = "                 __,__\r\n"
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

void test_uartdriver(void){
    test_uart3();
}

static void test_uart3()
{
    filesystem_init();
    uartdriver_init();

    int uart_fd = mos_fs_open("uart3");

    char writeSign[1] = ">";
    mos_fs_write(uart_fd, monkey, strlen(monkey));
    mos_fs_write(uart_fd, writeSign, 1);

    char readBuff[50];
    while(1){
        mos_fs_read(uart_fd, readBuff, 50);
        volatile int i;

    }
}
