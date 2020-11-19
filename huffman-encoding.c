/****************************************************************
Huffman Encoding
PROGRAM DESCRIPTION: Gets a text file as input and generates
another text file containing a list of Huffman codes as output.
BY: Rovie Jerahmeel T. Doculan
DATE: July 24, 2011
****************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 26
#define CAP 50

struct btnode {
	int value;
	int tag;
	int letter;
	struct btnode *left;	
	struct btnode *right;
	struct btnode *next;
};	
struct btnode *root = NULL, *conductor = NULL;

void count_letters (int lcount[], int slcount[]);
void sort_freq (int slcount[]);
void init_list (int slcount[], int lcount[]);
void list_to_binTree();
int insert_fatherNode (struct btnode *newFatherNode);
void read_codes(struct btnode *conductor, char code[CAP], char codeArray[][CAP], char currTag, int i);
void output(char codeArray[][CAP]);

int main() {
	
	int lcount[SIZE] = {0}, slcount[SIZE] = {0}, i = 0, j = 0;
	char codeArray[SIZE][CAP], code[CAP] = "\0", currTag;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < CAP; j++)
			codeArray[i][j] = ' ';

	count_letters(lcount, slcount);
	sort_freq(slcount);
	init_list (slcount, lcount);
	list_to_binTree();
	conductor = root; i = 0;
	read_codes(conductor, code, codeArray, currTag, i);
	output(codeArray);	
}	

/* Subprogram name: count_letters
	Description: counts letter frequencies of the input file and stores values to array lcount;
		then copies to array slcount which would be sorted later on */
	
void count_letters (int lcount[], int slcount[]) {
	
	FILE *iPtr;
	int *p, a = 0;
	p = &a;
	
	if ((iPtr = fopen("test.txt", "r")) == NULL)
			printf("File could not be opened.\n");
	else {
		while ((*p = fgetc(iPtr))!=EOF) {
			if (islower(*p))
				*p = *p - 32;
			if (isupper(*p)) {
				*p = *p - 'A';
				++lcount[*p]; 	
			}	
		}
		--lcount['E'-'A'];
		--lcount['N'-'A'];
		--lcount['D'-'A'];
	}
	fclose(iPtr);
	for (a = 0; a < SIZE; a++) {		//copies lcount to slcount
		slcount[a] = lcount[a];	
	}	
}	

/* Subprogram name: sort_freq
	Description: sorts array slcount in increasing order */
		
void sort_freq (int slcount[]) {
	
	int x = 0, y = 0, temp = 0;
	for (x = 2; x < SIZE; x++) {
		temp = slcount[x];
		y = x - 1;
		while ((y > 0) && (slcount[y] > temp)) {
			slcount[y+1] = slcount[y];	
			y = y - 1;
		}	
		slcount[y+1] = temp;
	}	
	temp = slcount[0], x = 1, 	y = 0;	//sorts first element
	while (temp > slcount[x]) {
		++y;
		++x;	
	}	
	for (x = 0; x <= y; x++) {
		slcount[x] = slcount[x+1];	
	}	
	slcount[y] = temp;
}	

/* Subprogram name: init_list
	Description: transfers array slcount to linked list */

void init_list (int slcount[], int lcount[]) {
 
 	int x = 0, y = 0, l = 0, temp = 1;
 	int mayLetterNaBa[SIZE] = {0};		
 	root = (struct btnode*)malloc(sizeof(struct btnode));	//init root = first element of slcount = 1
 	root -> value = slcount[0];
 	root -> tag = 0;	
 	while (slcount[0]!=lcount[x])
 		x++;
 	++mayLetterNaBa[x];	
 	l = x + 1;		
 	root -> letter = l;	
 	root -> next = NULL;
 	root -> left = NULL;
 	root -> right = NULL;				
	conductor = root;

	for (x = 1; x < SIZE; x++) {
		conductor -> next = (struct btnode*)malloc(sizeof(struct btnode));
		conductor = conductor -> next;
		conductor -> value = slcount[x];
		conductor -> tag = 0;
		y = 0;
		while (temp) {
			if ((slcount[x]==lcount[y]) && (mayLetterNaBa[y] == 0))
					temp = 0;	
			else y++;
		}	
		++mayLetterNaBa[y];	
 		conductor -> letter = y + 1; //plus one to exclude sum vertices (letter = 0)		
		conductor -> left = NULL;
		conductor -> right = NULL;
		conductor -> next = NULL;
		temp = 1;
	}	

}	

/* Subprogram name: list_to_binTree
	Description: converts list to a binary tree */
	
