#include <stdio.h> 
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
    
#define TRUE   1 
#define FALSE  0 


int master_socket , addrlen , new_socket , client_socket[30] , 
          max_clients = 30 , activity, i , valread , sd, max_server = 3, server_socket[3];  
int max_sd;  
fd_set readfds;
int clientListToBeServed[300];
int toBeServed = 0;
int serving;
int serverFileDiscripter[3];
char msgToClient[30]="Enter <Acc No>,<Name>,<Operation>,<Balance>";
char replicate[] = "replicate";
int replicationFlag = 0;
void add_master(){
    //add master socket to set 
    FD_SET(master_socket, &readfds);  
    max_sd = master_socket;  
} 

void add_client(){
    //add child sockets to set 
        for ( i = 0 ; i < max_clients; i++){  
            //socket descriptor 
            sd = client_socket[i];  
              
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }

        for ( i = 0 ; i < max_server; i++){  
            //socket descriptor 
            sd = server_socket[i];  
              
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }   
}

void add_clientToClientPool(int new_socket){
    printf("Client added to client Pool");
    for (i = 0; i < max_clients; i++){  
        //if position is empty 
        if( client_socket[i] == 0 ){  
            client_socket[i] = new_socket;  
            printf("Adding to client socket list %d\n" , i);  
            break;  
        }  
    }  
}
  
void add_serverToServerPool(int new_socket){
    printf("Added to server Pool\n");
    for (i = 0; i < max_server; i++){  
        //if position is empty 
        if( server_socket[i] == 0 ){  
            server_socket[i] = new_socket;  
            printf("Adding to list of sockets as %d\n" , i);  
            break;  
        }  
    }  
}  




