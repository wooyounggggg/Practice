#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	struct node *next;
	struct node *prev;	
	int data;
}NODE;

NODE* insert(NODE*,int);
void printList(NODE*);

int main(void)
{
	NODE *header;
	header = (NODE*)malloc(sizeof(NODE));
	header->next = NULL;
	header->prev = NULL;
	header->data = 2;

	insert(header,3);
	printList(header);
	free(header);
	return 0;
}

NODE* insert(NODE* header,int data)
{
	NODE *tmp=header;
	NODE *nodeNew;

	while(tmp->next!=NULL)
		tmp = tmp->next;
	nodeNew = (NODE*)malloc(sizeof(NODE));
	nodeNew->data = data;
	nodeNew->next = NULL;
	tmp->next = nodeNew;
	
	return nodeNew;
}

void printList(NODE* header)
{
	NODE* tmp = header;
	while(tmp!=NULL)
	{
		printf("%d\n",tmp->data);
		tmp = tmp->next;
	}

}
