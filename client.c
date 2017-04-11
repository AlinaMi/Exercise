#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define B_SIZE 1024

int main(int argc)
{
  /* local variables declaration */
  int clientSocket, status, addr_size;
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

  /* update sockaddr fields */
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(5000);  // how to transmit server port to client?  
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  /* get sockaddr size */
  addr_size = sizeof serverAddr;

  /* connect to server */
  status = connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);

  if(status < 0)
  {
    printf("Error %d in connecting to server socket!\n", errno);
  }

  /* get file name to be opened from keyboard */
  while(fgets(name, 100, stdin) != NULL)
  {
      printf("Send a request!\n");
      
      /* copy file name to buffer */
      strncpy(buffer, name, B_SIZE);

      /* send file name to server */
      status = send(clientSocket, buffer, B_SIZE, 0);

      if(status < 0)
      {
        printf("Error %d in sending the file name!\n", errno);
      }

      /* set buffer to 0 */
      memset(buffer, 0, sizeof(buffer));

      /* receive data from server */
      status = recv(clientSocket, buffer, B_SIZE, 0);

      if(status < 0)
      {
       printf("Error %d in receiving data from server!\n", errno);
      }

      /* specify the file name where the data will be written */
      sprintf(file_name, "%s_r", name);

      /* open file to be written in text mode */
      file_1 = fopen(file_name, "wt");
  
      if(file_1 == NULL)
      {
        printf("Error in openining the file!\n");
      }

      /* write data received from server to file_r */
      fwrite(buffer, 1, (strlen(buffer)-1), file_1);

      printf("Received data: %s", buffer);   
  
      memset(buffer, 0, sizeof(buffer));

      /* close new created file */
      fclose(file_1);
  }

return 0;
}
