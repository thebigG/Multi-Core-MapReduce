#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<ctype.h>
#include<string.h>
#include<getopt.h>
#include "sorter.h"
#include<stddef.h>
#include<pthread.h>

//Binary code:
//	struct treeNode * root = sortedArrayToBST(myRow, 0, totalRows-1);
//	printf("n PreOrder Traversal of constructed BST \n");
//
//	inOrder(root);
//	printf("\n");


struct treeNode * sortedArrayToBST(struct defineRow * myRow, int start, int end)
{
	// Initial case
	if (start > end)
		return NULL;
	// Find the middle and make it root
	int mid = (start + end)/2;

	struct treeNode * root = newNode(myRow, mid);

	root->left = sortedArrayToBST(myRow, start, mid-1);

	root->right = sortedArrayToBST(myRow, mid+1, end);

	return root;
}

struct treeNode * newNode (struct defineRow * myRow, int mid)
{
	struct treeNode * node = (struct treeNode *) malloc (sizeof(struct treeNode));
	if(node == NULL)
	{
		printf("Error could not malloc for node");
		exit(1);
	}
//	(*node).numData = myRow[mid].numData;
//	strncpy((*node).stringData, myRow[mid].stringData, MAXFIELDLEN);
//	strncpy((*node).wholeRow, myRow[mid].wholeRow, ((MAXFIELDLEN+1)*MAXCOL));
	(*node).row = &myRow[mid];
	node->left = NULL;
	node->right = NULL;

	return node;
}


struct treeNode * insert(struct defineRow * myRow, int i, enum varType colType, struct treeNode *node)
{
	if(colType == 1)
	{
		if(node == NULL)
		{
			return (newNode(myRow, i));
		}
		else if (strcmp(myRow[i].stringData, ((*node).row)->stringData) <= 0)
		{
			node->left = insert(myRow, i, colType, node->left);
		}
		else if (strcmp(myRow[i].stringData, ((*node).row)->stringData) > 0)
		{
			node->right = insert(myRow, i, colType, node->right);
		}
		return node;
	}
	else if (colType == 2)
	{
		if(node == NULL)
		{
			return (newNode(myRow, i));
		}
		else if (myRow[i].numData <= ((*node).row)->numData)
		{
			node->left = insert(myRow, i, colType, node->left);
		}
		else if (myRow[i].numData > ((*node).row)->numData)
		{
			node->right = insert(myRow, i, colType, node->right);
		}
		return node;
	}
}		



void preOrder(struct treeNode * node)
{
	if (node == NULL)
		return;
	printf("%s, ", ((*node).row)->stringData);
//	printf("%s, ", (*node).stringData);
	preOrder(node->left);
	preOrder(node->right);
}


void inOrder(struct treeNode * node, FILE * file)
{
//	if (node == NULL)
//		return;
	if(node != NULL)
	{
	inOrder(node->left, file);

	int a;
	a = strlen(((*node).row)->wholeRow);
	if(a < 5)
	{
	}
	else if(*(((*node).row)->wholeRow+a-1) == '\n')
	{	
//		printf("%s\n", ((*node).row)->wholeRow);
		fprintf(file, "%s", ((*node).row)->wholeRow);
	}
	else
	{
//		printf("%s\n\n", ((*node).row)->wholeRow);
		fprintf(file, "%s\n", ((*node).row)->wholeRow);
	}

	inOrder(node->right, file);
	}
}
