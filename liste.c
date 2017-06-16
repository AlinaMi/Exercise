#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* define list struct */
typedef struct node
{
	int value;
	int print_opt;
	struct node *next;
}Node;


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

	return new_node;
}

/* print all the list */
void print_node(Node* head)
{
	Node* my_node = head;

	while(my_node != NULL)
	{
		/* call function to print node values */
		print_n(my_node->value, my_node->print_opt);

		/* go to the next node */
		my_node = my_node->next;
	}		
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
	return;
}

/* sort the list */
void sort_list(Node* head)
{
	Node* node_c,* node_p;

	node_p = head;
	node_c = head->next;
	
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
	return;
}

void flush_list(Node* head)
{
	Node* nn,*pp;
	nn = head;
	while(nn != NULL)
	{
		pp = nn;
		nn = nn->next;
		free((void*)pp);
	}
	printf("List is empty!\n");
}

int main()
{
	Node *top = NULL, *nn;

	add_node(&top,5,1);
	add_node(&top,5,1);
	add_node(&top,5,1);
	add_node(&top,7,2);
	add_node(&top,5,1);
	add_node(&top,9,1);
	add_node(&top,4,1);
	add_node(&top,7,2);
	//add_node(&top,5,1);
	add_node(&top,1,1);
	//add_node(&top,2,1);
	//add_node(&top,10,1);
	//add_node(&top,6,2);

	print_node(top);

	delete_node(&top,5);
	delete_node(&top,1);

	printf("after delete:\n");
	print_node(top);

	printf("after sorting:\n");
	sort_list(top);
	print_node(top);
	printf("delete:\n");
	flush_list(top);
	print_node(top);
	/*
	 add_node(&top,9,2);
	  add_node(&top,1,2);
	   add_node(&top,4,2);
	    add_node(&top,2,1);
	    print_node(top);*/

	return 0;
}
