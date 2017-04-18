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
#define CLIENTS 5

int main(int argc, char* argv[])
{
  int server_socket, new_socket, status, socket_size, port, n_client, max, opt = 1, i = 0,j = 0;
  char buffer[B_SIZE], file_name[100], client_number[5];
  struct sockaddr_in server_Addr, server_storage;
  FILE *file_1;
  fd_set select_socket;
  char* name;
  struct timeval ss;
  
  /* create TCP, stream server socket */
  server_socket = socket(PF_INET, SOCK_STREAM, 0);
  
  /* initialise sockaddr and buffer to 0 */
  memset(&server_Addr, 0, sizeof(server_Addr));
  memset(buffer, 0, sizeof(buffer));

  /* initialise all client values to 0 */
  for(int i = 0;i < CLIENTS; i++)
  {
    client_number[i] = 0;
  }

  /* chceck that the port number was given */
  if(argc != 2)
  {
      printf("Please specify the port number!\n");
      return 1;
  }

  /* store and convert port number */
  port = atoi(argv[1]);
  
  /* update sockaddr fields */
  server_Addr.sin_family = AF_INET;
  server_Addr.sin_port = htons(port);
  server_Addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
  {
    printf("Error in setting socket options!\n");
    return 1;
  }

  /* get server address */
  status = bind(server_socket, (struct sockaddr *) &server_Addr, sizeof(server_Addr));

  if(status < 0)
  {
    printf("Error %d in getting server address!\n", errno);
    return 1;
  }

  /* listen for connections of 5 clients*/
  if(listen(server_socket, CLIENTS) == 0)
  {
    printf("Server listening!\n");
  }
  else
  {
    printf("Error in listening!\n");
    return 1;
  }

  /* wait for connections */
  while(1)
  {
    /* set all descriptors to 0 */
    FD_ZERO(&select_socket);

    /* add server socket to selected sockets */
    FD_SET(server_socket, &select_socket);

    /* initialise maximum number of descriptors */
    max = server_socket;

    /* set the bits for all the clients in file descriptor */
    for(int p = 0; p<i; p++)
    {
      if(client_number[p] > 0)
      {
        if(client_number[p] > max)
        { 
          /* update maximum number of descriptors */
          max = client_number[p];
        }

        /* add new client to selected sockets */
        FD_SET(client_number[p], &select_socket);
      }
    }

    /* select a descriptor from the structure */
    status = select(max+1, &select_socket, NULL, NULL, NULL);

    if(status < 0)
    {
      printf("Error %d in selecting the client ! \n", status);
    }

    /* check if server descriptor is set */
    if(FD_ISSET(server_socket, &select_socket))
    {
      socket_size = sizeof server_storage;

      /* server has accepted new connection */
      new_socket =  accept(server_socket, (struct sockaddr *)&server_storage, &socket_size);

      if(new_socket < 0)
      {
        printf("Error in accepting clients!\n");
        return 1;
      }

      /* add new client descriptor to client array */
      client_number[i] = new_socket;

      printf("Client number is: %d \n", client_number[i]);

      /* increment client array size */
      if(i < 5)
      {
        ++i;
      }
      else
      {
        printf("Out of clients array, wait until a connection si available!\n");

        /* close last created client */
        close(client_number[i]);
      }
    }
    

    for(int j = 0; j < i; j++)
    {
      n_client = client_number[j];

      printf("n_client = %d\n", n_client);

      /* check if one of the client descriptor is set */
      if((FD_ISSET(n_client, &select_socket)))
      {
        status = recv(client_number[j], buffer, B_SIZE, 0);
        
        /* check if the selected client was closed */
        if(status == 0)
        {
          /* remove client descriptor from monitored file descriptor structure */
          FD_CLR(client_number[j], &select_socket);

          printf("Client %d was closed! New connection can be performed!\n",client_number[j]);

          /* update clients number */
          --i;
        }

        if(status < 0)
        { 
          printf("Error %d in receiving data ! \n", status);
          return 1;
        }

        if(status > 0)
        {
          /* copy file name from buffer */
          snprintf(file_name, 100, "%s", buffer);
      
          printf("file name = %s\n", file_name);

          name = strtok(file_name, "\n");

          /* open file */
          file_1 = fopen(name, "r");

          if(file_1 == NULL)
          {
            printf("Error %s in openining the file\n", strerror(errno));

            /* set buffer to 0 */
            strncpy(buffer, "File not found", B_SIZE);

            /* send data to client */
            send(client_number[j], buffer, B_SIZE, 0);

            break;
          }
          else
              {
                /* set buffer to 0 */
                memset(buffer, 0, sizeof(buffer));

                while(!feof(file_1))
                {
                  /* read data from file and put in buffer */
                  fread(buffer, sizeof(buffer), 1, file_1);

                  /* send data to client */
                  send(client_number[j], buffer, B_SIZE, 0);
                }

                /* close opened file */
                fclose(file_1);
              }
        }   
      }
    }
  }

return 0;
}
     

    
 
