#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LISTEN_PORT 1337
#define SIZE 500
#define SMALLSIZE 200
#define RESULTSIZE 40
#define SUBSIZE 5
#define PORT 1337

void recv_loop(int csock){
	char buf[SIZE];
	char *flag = &buf[409];
	char substr[SUBSIZE];
	char result[RESULTSIZE];
  char *newbuf = &buf[201];
  char resultsub[20];
	int position;
	int diff;
  struct sockaddr_in caddr;
  int *pcsock = &csock;
  while(1){
		memset(buf, '\0', SIZE);
		memset(substr, '\0', SUBSIZE);
		memset(result, '\0', RESULTSIZE);
    memset(resultsub, '\0', 20);
		send(*pcsock, "Please enter the string (max 200 char) sent to the buffer:\n", 60, 0);
		recv(*pcsock, buf,SMALLSIZE  ,0);
		send(*pcsock, "Please enter what showed up in EIP:\n", 37, 0);
		recv(*pcsock, substr ,SUBSIZE ,0);
		substr[strlen(substr)-1] = '\0';
    sprintf(newbuf,buf);
    strncpy(resultsub, newbuf, 10);
    resultsub[11]='.';
    resultsub[12]='.';
    resultsub[13]='.';
    resultsub[14]='\n';
    sprintf(flag,"flag{us3-a-4m@t-StR1nG-ANd-bRUt3f0rc3}");	

    send(*pcsock, "Searching buffer:\n",19,0);
    send(*pcsock, resultsub,20,0);

		if (strstr(newbuf, substr)== NULL){
			send(*pcsock, "Substring not found\n",20,0 );
		}
		else{
			position = strstr(newbuf, substr);
			diff = position - (int) newbuf;	
			if (diff > 200){
				send(*pcsock, "Out of range read\n",18,0);
			} else {
				snprintf(result,RESULTSIZE,"Substring found at position %d\n", diff);
				send(*pcsock, result, RESULTSIZE, 0);
			}
				

    }
		}

}	

int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size =sizeof(newAddr);

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", PORT);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

    recv_loop(newSocket);
		}

	}

	close(newSocket);


	return 0;
}