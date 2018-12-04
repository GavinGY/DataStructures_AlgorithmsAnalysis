#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// 定义链表中的节点  
typedef struct NodeType  {  
    int member;                    // 节点中的成员  
    struct NodeType *pNext;        // 指向下一个节点的指针  
}Node,*pNode;  
  

pNode linkList_creat(int n);
pNode linkList_creat_arr(int *arr, int n);
void  linkList_traverse(pNode List);
pNode linkList_get(pNode List,int pos);
pNode linkList_find(pNode List,int val);
void  linkList_modify(pNode List, int pos, int val);
void  linkList_insert(pNode List, int pos, int val);
void  linkList_deleteNode(pNode List, int pos);
void  linkList_deleteList(pNode List);

#endif