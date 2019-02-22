#ifndef _Hash_Table_H
 
#include "debug.h" 

// 参考：https://blog.csdn.net/shuxiao9058/article/details/7433895
 
// 方式一 分离链接法
typedef int ElementType;
typedef unsigned int Index;
struct ListNode;
typedef struct ListNode *Position;
struct HashTbl;
typedef struct HashTbl *HashTable;
 
Index Hash( const int Key, int TableSize );
 
HashTable InitializeTable( int TableSize );
void DestroyTable( HashTable H );
Position Find( ElementType Key, HashTable H );
void Insert( ElementType Key, HashTable H );
ElementType Retrieve( Position P );
/* Routines such as Delete and MakeEmpty are omitted */

/***********************************************************分割线*****************************************/

// 方式二 开放定址法 
// typedef int ElementType;
// typedef unsigned int Index;
// typedef Index Position;
// struct HashTbl;
// typedef struct HashTbl *HashTable;
 
// static int NextPrime( int N );
// Index Hash( ElementType Key, int TableSize );
// HashTable InitializeTable( int TableSize );
// void DestroyTable( HashTable H );
// Position Find( ElementType Key, HashTable H );
// void Insert( ElementType Key, HashTable H );
// ElementType Retrieve( Position P, HashTable H );
// HashTable Rehash( HashTable H );
// /* Routines such as Delete and MakeEmpty are omitted */

#endif /* _Hash_Table_H */