void list_to_binTree() {
	
	int contr = 1, tvalue = 0, currValue = 0, count = 0, firstba = 0;
	struct btnode *node1 = NULL, *node2 = NULL, *newFatherNode = NULL, *del = NULL;
	conductor = root;
	
	while (conductor) { //scans through whole list
	
		if ((contr % 2) == 1)	{ //for odd nodes
			currValue = conductor->value;
			tvalue = tvalue + currValue;
			node1 = (struct btnode *)malloc(sizeof(struct btnode));	//copy currNode
			node1->value = currValue;
			node1->tag = 0;
			node1->letter = conductor->letter;
			node1->left = conductor->left;
			node1->right = conductor->right;
			node1->next = NULL; 
			del = conductor;
		}	
		else if ((contr % 2) == 0)	{ 
			if (del!=NULL) free(del);               
			currValue = conductor -> value;
			tvalue = tvalue + currValue;
			node2 = (struct btnode *)malloc(sizeof(struct btnode));	//copy currNode
			node2->value = currValue;
			node2->tag = 1;
			node2->letter = conductor->letter;
			node2->left = conductor->left;
			node2->right = conductor->right;
			node2->next = NULL; 
			del = conductor;
		   		
			newFatherNode = (struct btnode *)malloc(sizeof(struct btnode));	//make new vertice
			newFatherNode -> value = tvalue;
			newFatherNode ->tag = 0;
			newFatherNode ->letter = 0;
			newFatherNode -> left = node1;
			newFatherNode -> right = node2;
				
		   if(conductor->next) {
				root = conductor->next;
				firstba = insert_fatherNode(newFatherNode); //sets newFatherNode->next;
			}
			else //if last node
				root = newFatherNode;   		
			tvalue = 0;
			contr = 0;
			node1 = NULL, node2 = NULL, newFatherNode = NULL;                                                                                                                                    
		}	
						
		count = 0;
		++contr;
		currValue = 0;
		if (firstba!=1)conductor = conductor -> next;
		else conductor = root;
		firstba = 0;
	}	

}	

/* Subprogram name: insert_fatherNode
	Description: inserts a vertice to the binary tree */
	
int insert_fatherNode(struct btnode *newFatherNode) {
	
	int temp = newFatherNode -> value, a = 0;
	struct btnode *d = root, *p1 = NULL, *p2 = NULL, *p3 = NULL;
	p3 = root;
	if (temp < (p3->value)) { //if smaller than 1st value 
		root = newFatherNode;
		root -> next = p3;
		a = 1;
	}
	else {
 	  while (((d->value) < temp) && (d->next!=NULL)) {
   		p3 = d;
   		d = d->next;
  	 	}	
  	  if (d->value < temp) {
  	 		p1 = d;
   		p1 -> next = newFatherNode;
   		newFatherNode -> next = NULL;
   	}
	  else {		//p1 - node before node to be inserted
  			p1 = p3;		//p2 - node after node to be inserted
   		p2 = p1 -> next;
 			p1->next = newFatherNode;
 			newFatherNode -> next = p2;	
 		}
	}
	
	return a;
}	

/* Subprogram name: read_codes
	Description: reads Huffman codes */
	
void read_codes (struct btnode *conductor, char code[CAP], char codeArray[][CAP], char currTag, int i) {
	
	if (conductor!=NULL) {	
		if (conductor!=root) {
			currTag = (char)(((int)'0')+conductor->tag);
			code[i] = (char)currTag;
			++i;
			code[i] = '\0';
		}
		if((conductor->left==NULL)&&(conductor->right==NULL)) { //if leaf reached
			int index = (conductor->letter);
			int length = strlen(code);
			code[length] = '\0';
			if(index!=0) strcpy(codeArray[index-1],code);
			code = "\0";
			i = 0;
		}	
		read_codes(conductor->left, code, codeArray, currTag, i);
		read_codes(conductor->right, code, codeArray, currTag, i);
	}
	
}	

/* Subprogram name: output
	Description: prints codes to output file */
	
void output (char codeArray[][CAP]) {
	
	int x = 0;
	FILE *fPtr;
	if ((fPtr = fopen("201046535.txt", "w")) == NULL)
		printf("File could not be opened.\n");
	else {
		for (x = 0; x < SIZE; x++) {
			fprintf(fPtr, "%c=%s", 'A' + x, codeArray[x]);
			if (x!=(SIZE-1)) fprintf(fPtr, "\n");
		}		
	}	
	fclose(fPtr);	
	
}	