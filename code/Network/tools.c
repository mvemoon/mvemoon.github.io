#include "tools.h"

int open_clientfd(char *hostname, char *port)
{
		int clientfd;
		struct addrinfo hints, *listp, *p;

		/* Get a list of potential server address*/
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_NUMERICHOST | AI_ADDRCONFIG;
		getaddrinfo(hostname, port, &hints, &listp);

		/* Walk the list for one that we can successfully connect to */
		for(p = listp; p; p =  p->ai_next)
		{
			if((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0 )
				continue;
			
			/* connet to the server */
			if(connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
				break;/* successfully connect*/

			close(clientfd);
		}

		/* Clean up */
		freeaddrinfo(listp);
		if(!p)
			return -1;
		else 
			return clientfd;
}


int open_listenfd(char *port)
{
	
}