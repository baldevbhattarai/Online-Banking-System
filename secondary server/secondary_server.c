#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 300
char returnstring[100];

void DieWithMessage(char *errorMessage);

void QueryAcc(char* name){
	int i = 0;
    char string[1025];
	FILE *f = fopen(name, "r");      // open the specified file
		 if (f != NULL){
			  char c;

			while ((c = fgetc(f)) != EOF){     // read character from file until EOF
				string[i++]=c;  
			}
		 	strcpy(returnstring,string);
 			fclose(f);
	  		}
	  
	printf("\nstring :%s \n returnstring:%s\n",string,returnstring);
}


void NewAcc(char *accNo, char *name, char *balance){
	printf("New Acc function\n");
	// printf("Account Number:%s \n Name:%s \n Balance:%s \n ",accNo,name,balance);
	FILE *f = fopen(name, "w");
	if (f == NULL){
		printf("Error opening file!\n");
		exit(1);
	}
	//fputs("%s",);
	//fputs(name,f);
	fprintf(f, "Account Number:%s \n Name:%s \n Balance:%s \n ",accNo,name,balance);
	fclose(f);
	//puts(echoBuffer);
	//memset(echoBuffer, 0, sizeof(echoBuffer));
}

void Update(char *accNo,char *name,char *balance,char *operation){
	printf("Update function entered");
	int colonCount = 0;
	char string[50];
	int i = 0;
	int intValue = 0;
	int balanceValue = 0;
	int newBalance = 0;
	if(strcmp(operation,"deposit")==0){
		printf("deposit function entered\n");
		FILE *f = fopen(name, "r");
		if (f != NULL){
		    char c;
			colonCount=0;
			// read character from file until EOF
	        while ((c = fgetc(f)) != EOF){ 

	        	//printf("%c",c);
			 	if(c==':'){
					colonCount++;
				}else{
				if(colonCount==3){
				  string[i]=c;
				  i++;
				 }
				}
			}
			string[i] = '\0';
			printf("\nstring : %s\n", string);
			// intValue = atoi(string);
			// printf("IntValue  = %ld",intValue);
			//--------------------------------------------------

			printf("the val is : %d\n", atoi(string));
			printf("the new balance is = %d", atoi(string) + atoi(balance));
			newBalance = atoi(string) + atoi(balance);
			fclose(f);
			f = fopen(name, "w");

			puts("    ");
			fprintf(f,"Account Number:%s \n Name:%s \n Balance:%d \n ",accNo,name,newBalance);

			//printf("the integer value");
		    //ssize_t numBytesSent = send(clntSocket, string, sizeof(string), 0);
  			fclose(f);
		}
	}else if(strcmp(operation,"withdraw")==0){
     printf("withdraw function entered\n");
		FILE *f = fopen(name, "r");
		if (f != NULL){
		    char c;
			colonCount=0;
			// read character from file until EOF
	        while ((c = fgetc(f)) != EOF){ 

	        	//printf("%c",c);
			 	if(c==':'){
					colonCount++;
				}else{
				if(colonCount==3){
				  string[i]=c;
				  i++;
				 }
				}
			}
			string[i] = '\0';
			printf("\nstring : %s\n", string);
			// intValue = atoi(string);
			// printf("IntValue  = %ld",intValue);
			//--------------------------------------------------

			printf("the val is : %d\n", atoi(string));
			//puts("string");
			//printf("the integer value");
		    //ssize_t numBytesSent = send(clntSocket, string, sizeof(string), 0);
  			fclose(f);
		}
    }

}

//void replicate

