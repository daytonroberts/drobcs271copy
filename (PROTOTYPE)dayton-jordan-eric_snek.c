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
#define BUFF_SIZE 2
#define LOOPBACK "::1"
#define CHECK1 "check 1"
#define CHECK2 "check 2"
#define CHECK3 "check 3"

// debug
#define VERBOSE 1
#define BUFF 16

// gameplay
#define HIGH 8
#define WIDE 24

#define CHAR '@'
#define HEAD 'O'

#define CMDS 7

#define REDO 'r'
#define QUIT 'q'
#define PAUS 'p'

#define FORE 'w'
#define BACK 's'
#define LEFT 'a'
#define RITE 'd'

// A node in the linked list
struct node {
	void *value;
	struct node* next;
};
	// The linked list structure
struct plist {
	struct node* head;
};
// Prepends a new node with the given value to the list
void prepend(struct plist* plist, void *value) {
	struct node* new_node = malloc(sizeof(struct node));
	new_node->value = value;
	new_node->next = plist->head;
	plist->head = new_node;
}
// Removes the first node from the list and returns its value
void *pop(struct plist* plist) {
	if (plist->head == NULL) {
		return 0;
  	}
  	if (plist->head->next == NULL) {
    		// Special case: list has only one element
    		void *value = plist->head->value;
    		free(plist->head);
    		plist->head = NULL;
    		return value;
  	}
  // Find the second-to-last element
  	struct node* current = plist->head;
  	while (current->next->next != NULL) {
    		current = current->next;
  	}
  	// Remove the last element
  	void *value = current->next->value;
  	free(current->next);
  	current->next = NULL;
  	return value;
}
// Returns the value at the given index in the list
void *pindex(struct plist* plist, int index) {
	struct node* current = plist->head;
	for (int i = 0; i < index; i++) {
		if (current == NULL) {
			return 0;
		}
		current = current->next;
	}
	return current->value;
} 

int size(struct plist* plist) {
  	int size = 0;
  	struct node* current = plist->head;
  	while (current != NULL) {
    		size++;
    		current = current->next;
  	}
  	return size;
}

int search(struct plist* plist, void *value) {
    	struct node *current = plist->head;
    	while (!(current)) {
        	if(current->value == value) {
            		return 1;
        	}
        	current = current->next;
    	}
  	return 0;
}
int make_scr(struct plist *p){
	//int food = 1;
	//if (!food) {pop(p);}
	move(0, 0);
	
	initscr();
	
	refresh();
	
	char line[WIDE + 2]; // 80 printable characters + new line + null terminator
	
	line[0       ] = '|';
	line[WIDE - 1] = '|';
	line[WIDE    ] = '\n';
	line[WIDE + 1] = '\0';	
		
	putchar('\n');
	
	for ( int i = 0 ; i < HIGH ; i ++ ) 
	{
		for ( int j = 1 ; j < WIDE - 1 ; j ++ )
		{
			/*int list[2];
			list[0], list[1] = i, j;*/
			line[j] = ' ';
			if (i == 0 || i == HIGH - 1) { // top or bottom
				line[j] = '-';
			} if (i == (HIGH/2) && j == (3*WIDE/4)) { // food
				line[j] = CHAR;
			} else if (0) { // snek
				line[j] = HEAD;
			}
		}
		printw("%s",line);
		refresh();
	}
	return 0;
}

int snek_move(char dirc, struct plist *p) {
	int list[2];
	int *head = (int *)&p->head->value;
	if (head[0] == 2 || head[0] == HIGH - 1 || head[1] - WIDE/4 == 1 || head[1] - WIDE/4 == WIDE - 2) {
		printw("You have hit a wall, press r to restart");
		refresh();
	} else if (FORE == dirc) {
		list[0] = head[0]--;
		list[1] = head[1];
		prepend(p, list);
	} else if (BACK == dirc) {
		list[0] = head[0]++;
		list[1] = head[1];
		prepend(p, list);
	} else if (LEFT == dirc) {
		list[0] = head[0];
		list[1] = head[1]--;
		prepend(p, list);
	} else if (RITE == dirc) {
		list[0] = head[0];
		list[1] = head[1]++;
		prepend(p, list);
	} else {
		return 0; // quit || error case
		endwin();
	}
	make_scr(p);
}

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
		
		int xety[2];
		xety[0] = (HIGH/2);
		xety[1] = (WIDE/2);
				
		struct plist *p;
		p->head = NULL;
		prepend(p, xety);
		
		//make_scr(p);
		while (1) {
			fcntl(c_sock, F_SETFL, fcntl(c_sock, F_GETFL) | O_NONBLOCK);
            		sleep(1);
            		char *incoming[BUFF];
            		int numRead = read(c_sock, incoming, sizeof(incoming));
            		if (numRead > 0) {
                		fcntl(c_sock, F_SETFL, fcntl(c_sock, F_GETFL) & ~O_NONBLOCK);
                		printw("You said: %s", (char *)incoming);
                		refresh();
            		}
            		if ((char)incoming[0] == QUIT) { 
            			endwin();
            			return 0;
            		}
            		if ((char)incoming[0] == REDO) {
            			WINDOW *win;
				refresh();
              			xety[0] = (HIGH/2);
				xety[1] = (WIDE/2);
            			make_scr(p);
               		}
            		//snek_move((char)incoming[0], p);
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

		char buff[16];
		
		while (1) {
			fgets(buff, sizeof(buff), stdin);
			write(sock, buff, strlen(buff));
			if ((char)buff[0] == QUIT) {exit(-1);}	
		}
	}

	if (argv[1][1] == 'h')
	{
		printf("HELP: Usage:  -s for server, -c for client\n");
	}

    return 0;
}