int main(int argc , char *argv[]){  

    //---------------------------INITIALIZING------------------------------
   
    //int opt = TRUE;  
    struct sockaddr_in address;  
    char *buffer;  //data buffer of 1K 
    char string[200];
    char storeName[200];
    //set of socket descriptors 
    // fd_set readfds;  
    int serverCount = 0;     
    //a message 
    char *message = "ECHO Daemon v1.0 \r\n";  
//-----------------------------------------------------------

    int commaCount = 0;
    char* name;
    char* accNo;
    char* operation;
    char* balance;
    int arrayPosition  = 0;  
    int flag = 0;

    char c;
    int numBytesSent;
//-----------------------------------------------------------    
    //check for the valid arguments
    if (argc != 2){
        //DieWithUserMessage("Parameter(s)", "<Server Port>");    
    } 

    in_port_t PORT = atoi(argv[1]);

    
    //initialise all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++)  
    {  
        client_socket[i] = 0;  
    }  
        
    //create a master socket 
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
    
   
    
    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );  
        
    //bind the socket to localhost port 8888 
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", PORT);  
        
    //try to specify maximum of 3 pending connections for the master socket 
    if (listen(master_socket, 3) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
        
    //accept the incoming connection 
    addrlen = sizeof(address);  
    puts("Waiting for connections ...\n");  
        
    while(TRUE){  
  //      puts("Loop");
        FD_ZERO(&readfds);  
        add_master();
        add_client();
        printf("while 1 loop entered");
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
      
        if ((activity < 0) && (errno!=EINTR)){  
            printf("select error");  
        }  

        if (FD_ISSET(master_socket, &readfds)){  
            puts("connecting\n");
         
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){  
                perror("accept\n");  
                exit(EXIT_FAILURE);  
            }  


            strcpy(string,inet_ntoa(address.sin_addr));
            
            //-------------------------------------------------------------------------------------------
            if(strcmp(string,"192.168.0.5") == 0){
                serverCount++;
                printf("New server detected \n");
                printf("server count = %d \n",serverCount);

                //----------------------REPLICATION -------------------------
               
                // if(serverCount > 1){
                    // replicationFlag = 1;
                    //replication if more than one server is connected
                    // printf("replication process starting\n");
                    // printf("replication flag turned on!!!!");
                    //get name from namelist
                    // int numBytesSent = send(server_socket[0],"*,rajan,*,replicate",sizeof("*,rajan,*,replicate"),0);
                    // printf("replication request sent to the server to the server descriptor %d \n",server_socket[0]);
                    // sleep(3);
/*      
                    FILE *f = fopen("namelist.txt", "r");      // open the specified file
                    if (f != NULL){  
                        strcpy(storeName,"");
                        int i=0;
                        char comma[100]="*,";
                        // read character from file until EOF
                        //accno,name,balance,operation(*,storename,*,replication)
                        while ((c = fgetc(f)) != EOF){
                            if(c!='\n'){
                                storeName[i++]=c;  
                            }else{
                                storeName[i] = '\0';
                                i = 0;
                                printf("\n %s \n",storeName);
                                for(int j = 0; j < max_server; j++){
                                    if(server_socket[j] != 0){
                                        strcat(comma,storeName);
                                        strcat(comma,",*,");
                                        strcat(comma,"replicate");
                                        printf("message = %s \n",comma);
                                        numBytesSent = send(server_socket[j], comma, sizeof(comma), 0);
                                        printf("send message for replication");
                                        strcpy(comma,"");
                                        strcpy(comma,"*,");
                                        break;    
                                        // if ((valread = read( sd , buffer, 25)) == 0){
                                        // }  
                                    }

                                }
                            }
                        }
                        
                        fclose(f);
                    }*/

                // }
                //------------------------------------------------------------
                add_serverToServerPool(new_socket);
               
 
            }else{
                printf("New client connected \n");
                printf("This is the string: %s \n",string);
                printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                add_clientToClientPool(new_socket);
            }

            //-------------------------------------------------------------------------------------------
            puts("test");
        for(int j = 0 ; j <max_server ; j++){
            if(server_socket[j] != 0)
            printf("after adding server sockets : %d \n",server_socket[j]);
        }
        for(int j = 0 ; j <max_clients ; j++){
            if(client_socket[j] != 0)
            printf("after adding client sockets : %d \n",client_socket[j]);
        }
        }

        

        name = (char *)malloc(10);
        accNo = (char *)malloc(10);
        balance = (char *)malloc(10);
        operation = (char *)malloc(10);
        bzero(name,sizeof(name));
        bzero(accNo,sizeof(accNo));
        bzero(balance,sizeof(balance));
        bzero(operation,sizeof(operation));
        commaCount = 0;    

       
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++){  
            sd = client_socket[i];  

            if (FD_ISSET( sd , &readfds)){  
                //puts("entered \n");
                buffer=(char *)malloc(25);
                if ((valread = read( sd , buffer, 25)) == 0){
                    puts("Closing Client\n");
                    close( sd );  
                    client_socket[i] = 0;  
                }else{  
                    clientListToBeServed[toBeServed] = sd;
                    printf("next client to be served is : %d \n",clientListToBeServed[toBeServed]);
                    toBeServed++;
                    puts(buffer);
                    printf("After read buffer:%s,%d\n",buffer,strlen(buffer));
                    //puts("test");

                    if(strcmp(string,"192.168.0.5") != 0){
                        printf("reading \n");
                       /*------------------------------------------------------------*/
                        
                        //read the input message to decide if need to add to the namelist
                        for(int i = 0; i <=strlen(buffer);i++){
                            //printf("buffer : %c",buffer[i]);

                            if(buffer[i] == ',' || buffer[i] == '\0'){
                                commaCount++;
                                arrayPosition = 0;
                            }else{
                                if(commaCount == 0){
                                    accNo[arrayPosition] = buffer[i];
                                    arrayPosition++;
                                }else if(commaCount == 1){
                                    name[arrayPosition] = buffer[i];
                                    arrayPosition++;
                                    
                                }else if(commaCount == 2){
                                    balance[arrayPosition] = buffer[i];
                                    arrayPosition++;
                                                        
                                }else{
                                    operation[arrayPosition] = buffer[i];
                                    arrayPosition++;
                                    
                                }
                                

                            }   

                        }

                        //check if the client has requested to add new acc and then add to the namelist
                        if(operation[0]=='a'){
                            FILE *f = fopen("namelist.txt", "a");
                            if (f == NULL){
                                printf("Error opening file!\n");
                                exit(1);
                            }
                            fprintf(f,"%s\n",name);
                            fclose(f);
                        }
                        /*------------------------------------------------------------*/

                        printf("sending message to the server\n");

                        //-------send the message received to all the servers
                        for(int i = 0; i < max_server; i++){
                            printf("serverCount = %d \n",serverCount);
                            printf("sending loop \n");
                            if(server_socket[i] != 0)
                            printf("buffer:%s \tsize of Buffer:%d",buffer,strlen(buffer));
                            send(server_socket[i], buffer , strlen(buffer) , 0 );
                            printf("sending client message to the server");
                            puts(buffer);
                            //perror("error sending");
                        }
                         
                    }  
                }
            }  
        }



        for (i = 0; i < max_server; i++){  
            sd = server_socket[i];  

            //printf("sd:%d\n",sd);
            //printf("server socket id : %d \n", server_socket[i]);
            if (FD_ISSET( sd , &readfds)){  
                puts("entered \n");
                //Check if it was for closing , and also read the 
                //incoming message 
                //strcpy(buffer,"");

                //------------------------------------------------------------------
                //if(replicationFlag == 0){
                    if ((valread = read( sd , buffer, 1024)) == 0){
                       // puts(buffer);
                        puts("closing server\n");
                        close( sd );  
                        server_socket[i] = 0;
                        for(int j = 0 ; j < max_server; j++){
                            if(server_socket[j] != 0){
                                printf("server_socket = %d \n",server_socket[j]);
                            }
                        } 
                        serverCount--;
                        printf("serverCount after reduction = %d \n",serverCount);
                        for(int j = 0 ; j <3 ; j++){
                            printf("after deleting server descriptor : %d \n",server_socket[j]);
                        } 

                    }else{
                        //puts("test for core dump");
                       // puts(buffer);

                        printf("reply to query received!!!\n");  
                        printf("received Buffer:%s , i : %d \n",buffer,i);
                        printf("message replying to client at descriptor %d \n",clientListToBeServed[serving]);
                        send(clientListToBeServed[serving], buffer , strlen(buffer) , 0 );
                        printf("send server message to client\n");
                        serving++;
                        printf("message replied to the client:\n at descriptor %d\n",serving);
                        printf("sent Buffer:%s\n",buffer); 
                        strcpy(buffer,"");    
                        
                           
                    }
                   

                
                /*    
                else{

                    printf("Replication = 1 so this loop is running");
                     if ((valread = read( server_socket[0] , buffer, 1024)) == 0){
                       // puts(buffer);
                        puts("closing server\n");
                        close( sd );  
                        server_socket[i] = 0;
                        for(int j = 0 ; j < max_server; j++){
                            if(server_socket[j] != 0){
                                printf("server_socket = %d \n",server_socket[j]);
                            }
                        } 
                        serverCount--;
                        printf("serverCount after reduction = %d \n",serverCount);
                        for(int j = 0 ; j <3 ; j++){
                            printf("after deleting server descriptor : %d \n",server_socket[j]);
                        } 

                    }else if (valread > 0){
                        puts("test for core dump");
                       // puts(buffer);

                        // printf("reply to query received!!!\n");  
                        // printf("received Buffer:%s , i : %d \n",buffer,i);
                        // printf("message replying to client at descriptor %d \n",clientListToBeServed[serving]);
                        // send(clientListToBeServed[serving], buffer , strlen(buffer) , 0 );
                        // printf("send server message to client\n");
                        // serving++;
                        // printf("message replied to the client:\n at descriptor %d\n",serving);
                        // printf("sent Buffer:%s\n",buffer); 
                        // strcpy(buffer,"");    
                        
                           
                    }
                    else{
                        puts("in second loop less than 0 detected");
                        sleep(5);
                    }
                }*/
                
                
            } 
        }
    }  
    return 0;  
}  

