#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
	struct node *next;
	struct node *prev;
	char data[20];
}NODE;

NODE *insert(NODE*,char data[20]);
void printList(NODE*);

int main(void)
{
	NODE* header;
	header = (NODE*)malloc(sizeof(NODE));
	header->next=NULL;
	header->prev=NULL;
	strcpy(header->data,"header");
	
	while(1)
	{
		char data[20];
		printf("노드에 추가할 데이터를 입력하세요 : ");
		scanf("%s",data);
		if(strcmp(data,"exit")==0)
			break;
		insert(header,data);
		printList(header);
	}
	return 0;
}

NODE *insert(NODE* header,char data[20])
{
	NODE *nodeNew;
	NODE *tmp;
	nodeNew=(NODE*)malloc(sizeof(NODE));
	nodeNew -> next=NULL;
	strcpy(nodeNew->data,data);
	tmp = header;

	while(tmp->next!=NULL)
		tmp=tmp->next;
	
	tmp->next = nodeNew;
	nodeNew->prev = tmp;

	return nodeNew;
}

void printList(NODE* header)
{
	NODE* tmp = header->next;
	printf("LIST : ");
	while(tmp!=NULL)
	{
		printf("%s",tmp->data);
		if(tmp->next!=NULL)
			printf(" -> ");
		tmp=tmp->next;
	}
	printf("\n\n");
}
