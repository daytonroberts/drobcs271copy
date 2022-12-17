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
#define HIGH 12
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
/*struct node {
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
}*/

int make_scr(int *xety, WINDOW *win, int *food){
	/*if (search(p, food)) {
        	while(search(p, food)) {
            		food[0] = (rand() % HIGH - 2) + 1;
            		food[1] = (rand() % WIDE - 2) + 1;
       		}
    	}*/
	
	werase(win);
	wrefresh(win);
	
	move(0, 0);
	
	initscr();
	
	refresh();
	
	char line[WIDE + 2]; // 80 printable characters + new line + null terminator
	
	line[0       ] = '#';
	line[WIDE - 1] = '#';
	line[WIDE    ] = '\n';
	line[WIDE + 1] = '\0';	
		
	putchar('\n');
	
	for ( int i = 0 ; i < HIGH ; i ++ ) 
	{
		for ( int j = 1 ; j < WIDE - 1 ; j ++ )
		{
			line[j] = ' ';
			if (i == 0 || i == HIGH - 1) { // top or bottom
				line[j] = '#';
			} if (i == xety[0] && j == xety[1]) { // snek
				line[j] = HEAD;
			} else if ((i == (int)food[0]) && (j == (int)food[1])) { // food
				line[j] = CHAR;
			} 
		}
		printw("%s",line);
		refresh();
	}
	return 0;
}

int snek_move(char dirc, int *xety, WINDOW *win, int *food) {
	printw("%d, %d\n", xety[0], xety[1]);
	if ((xety[0] == 2 && dirc == FORE) || (xety[0] == HIGH - 2 && dirc == BACK) || (xety[1] == 1 && dirc == LEFT) || (xety[1] == WIDE - 2 && dirc == RITE)) {
		//printw("You have hit a wall, press r to restart");
		refresh();
        } else if (FORE == dirc) {
			xety[0]--;
	} else if (BACK == dirc) {
			xety[0]++;
	} else if (LEFT == dirc) {
			xety[1]--;
	} else if (RITE == dirc) {
			xety[1]++;
	} else {
		return 0; // quit || error case
		endwin();
	}
	while (xety[0] == food[0] && xety[1] == food[1]) {
		srand(time(0));
		food[0] = (rand() % (HIGH - 2 - 2)) + 2;
        	food[1] = (rand() % (WIDE - 2 - 1)) + 1;
	}
	make_scr(xety, win, food);
	
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
		
		int incoming[sizeof(QUIT)];
		
		int xety[2];
		xety[0] = (HIGH/2);
		xety[1] = (WIDE/2);
		
		int food[2];
		srand(time(0));
		food[0] = (rand() % (HIGH - 2 - 2)) + 2;
        	food[1] = (rand() % (WIDE - 2 - 1)) + 1;
		
		WINDOW *win;
		make_scr(xety, win, food);
		while (1) {
			fcntl(c_sock, F_SETFL, fcntl(c_sock, F_GETFL) | O_NONBLOCK);
            		sleep(1);
            		int numRead = read(c_sock, incoming, sizeof(incoming));
            		if (numRead > 0) {
                		fcntl(c_sock, F_SETFL, fcntl(c_sock, F_GETFL) & ~O_NONBLOCK);
                		//printw("You said: %s", (char *)incoming);
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
            			make_scr(xety, win, food);
               		}
            		snek_move((char)incoming[0], xety, win, food);
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
			if ((char)buff[0] == QUIT) {exit(-1);}	
		}
	}

	if (argv[1][1] == 'h')
	{
		printf("HELP: Usage:  -s for server, -c for client\n");
	}

    return 0;
}




