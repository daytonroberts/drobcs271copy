//SERV

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <ncurses.h>

int buff_size = 1024;
int arr[10][10];

int main(int argc, char argv[])
{
    char server_message[256] = "You reached server";

    int serv_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(serv_sock, (struct sockaddr) &server_address, sizeof(server_address));

    listen(serv_sock, 1);

    int client_sock;
    client_sock = accept(serv_sock, NULL, NULL);

    void incoming[buff_size];

    initscr();
    refresh();

    int high = 23;
    int wide = 80;
    getmaxyx(stdscr, high, wide);

    WINDOW board_win = newwin(high, wide, 10, 10);
    box(board_win, 0, 0);
    wrefresh(board_win);

    while (1)
    {
        read(client_sock, (char *)incoming, sizeof(incoming));
        printw("%s\n", incoming); 
        refresh();
    }
    return 0;
}
