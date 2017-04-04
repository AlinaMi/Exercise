#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* MACROs definition */
#define SERVER_NAME "/server"
#define BUF_SIZE    (1<<13)

/* global variable declaration */
static int token = 0;

int main( int argc, char* argv[])
{
	int rc;
    mqd_t sv, cl;
    char buf_out[BUF_SIZE];
    char buf_in[BUF_SIZE] ;
    char cl_name[32];

    /* initialise buffers to 0 */
 	memset(buf_in,0,sizeof(buf_in));
    memset(buf_out,0,sizeof(buf_out));

    /* check if the program run as server or as client */
    if ((strcmp(argv[1] ,"server")) == 0) 
    {	
        /* close Server queue if it is open*/
        rc = mq_unlink(SERVER_NAME);

        /* open Server queue */
        sv = mq_open(SERVER_NAME, O_CREAT | O_RDWR, 0666, NULL);

        /* check that queue was successfully created */
        if(sv < 0)
        {
            printf("Error %d in opening the Server queue! \n", errno);
            return 0;
        }
        else
        {
            printf("Server queue created!\n");
        }
        
        /* wait for clients to request a token */
        while(1)
        {

        /* wait for clinets to send a request */
        rc = mq_receive(sv, buf_in, BUF_SIZE, NULL);

        if(rc < 0)
        {
            printf("Error %d in receiving data in Server queue! \n", errno);
            return 0;
        }

        /* form client name based on the information received in buffer */
        sprintf(cl_name,"/client-%s",buf_in);
    
        /* open Client queue */
        cl = mq_open(cl_name, O_WRONLY);

        /* check that queue was successfully created */
        if(cl < 0)
        {
            printf("Error %d in opening Cleint queue! \n", errno);
            return 0;
        }

        /* print the client name requesting the token */
        printf("Client %s request token.\n", buf_in);

        /* put token in buffer */
        sprintf(buf_out, "%d", token);

        /* send token to Client queue */
    	rc = mq_send(cl, buf_out, strlen(buf_out), 0);

        if(rc < 0)
        {
            printf("Error %d in sending message to client! \n", errno);

        }

        /* increment token value */
        token++;
       }
    	
    }
    else
    {
        /* concatenate client queue name */
        sprintf(cl_name,"/client-%s",argv[1]);

        /* open Server queue */
        sv = mq_open(SERVER_NAME, O_WRONLY, 0666, NULL);
        
        /* check that queue was successfully created */
        if(sv < 0)
        {
            printf("Error %d in opening the Server queue! \n", errno);
            return 0;
        }
    
        /* open Client queue */
        cl = mq_open(cl_name, O_RDWR | O_CREAT, 0666, NULL);

        /* check that queue was successfully created */
        if(cl < 0)
        {
            printf("Error %d in opening the Client queue! \n", errno);
            return 0;
        }
        else
        {
            printf("Client queue created!\n");
        }

        /* copy client name in buffer */
        strncpy(buf_out, argv[1],sizeof(buf_out));

        /* send client queue name to Server */
        rc = mq_send(sv, buf_out, strlen(buf_out), 0);

        if(rc < 0)
        {
            printf("Error %d in sending message to Server! \n",errno);

        }

        /* get token */
    	rc = mq_receive(cl, buf_in, BUF_SIZE, NULL);
    	

        if(rc < 0)
        {
            printf("Error %d in receiving message from Server! \n",errno);

        }

    	printf("Client %s has token: %s\n", argv[1], buf_in);

        mq_close(cl);

        mq_unlink(cl_name);
    }

}
