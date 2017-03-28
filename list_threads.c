#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define _XOPEN_SOURCE 600
#define NUMBER_OF_THREADS 3

/* define list struct */
typedef struct node
{
	int value;
	int print_opt;
	struct node *next;
}Node;

/* global variables declaration */
Node *top = NULL;
pthread_barrier_t barrier;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* defines how the element will be displayed */
void print_n(int val, int n_opt)
{
	switch( n_opt )
	{
	case 1: printf("Node value is: %d\n", val);
				break;
	case 2:
			printf("Node value is: 0x%xh\n", val); 
				break;
	default:
				break;
	}
} 

/* add a new node to the end of the list */
Node* add_node(Node** up_node, int val, int print_option)     
{
	/* memory allocation for the new node */
	Node* new_node = (Node*)malloc(sizeof(Node));

	/* lock the list */
	pthread_mutex_lock(&mutex);

	if(new_node == NULL)
	{
		printf("Memory allocation failed.\n");
		return NULL;
	}

	/* copy the given data to new node fields */
	new_node -> value = val;
	new_node -> print_opt = print_option;
	new_node -> next = NULL;

	Node* nnp = *up_node;

	/* check if the list is empty */
	if(*up_node == NULL)
	{
		*up_node = new_node;

	}
	else
	{
		/* search for the last node of the given list
		   and then insert the new node */
		while(nnp->next != NULL)
		{
			/* go to the next node */
			nnp = nnp->next;
		}

		nnp->next = new_node;
	}
	
	/* unlock the list */
	pthread_mutex_unlock(&mutex);

	return new_node;
}

/* print all the elements */
void print_node(Node* head)
{
	/* lock the list */
	pthread_mutex_lock(&mutex);

	Node* my_node = head;

	while(my_node != NULL)
	{
		/* call function to print node values */
		print_n(my_node->value, my_node->print_opt);

		/* go to the next node */
		my_node = my_node->next;
	}		
	
	/* unlock the list */
	pthread_mutex_unlock(&mutex);
}

/* delete the first occurrences of the element in the list if matches*/
Node* delete_first(Node** head, int val)
{
    Node* temp = *head, *temp_1;

    /* check if head of the list contains the element, update the 
       head and check again untill head is different from the element */
    while ((temp != NULL) && (temp->value == val))
    {
        *head = temp->next;   
        free(temp);               
        temp = *head;        
    }

    /* store head value after checking */
    temp_1 = *head;

  /* return the head */ 
  return temp_1;
}

void delete_node(Node** head, int val) 
{
	Node *temp, *temp_p, *temp_i;

	/* lock the list */
	pthread_mutex_lock(&mutex);

	/* check if the head contains the seeking element */
	temp_i = delete_first(head, val);

	/* check if one of the nodes contain the element */ 
	for(temp = temp_i; temp != NULL; temp = temp->next)
	{
		if(temp->value == val)
		{
			temp_p->next = temp->next;
		}
		else 
		{	
			temp_p = temp;
		}
	}	

	/* unlock the list */
	pthread_mutex_unlock(&mutex);
	
	return;
}

/* sort the list */
void sort_list(Node* head)
{
	Node* node_c,* node_p;

	node_p = head;
	node_c = head->next;
	
	/* lock the list */
	pthread_mutex_lock(&mutex);

	while(node_c != NULL)
	{
		while(node_c != node_p)
		{
			if(node_p->value > node_c->value)
	 		{
	 			int aux = node_p->value;
	 			node_p->value = node_c->value;
	 			node_c->value = aux;
			}
			/* go to the next node */
			node_p = node_p->next;
		}

	node_p = head;
	node_c = node_c->next;
	}
	
	/* unlock the list */
	pthread_mutex_unlock(&mutex);

	return;
}

void flush_list(Node* head)
{

	Node* nn,*pp;
	nn = head;

	/* lock the list */
	pthread_mutex_lock(&mutex);

	while(nn != NULL)
	{
	
		pp = nn;
		nn = nn->next;
		free((void*)pp);
	}
	printf("List is empty!\n");

	/* unlock the list */
	pthread_mutex_unlock(&mutex);
	
}

///////////////////////// THREADS ACTIONS //////////////////////////////

/* thread 1 execution routine */
void *thread_1_funct(void *arg)
{
	/* wait for all threads to be created */
	pthread_barrier_wait(&barrier);

	printf("Thread %p add node 2 to the list.\n", arg);
	add_node(&top,2,1);

	printf("Thread %p add node 4 to the list.\n", arg);
	add_node(&top,4,2);

	sleep(10);

	printf("Thread %p add node 10 to the list.\n", arg);
	add_node(&top,10,1);

	printf("Thread %p delete node 2 from the list.\n", arg);
	delete_node(&top,2);

	printf("Thread %p sorted the list.\n", arg);
	sort_list(top);

	printf("Thread %p delete node 10 from the list.\n", arg);
	delete_node(&top,10);

	printf("Thread %p delete node 5 from the list.\n", arg);
	delete_node(&top,5);

	return NULL;
}

/* thread 2 execution routine */
void *thread_2_funct(void *arg)
{
	/* wait for all threads to be created */
	pthread_barrier_wait(&barrier);

	printf("Thread %p add node 11 to the list.\n", arg);
	add_node(&top,11,1);

	printf("Thread %p add node 1 to the list.\n", arg);
	add_node(&top,1,2);

	printf("Thread %p delete node 11 to the list.\n", arg);
	delete_node(&top,11);

	sleep(30);

	printf("Thread %p add node 8 to the list.\n", arg);
	add_node(&top,8,2);
	
	printf("Thread %p print the list.\n", arg);
	print_node(top);

	return NULL;
}

/* thread 3 execution function */
void *thread_3_funct(void *arg)
{
	/* wait for all threads to be created */
	pthread_barrier_wait(&barrier);

	printf("Thread %p add node 30 to the list.\n", arg);
	add_node(&top,30,1);

	printf("Thread %p add node 25 to the list.\n", arg);
	add_node(&top,25,2);

	printf("Thread %p add node 100 to the list.\n", arg);
	add_node(&top,100,2);

	printf("Thread %p sorted the list.\n", arg);
	sort_list(top);

	printf("Thread %p print the list.\n", arg);
	print_node(top);
	
	sleep(5);

	printf("Thread %p delete node 100 from the list.\n", arg);
	delete_node(&top,100);

	return NULL;
}

int main()
{
	/* declaration of threads */
	pthread_t thread_1, thread_2, thread_3;

	/* inittialise the barrier */
	pthread_barrier_init(&barrier, NULL, 3);

	/* create threads */
	pthread_create(&thread_1, NULL, thread_1_funct, (void *) 1);
	pthread_create(&thread_2, NULL, thread_2_funct, (void *) 2);
	pthread_create(&thread_3, NULL, thread_3_funct, (void *) 3);

	/* wait for the threads to finish execution */
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	pthread_join(thread_3, NULL);

	/* delete the barrier */
	pthread_barrier_destroy(&barrier);
	
	printf("Nodes of the list:\n");
	print_node(top);
	
	printf("Remove all nodes!\n");
	flush_list(top);
	print_node(top);

	return 0;
}
