#include "../include/hash_table.h" 

#define MinTableSize (10)

// typedef Position List;
 
// Index Hash( const int Key, int TableSize )
// {
	// return Key % TableSize;
// }
 
// struct ListNode
// {
	// ElementType Element;
	// Position Next;
// };
 
// /* List *TheList will be an array of lists,allocated later */
// /* The lists use headers (for simplicity), */
// /* though this wastes space */
// struct HashTbl
// {
	// int TableSize;
	// List *TheLists;
// };
 
// /* Return next prime; assume N >= 10 */
// static int NextPrime( int N )
// {
	// int i;
 
	// if( N % 2 == 0 )
		// N++;
	// for( ; ; N += 2 )
	// {
		// for( i = 3; i * i <= N; i += 2 )
			// if( N % i == 0 )
				// goto ContOuter;  /* Sorry about this! */
		// return N;
// ContOuter: ;
	// }
// }
 
// HashTable InitializeTable( int TableSize )
// {
	// HashTable H;
	// int i;
 
	// if ( TableSize < MinTableSize )
	// {
		// Error( "Table size too small" );
		// return NULL;
	// }
 
	// /* Allocate table */
	// H = malloc( sizeof( struct HashTbl ) );
	// if ( H == NULL)
		// FatalError( "Out of space!!!" );
 
	// H->TableSize = NextPrime( TableSize );
 
	// /* Allocate array of lists */
	// H->TheLists = malloc( sizeof( List ) * H->TableSize );
	// if( H->TheLists == NULL )
		// FatalError( "Out of space!!!" );
 
	// /* Allocate list headers */
	// for( i = 0; i < H->TableSize; i++ )
	// {
		// H->TheLists[ i ] = malloc( sizeof( struct ListNode ) );
		// if ( H->TheLists[ i ] == NULL )
			// FatalError( "Out of space!!!" );
		// else
			// H->TheLists[ i ]->Next = NULL;
	// }
	
	// return H;
// }
 
// Position Find( ElementType Key, HashTable H )
// {
	// Position P;
	// List L;
 
	// L = H->TheLists[ Hash( Key, H->TableSize )];
	// P = L->Next;
 
	// while( P != NULL && P->Element != Key )
		// /* Probably need strcmp!! */
		// P = P->Next;
	
	// return P;
// }
 
// void Insert( ElementType Key, HashTable H )
// {
	// Position Pos, NewCell;
	// List L;
	
	// Pos = Find( Key, H );
	// if ( Pos == NULL ) /* Key is not found */
	// {
		// NewCell = malloc( sizeof( struct ListNode ) );
		// if ( NewCell == NULL )
			// FatalError( "Out of space!!!" );
		// else
		// {
			// L = H->TheLists[ Hash( Key, H->TableSize ) ];
			// NewCell->Next = L->Next;
			// NewCell->Element = Key; /* Probably need strcpy!! */
			// L->Next = NewCell;
		// }
	// }
// }
// void DestroyTable( HashTable H )
// {
	// int i;
 
	// for( i = 0; i < H->TableSize; i++ )
	// {
		// Position P = H->TheLists[ i ];
		// Position Tmp;
 
		// while( P != NULL )
		// {
			// Tmp = P->Next;
			// free( P );
			// P = Tmp;
		// }
	// }
 
	// free( H->TheLists );
	// free( H );
// }
 
 
// ElementType Retrieve( Position P )
// {
	// return P->Element;
// }

/***********************************************************分割线*****************************************/
 
// 方式二 开放定址法 

#define MinTableSize (10)
 
enum KindOfEntry { Legitimate, Empty, Deleted };
 
struct HashEntry
{
	ElementType Element;
	enum KindOfEntry Info;
};
 
typedef struct HashEntry Cell;
 
/* Cell *TheCells will be an array of */
/* HashEntry cells, allocated later */
struct HashTbl
{
	int TableSize;
	Cell *TheCells;
};
 
/* Return next prime; assume N >= 10 */
 
static int NextPrime( int N )
{
	int i;
 
	if( N % 2 == 0 )
		N++;
	for( ; ; N += 2 )
	{
		for( i = 3; i * i <= N; i += 2 )
			if( N % i == 0 )
				goto ContOuter;  /* Sorry about this! */
		return N;
ContOuter: ;
	}
}
 
/* Hash function for ints */
Index Hash( ElementType Key, int TableSize )
{
	return Key % TableSize;
}
 
HashTable InitializeTable( int TableSize )
{
	HashTable H;
	int i;
 
	if ( TableSize < MinTableSize )
	{
		Error( "Table size too small!" );
		return NULL;
	}
 
	/* Allocate table */
	H = malloc( sizeof( struct HashTbl ) );
	if ( H == NULL )
		FatalError( "Out of space!!!" );
 
	H->TableSize = NextPrime( TableSize );
	DebugPrintf("H->TableSize: %d \n",H->TableSize);
 
	/* Allocate array of Cells */
	H->TheCells = malloc( sizeof( Cell ) * H->TableSize );
	if ( H->TheCells == NULL )
		FatalError( "Out of space!!!" );
 
	for ( i = 0; i < H->TableSize; i++ )
		H->TheCells[ i ].Info = Empty;
	
	return H;
}
 
Position Find( ElementType Key, HashTable H )
{
	Position CurrentPos;
	int CollisionNum;
 
	CollisionNum = 0;
	CurrentPos = Hash( Key, H->TableSize );
	while ( H->TheCells[ CurrentPos ].Info != Empty &&
		H->TheCells[ CurrentPos ].Element != Key )
		/* Probably need strcpy! */
	{
		CurrentPos += 2 * ++CollisionNum - 1;
		if ( CurrentPos >= H->TableSize )
			CurrentPos -= H->TableSize;
	}
	return CurrentPos;
}
 
void Insert( ElementType Key, HashTable H )
{
	Position Pos;
 
	Pos = Find( Key, H );
	if ( H->TheCells[ Pos ].Info != Legitimate )
	{
		/* Ok to insert here */
		H->TheCells[ Pos ].Info = Legitimate;
		H->TheCells[ Pos ].Element = Key; /* Probably need strcpy! */
	}
}
 
HashTable Rehash( HashTable H )
{
	int i, OldSize;
	Cell *OldCells;
 
	OldCells = H->TheCells;
	OldSize = H->TableSize;
 
	/* Get a new, empty table */
	H = InitializeTable( 2 * OldSize );
	
	/* Scan through old table, reinserting into new */
	for( i = 0; i < OldSize; i++ )
		if ( OldCells[ i ].Info == Legitimate )
			Insert( OldCells[ i ].Element, H );
 
	free( OldCells );
 
	return H;
}
 
ElementType Retrieve( Position P, HashTable H )
{
	return H->TheCells[ P ].Element;
}
 
void DestroyTable( HashTable H )
{
	free( H->TheCells );
	free( H );
}