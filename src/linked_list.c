#include "../include/linked_list.h"

//参考：https://www.cnblogs.com/maluning/p/7966875.html  和 https://blog.csdn.net/endeavor_g/article/details/80552680

//创建链表，通过用户输入创建
pNode linkList_creat(int n){
	pNode pHead,pNormal,pTail;            //定义头节点，普通节点，尾部节点；
	pHead = (pNode)malloc(sizeof(Node));  //分配地址
	pTail = pHead;                        //若是空链表则头尾节点一样
	for (int i = 0; i < n; i++) {
		pNormal = (pNode)malloc(sizeof(Node));
		printf("总共 %d 个数据，现在请输入第 %d 个数据：",n,i+1);
		scanf("%d",&pNormal->member);
		pTail->pNext = pNormal;
		pTail = pNormal;
	}
	pTail->pNext = NULL;//结束创建
	return pHead;
}

//创建链表，通过数组创建
pNode linkList_creat_arr(int *arr, int n){
	pNode pHead,pNormal,pTail;            //定义头节点，普通节点，尾部节点；
	pHead = (pNode)malloc(sizeof(Node));  //分配地址
	pTail = pHead;                        //若是空链表则头尾节点一样
	for (int i = 0; i < n; i++) {
		pNormal = (pNode)malloc(sizeof(Node));
		pNormal->member = arr[i];
		pTail->pNext = pNormal;
		pTail = pNormal;
	}
	pTail->pNext = NULL;//结束创建
	return pHead;
}

//链表遍历函数
void linkList_traverse(pNode List) {
    pNode P = List->pNext;         // 首节点赋值给临时节点P
    printf("遍历链表的值为：");
    if (P == NULL)
        printf("链表为空");
    while (P != NULL) {            // 当临时节点P不为尾节点时，输出当前节点值 
        printf("%d ", P->member);
        P = P->pNext;
    }
    printf("\n");
}

//链表获取函数
pNode linkList_get(pNode List,int pos) {
    int position = 0;
    pNode P = List;                             // 定义节点p指向头节点
    while (P != NULL && position<pos){          // 寻找pos节点
        P = P->pNext;
        ++position;
    }
	if (P != NULL) {
		printf("节点 %d 的值为：%d \n",pos,P->member);
	}
	else {
		puts("节点不存在");
	}
    return P;
}

//链表查询函数
pNode linkList_find(pNode List,int val) {
    pNode P = List->pNext;    // 定义临时指针P指向首节点的地址
    int num = 0;              // 用于记录链表节点位置
    while (P != NULL && P->member != val) {
        P = P->pNext;
        ++num;
    }
    if (P != NULL)
        printf("找到 %d 的所在节点为：%d",val, num + 1);
    else
        printf("找不到该节点");
    printf("\n");
    return P;
}

// 链表修改函数, 在链表位置第pos节点修改数据为val
void linkList_modify(pNode List, int pos, int val) {
    int position = 0;
    pNode P = List;                             // 定义节点p指向头节点
    while (P != NULL && position<pos){          // 寻找pos节点
        P = P->pNext;
        ++position;
    }
	if (P != NULL) {
		P->member = val;
	}
	else {
		puts("节点不存在");
	}
}

// 链表插入/增加函数, 在链表位置第pos节点前插入包含数据val的节点
void linkList_insert(pNode List, int pos, int val) {
    int position = 0;
    pNode P = List;                             // 定义节点p指向头节点
    while (P != NULL && position<pos - 1){      // 寻找pos节点的前驱结点
        P = P->pNext;
        ++position;
    }
    pNode Tmp = (pNode)malloc(sizeof(Node));    // 分配一个临时节点用来存储要插入的数据
    if (Tmp == NULL){
        printf("内存分配失败！");
        exit(-1);
    }
    Tmp->member = val;                         // 插入节点
    Tmp->pNext = P->pNext;
    P->pNext = Tmp;
}

// 删除链表元素函数, 删除链表中的第pos节点
void linkList_deleteNode(pNode List, int pos) {
    int position = 0;
    pNode P = List;                             // 定义一个指针p指向链表头节点       
    while (P != NULL && position < pos - 1) {   // 寻找pos节点位置的前驱节点
        P = P->pNext;
        ++position;
    }
    // 删除节点
    pNode Tmp = P->pNext;      // 定义临时指针Tmp指向要删除的节点
    P->pNext = Tmp->pNext;     // 使要删除节点的前驱结点指向其后驱节点
    free(Tmp);                 // 释放删除节点的内存空间，防止内存泄漏
    Tmp = NULL;                // 使q指向空指针，防止产生野指针
}

// 删除整个链表函数
void linkList_deleteList(pNode List) {
    pNode P, Tmp;
    P = List->pNext;    //定义指针P指向链表要删除的链表List的第一个点节点
    List->pNext = NULL;
    while (P != NULL) {
        Tmp = P->pNext;        //临时Tmp指向要删除的节点的下个节点
        free(P);    //释放指针P指向的节点
        P = Tmp;    //重新赋值
    }
    printf("删除链表成功！\n");
}


