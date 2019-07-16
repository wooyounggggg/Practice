#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	struct Node* next;
	struct Node* prev;
	int value;
}Node;	

//function declaration
void setDefault();
Node* NodeAllocation();
int getListLength();
void freeAllList();
void addNode(int,int);
void printAllList();

//global variable
Node* header;
Node* trailer;
 
int main(void){
	int listLength;
	setDefault(1,10);
	listLength = getListLength();
    addNode(1,2);
    addNode(2,3);
    addNode(3,4);
    printAllList();
	freeAllList();	

	return 0;
}

void setDefault(int headerValue,int trailerValue){
	header = NodeAllocation(headerValue);
	trailer = NodeAllocation(trailerValue);
	header->next = trailer;
	trailer->prev = header;
    header->value=headerValue;
    trailer->value=trailerValue;
}

Node* NodeAllocation(){
    Node* newNode;
	if(!(newNode = (Node*)malloc(sizeof(Node)))){
    }
	return newNode;
}

int getListLength(){
	int listLength=1;
	Node* tmp;
	for(tmp=header;(tmp->next)!=NULL;tmp=tmp->next){
		listLength++;
	}
	return listLength;
}

void freeAllList(){
	Node* tmp;
	for(tmp=header;(tmp->next)!=NULL;tmp=tmp->next){
		if(tmp==header) continue;
		free(tmp->prev);
	}
	free(trailer->prev);
	free(trailer);
}
void addNode(int index,int value){
	Node* tmp = header;
    Node* newNode;
	int i;
    newNode=NodeAllocation();
	newNode->value=value;
	for(i=0;i<index-1;i++){
        tmp=tmp->next;
	}
	newNode->prev=tmp;
	newNode->next=tmp->next;
	(tmp->next)->prev=newNode;
	tmp->next=newNode;		
}
void printAllList(){
    Node* tmp;
    printf("header value:%d\n",header->value);
    for(tmp=header->next;tmp->next!=NULL;tmp=tmp->next){
        printf("value:%d\n",tmp->value);
    }
    printf("trailer value:%d\n",trailer->value);

}
