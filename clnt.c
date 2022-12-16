//CLNT

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

// debug
#define VERBOSE 1
#define BUFF 16

// booleans
#define TRUE 1
#define FALS 0
typedef int bool;

// gameplay
#define HIGH 23
#define WIDE 80

#define SNAK '&'
#define SNEK 'O'

#define REDO 'r'
#define QUIT 'q'

#define FORE 'w'
#define BACK 's'
#define LEFT 'a'
#define RITE 'd'

int main(int argc, char argv[])
{
    //creating socket
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address for socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(sock, (struct sockaddr) &server_address, sizeof(server_address));
    //error checking
    if (connection_status == -1)
    {
        printf("There was an error making connection to the remote socket \n\n");
    }

    while (1) {
        char str[256];
        fgets(str, 256, stdin);
        write(sock, str, sizeof(str));
    }


    //close sock

    return 0;
}
