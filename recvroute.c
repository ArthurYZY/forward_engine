#include "recvroute.h"

int static_route_get(struct selfroute *selfrt)
{
	int sock_fd; 
	struct sockaddr_in server_addr;
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&server_addr, sizeof(struct sockaddr_in)); 
	server_addr.sin_family = AF_INET; 
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	server_addr.sin_port = htons(800);

	bind(sock_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)); 
	listen(sock_fd, 5);
	printf("----------------------------------\n");

	int ret = -1;
	while (1) { 
		printf("***\n");
		int conn_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL);
		ret = recv(conn_fd, &selfrt, sizeof(struct selfroute), 0); 
		printf("%d\n", ret);
		if(ret >= 0){
			return 0;
		}
		close(conn_fd);
	}

}
