#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 1025

void DieWithMessage(char *errorMessage);

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in echoServAddr;
	unsigned short echoServPort;
	char *servIP;
	char *echoString;
	char echoBuffer[RCVBUFSIZE];
	unsigned int echoStringLen;
	int bytesRcvd,totalBytesRcvd;
	char message[100];

	if((argc < 3)|| (argc >4)){
		fprintf(stderr, "Usage: %s <Server IP>n <Echo Word> [<Echo Port]\n",argv[0]);
		exit(1);	
	}

	servIP = argv[1];
	echoString = argv[2];
	
	if(argc == 4)
		echoServPort = atoi(argv[3]);
	else
		echoServPort = 7;

	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
		DieWithSystemMessage("socket() failed");

	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);
	// echoServAddr.sin_port = htons(echoServPort);
	echoServAddr.sin_port = htons(echoServPort);

	if(connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr))<0)
		DieWithSystemMessage("connect() failed");

	echoStringLen = strlen(echoString);
	//printf("echoStringlen:%s,%d",echoString,echoStringLen);
	//puts("test");
	//for(int i = 0; i < 5; i++){
        strcpy(message,"");

		printf("Enter the details(Enter <Acc No>,<Name>,<Balance>,<Operation>) seperated by comma\n");
		scanf("%s",message);	
		puts(message);
		if(send(sock, message, strlen(message), 0) != strlen(message))
			DieWithSystemMessage("send() sent a different number of bytes than expected");	

		puts(message);
		// if(send(sock, echoString, echoStringLen, 0) != echoStringLen)
		// 	DieWithSystemMessage("send() sent a different number of bytes than expected");	

		printf(" after send echoStringlen:%s,%d \n",echoString,echoStringLen);
		puts("test");


		printf("waiting to receive reply\n");
		if((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE, 0)) <=0)
			DieWithSystemMessage("recv() failed or connection closed permaturely");
			// totalBytesRcvd += bytesRcvd;
			// echoBuffer[bytesRcvd] = '\0';
			printf("Received message :\n %s \n", echoBuffer);


	/*
		totalBytesRcvd = 0;
		printf("Received: ");
		while(totalBytesRcvd < echoStringLen){
			if((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE-1, 0)) <=0)
				DieWithSystemMessage("recv() failed or connection closed permaturely");
			totalBytesRcvd += bytesRcvd;
			echoBuffer[bytesRcvd] = '\0';
			printf("Received message : %s", echoBuffer);
			
		}
	*/
		
		sleep(3);
	//}
	

	close(sock);
	exit(0);


}
