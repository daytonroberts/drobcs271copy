#include <stdio.h>
#include <ncurses.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


#define PORT 8080 
#define DOMAIN AF_INET
#define BUFF_SIZE 1024
#define LOOPBACK "::1"
#define CHECK1 "check 1"
#define CHECK2 "check 2"
#define CHECK3 "check 3"



int main(int argc, char const *argv[])
{
	printf("%s, expects (1) arg, %d provided", argv[0], argc-1);
	if (argc == 2)
	{
		printf(": \"%s\".\n", argv[1]);
	} else {
		printf(".\n");
		return 1;
	}

	if (argv[1][1] == 's')
	{
		printf("Starting server...\n");
		
		int sock;
		sock = socket(DOMAIN, SOCK_STREAM, 0);
		
		struct sockaddr_in address; 
		address.sin_family = DOMAIN; 
		address.sin_port = htons(PORT); 
		inet_pton(DOMAIN, "::1", &address.sin_addr);
		
		bind(sock, (struct sockaddr*) &address, sizeof(address));
		listen(sock, 3);
		
		int c_sock;
		c_sock = accept(sock, NULL, NULL);
		
		void *incoming[BUFF_SIZE];
		
		while (1) {
			read(c_sock, incoming, sizeof(incoming));
			printf("%s\n", (char *)incoming);
		}
			
	}

	if (argv[1][1] == 'c')
	{
		printf("Starting client...\n");
		
		int sock;
		struct sockaddr_in address; 

		sock = socket(DOMAIN, SOCK_STREAM, 0);

		address.sin_family = DOMAIN; 

		address.sin_port = htons(PORT);
		inet_pton(DOMAIN, "::1", &address.sin_addr); 

		connect(sock, (struct sockaddr*) &address, sizeof(address));

		char buff[BUFF_SIZE];
		
		while (1) {
			fgets(buff, sizeof(buff), stdin);
			write(sock, buff, strlen(buff));
		
	}

	if (argv[1][1] == 'h')
	{
		printf("HELP: Usage:  -s for server, -c for client\n");
	}

    return 0;
	}
}	
