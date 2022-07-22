/*
 mbot Copyright (C) <2016> <matthias holl>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>



 mbot.c sends http requests

 www.mahoosoft.com

 linux:

 gcc -o mbot mbot.c

 sudo ./mbot hostname

 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#define BUF_SEND 4096
#define BUFF_RECV 4096
int main(int argc, char **argv) {
	int sock;
	int x;
	int size;
	int requests;
	char *buffer_send = malloc(BUF_SEND);
	char *buffer_recv = malloc(BUFF_RECV);
	char GET[1000] = "GET / HTTP/1.1\nHost: ";
	char GET2[1000] = "\nUser-Agent:mahoobot\n\r\n\r";
	struct sockaddr_in dest_address;
	struct hostent *host;
	if (argc < 2) {
		printf("Usage: %s host\n", *argv);
		exit(EXIT_FAILURE);
	}
	/* set number of http requests */
	printf("number of requests:");
	scanf("%d", &requests);
	for (x = 1; x <= requests; x++) {
		/* create a socket */
		sock = socket(AF_INET, SOCK_STREAM, 0);
		/* resolve host by name from bash */
		host = gethostbyname(argv[1]);
		/* server ip-address */
		memcpy((char *) &dest_address.sin_addr, host->h_addr, host->h_length );
		/* set protocol */
		dest_address.sin_family = AF_INET;
		/* set port */
		dest_address.sin_port = htons(80);
		/* connect to dest host */
		connect(sock, (struct sockaddr *) &dest_address, sizeof(dest_address));
		printf("connected: (%s) \n", argv[1]);
		sleep(2);
		/* concatenate the strings */
		strcat(GET, argv[1]);
		strcat(GET, GET2);
		buffer_send = GET;
		/* send http requests */
		send(sock, buffer_send, strlen(buffer_send), 0);
		/* receive server response */
		size = recv(sock, buffer_recv, BUFF_RECV - 1, 0);
		if (size > 0)
			buffer_recv[size] = '\0';
		printf("message from server: %s\n", buffer_recv);
	}
	/* close socket */
	close(sock);
	return EXIT_SUCCESS;
}