void replicate(char *name){
  //try to delete the old file
  // if (remove("abc.txt") == 0){
  //     printf("Deleted successfully");
  // }
  //  	else{
  //     printf("Unable to delete the file");
  //  	}
	printf("Replicate function entered");
	QueryAcc(name);


}	

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in echoServAddr;
	unsigned short echoServPort;
	char *servIP;
	char *echoString;
	char *echoBuffer;
	unsigned int echoStringLen;
	int bytesRcvd,totalBytesRcvd;
	char inputString[300];
	int commaCount = 0;
	char* name;
	char* accNo;
	char* operation;
	char* balance;
	int arrayPosition  = 0;  
	int flag = 0;
	int numBytesSent;

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
	echoServAddr.sin_port = htons(echoServPort);

	if(connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr))<0)
		DieWithSystemMessage("connect() failed");
	puts("connected");

	//echoStringLen = strlen(echoString);

	//for(int i = 0; i < 5; i++){
	echoBuffer = (char *)malloc(40);

	while(1){
		name = (char *)malloc(30);
		accNo = (char *)malloc(30);
		balance = (char *)malloc(30);
		operation = (char *)malloc(30);
		bzero(name,sizeof(name));
		bzero(accNo,sizeof(accNo));

		bzero(balance,sizeof(balance));

		bzero(operation,sizeof(operation));
		commaCount = 0;
		strcpy(echoBuffer,"");
		strcpy(operation,"");
		// bze(accNo,"");
		// strcpy(balance,"");
		// strcpy(operation,"");

		//printf("before recv: %s \n", echoBuffer);
		if((bytesRcvd = recv(sock, echoBuffer, 1024, 0)) <=0){
			DieWithSystemMessage("recv() failed or connection closed permaturely\n");
			puts("test");
			exit(1);
		}	

		printf("after recv: %s\n", echoBuffer);
		printf("echobuffer length: %d\n",strlen(echoBuffer));
		
		for(int i = 0; i <=strlen(echoBuffer);i++){
			//printf("echobuffer : %c",echoBuffer[i]);

			if(echoBuffer[i] == ',' || echoBuffer[i] == '\0'){
				commaCount++;
				arrayPosition = 0;
			}else{
				if(commaCount == 0){
					accNo[arrayPosition] = echoBuffer[i];
					arrayPosition++;
				}else if(commaCount == 1){
					name[arrayPosition] = echoBuffer[i];
					arrayPosition++;
					
				}else if(commaCount == 2){
					balance[arrayPosition] = echoBuffer[i];
					arrayPosition++;
										
				}else if(commaCount == 3){
					operation[arrayPosition] = echoBuffer[i];
					arrayPosition++;
				}else {
					exit(0);
				}
			}	
		}		

		accNo[strlen(accNo)+1] = '\0';
		name[strlen(name)+1]='\0';
		balance[strlen(balance)+1]='\0';

		//printf("After ...............\n Account Number:%s \n Name:%s \n Balance:%s \n Operation:%s \n ",accNo,name,balance,operation);

						
		printf("\n");
		printf("Operation : %s \n",operation);
		switch(operation[0]) {

			case  'q':
				QueryAcc(name);
				puts(returnstring);
				numBytesSent = send(sock, returnstring, sizeof(returnstring), 0);
				printf("messaged replied for the query\n");
				//puts(returnstring);
				//printf("reply to query sent!! %s \n",returnstring);
				//strcpy(returnstring,"");
				break; /* optional */

			case 'a':
				NewAcc(accNo,name,balance);
				numBytesSent = send(sock, "Successfully Added!!!\n", sizeof("Successfully Added"), 0);
				printf("message sent\n");
				break; /* optional */

		    case 'd':
		    	Update(accNo,name,balance,operation);
				break; /* optional */

		    case 'w':
		    	//Update();

		    case 'r':
		    	replicate(name);
		    	char *newstring="Duplicate data sent successfully";
				numBytesSent = send(sock, newstring, sizeof(newstring), 0);
				printf("message sent\n");
		    	break;
		   
		    default : /* Optional */
			printf("no such operation available \n");
	    }
		
	}
	printf("\n");	
	
	

	close(sock);
	exit(0);



}