#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>

#define B_SIZE 1024

int main(int argc, char* argv[])
{
  int server_socket, newSocket, status, socket_size, port;
  char buffer[B_SIZE], file_name[100];
  struct sockaddr_in server_Addr, server_storage;
  FILE *file_1;
 
  /* create TCP, stream server socket */
  server_socket = socket(PF_INET, SOCK_STREAM, 0);
  
  /* initialise sockaddr and buffer to 0 */
  memset(&server_Addr, 0, sizeof(server_Addr));
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
  server_Addr.sin_family = AF_INET;
  server_Addr.sin_port = htons(g);
  server_Addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  /* get server address */
  status = bind(server_socket, (struct sockaddr *) &server_Addr, sizeof(server_Addr));

  if(status < 0)
  {
    printf("Error %d in getting server address!\n", errno);
  }

  /* listen for connections */
  if(listen(server_socket, 5)==0)
  {
    printf("Server listening!\n");
  }
  else
  {
    printf("Error in listening!\n");
  }

  while(1)
  {
     /* get sockaddr size */
    socket_size = sizeof server_storage;

    /* accept new connections */
    newSocket = accept(server_socket, (struct sockaddr *) &server_storage, &socket_size);

    if(newSocket < 0)
    {
      printf("Error %d in accepting client socket!\n", errno);
      return 1;
    }

    /* receive the name of the file which will be read */
    status = recv(newSocket, buffer, B_SIZE, 0);
 
    if(status < 0)
    {
      printf("Error %d in receiving data ! \n", status);
    }
    
    /* copy file name from buffer */
    sprintf(file_name, "%s", buffer);

    /* open file */
    file_1 = fopen(file_name,"r");

    if(file_1 == NULL)
    {
      printf("Error in openining the file\n");
    }

    while(!feof(file_1))
    {
      /* read data from file and put in buffer */
      fread(buffer, sizeof(buffer), 1, file_1);

      /* send data to client */
      send(newSocket, buffer, B_SIZE, 0);
    }
 }

fclose(file_1);
return 0;
}
