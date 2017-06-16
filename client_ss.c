#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

#define B_SIZE 1024

int main(int argc, char* argv[])
{
  /* local variables declaration */
  int clientSocket, status, addr_size,port;
  char buffer[B_SIZE], file_name[100];
  char *name;
  struct sockaddr_in serverAddr;
  FILE *file_1;
  
  /* create TCP, stream client socket */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  if(clientSocket < 0)
  {
    printf("Error %d in creating client socket!\n", errno);
    return 1;
  }
 
   /* initialise sockaddr and buffer to 0 */
  memset(&serverAddr, 0, sizeof(serverAddr));
  memset(buffer, 0, sizeof(buffer));

  /* chceck that the port number was given */
  if(argc !=2)
  {
      printf("Please specify the port number!\n");
      return 1;
  }

   /* store and convert port number */
  port = atoi(argv[1]);

  /* update sockaddr fields */
  serverAddr.sin_family = AF_INET;
    // how to transmit server port to client?  
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* get sockaddr size */
  addr_size = sizeof serverAddr;

  serverAddr.sin_port = htons(port);
  /* connect to server */
  status = connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);

  if(status < 0)
  {
    printf("Error %d in connecting to server socket!\n", errno);
  }

  /* get file name to be opened from keyboard */
  while(1)
  { 
      fgets(name, 100, stdin);
      printf("Send a request!\n");
      
      /* copy file name to buffer */
      strncpy(buffer, name, B_SIZE);

      /* send file name to server */
      status = send(clientSocket, buffer, B_SIZE, 0);

      if(status < 0)
      {
        printf("Error %d in sending the file name!\n", errno);
        return 1;
      }

      /* set buffer to 0 */
      memset(buffer, 0, sizeof(buffer));

      /* receive data from server */
      status = recv(clientSocket, buffer, B_SIZE, 0);

      if(status < 0)
      {
       printf("Error %d in receiving data from server!\n", errno);
       return 1;
      }

      /* specify the file name where the data will be written */
      snprintf(file_name, 100, "%s_r", name);

      /* open file to be written in text mode */
      file_1 = fopen(file_name, "wt");
  
      if(file_1 == NULL)
      {
        printf("Error in openining the file!\n");
        return 1;
      }

      if(buffer != NULL)
      {
        /* write data received from server to file_r */
        fwrite(buffer, 1, (strlen(buffer)-1), file_1);

        printf("Received data: %s\n", buffer);   
      }

      memset(buffer, 0, sizeof(buffer));

      /* close new created file */
      fclose(file_1);
  }
return 0;
}
