//--------------------------------------------------------------------------------------------
// Project:	The Capricorn Document by Team V
// Team:		Charles Cox, Dave Kondor, Dave Wilson, Hideki Saito, Paul Sequeira, Wyatt Jackson
// Desc:		Linked List Library - Public Declarations
// Authors:	Dave Wilson (initial design and code)
//				Wyatt Jackson (comments and tweaks)
// File:		listpub.h
// Changed:	01-25-2000
//--------------------------------------------------------------------------------------------
#ifndef LISTPUB_H
#define LISTPUB_H

typedef struct __LIST *PLIST; // An opaque structure to the private __LIST structure

//--------------------------------------------------------------------------------------------
// Function: EVALUATE(PPOTYPE Object)
// Purpose:  A function pointer to print or evaluate an object
//
// Inputs:	 PPOTYPE		Object		- Pointer to the object to check for
//
// Returns:  (Success) None
//				 (Failure) None
//--------------------------------------------------------------------------------------------
typedef void ((*(EVALUATE))(PPOTYPE Object));

//--------------------------------------------------------------------------------------------
// Function: COMPARE(PPOTYPE Object1, PPOTYPE Object2)
// Purpose:  A function pointer to compare two objects
//
// Inputs:	 PPOTYPE		Object1		- Pointer to the first object to compare
//				 PPOTYPE		Object2		- Pointer to the second object to compare
//
// Returns:  < 0 if Object1 is less than Object2
//				 = 0 if Object1 is equal to Object2
//				 > 0 if Object1 is greater than Object2
//--------------------------------------------------------------------------------------------
typedef int ((*(COMPARE))(PPOTYPE Object1,PPOTYPE Object2));

//--------------------------------------------------------------------------------------------
// Function: createList()
// Purpose:  Initializes a new linked list object
//
// Inputs:	 None
//
// Returns:  (Success) Pointer to a linked list object
//				 (Failure) NULL if error creating list pointer
//--------------------------------------------------------------------------------------------
PLIST createList();

//--------------------------------------------------------------------------------------------
// Function: addtoList(PLIST objList, POTYPE newObject, COMPARE Compare)
// Purpose:  Adds a new object to the list based on the provided COMPARE function
//
// Inputs:	 PLIST		objList		- Pointer to list to add to
//				 POTYPE		newObject	- Object to be added
//				 COMPARE		Compare		- Function to use for comparison
//												  Compare return values: < 0 if object1 is less than object2
//																				 = 0 if object1 is equal to object2
//																				 > 0 if object1 is greater than object2
//
// Returns:  (Success)  0 if new object added successfully
//				 (Failure) -1 if object failed COMPARE function and was not added
//							  -2 if unable to allocate memory for the new node
//							  -3 if a very bad thing happened - U.R.E. (unidentified runtime error)
//--------------------------------------------------------------------------------------------
int addtoList(PLIST objList, POTYPE newobj,COMPARE Compare);

//--------------------------------------------------------------------------------------------
// Function: freeList(PLIST objList)
// Purpose:  Frees memory used by the list
//
// Inputs:	 PLIST		objList		- Pointer to list to de-allocate
//
// Returns:  (Success)  0 if list was de-allocated successfully
//				 (Failure) -1 if the function was passed a NULL list
//							  -2 if the end of the list is NULL (can't free NULL memory)
//--------------------------------------------------------------------------------------------
int freeList(PLIST objList);

//--------------------------------------------------------------------------------------------
// Function: dumpList(PLIST objList, EVALUATE Evaluate, int rate)
// Purpose:  Dumps all objects to an evaluate function for printing and the like
//
// Inputs:	 PLIST		objList		- Pointer to list to dump
//				 EVALUATE	Evaluate		- Function to use for evaluation
//				 int			rate			- Rate at which to dump list (if <=0 no pause)
//
// Returns:  (Success)  0 if list was successfully dumped
//				 (Failure) -1 if the function was passed a NULL list
//							  -2 if the evaluation function was NULL
//							  -3 if a very bad thing happened - U.R.E. (unidentified runtime error)
//--------------------------------------------------------------------------------------------
int dumpList(PLIST objList,EVALUATE Evaluate, int rate);

//--------------------------------------------------------------------------------------------
// Function: checkforObject(PLIST objList, PPOTYPE Object, COMPARE Compare)
// Purpose:  Checks for an object in the given list and returns accordingly
//
// Inputs:	 PLIST		objList		- Pointer to list to dump
//				 PPOTYPE		Object		- Pointer to the object to check for
//				 COMPARE		Compare		- Function to use for comparison
//												  Compare return values: < 0 if object1 is less than object2
//																				 = 0 if object1 is equal to object2
//																				 > 0 if object1 is greater than object2
//
// Returns:  (Success)  i position of duplicate in list if Compare returns 0
//				 (Failure) -1 if the function was passed a NULL list
//							  -2 if no comparable object was found
//--------------------------------------------------------------------------------------------
int checkforObject(PLIST objList, PPOTYPE object, COMPARE Compare);

//--------------------------------------------------------------------------------------------
// Function: findObject(PLIST objList, PPOTYPE Object, COMPARE Compare)
// Purpose:  Finds an object in the current list and returns a pointer to it if found
//
// Inputs:	 PLIST		objList		- Pointer to list to search
//				 PPOTYPE		Object		- Pointer to the object to find
//				 COMPARE		Compare		- Function to use for comparison
//												  Compare return values: < 0 if object1 is less than object2
//																				 = 0 if object1 is equal to object2
//																				 > 0 if object1 is greater than object2
//
// Returns:  (Success) PPOTYPE pointer to the object that was found
//				 (Failure) NULL if the function was passed a NULL list or no matching object was found
//--------------------------------------------------------------------------------------------
PPOTYPE findObject(PLIST objList, PPOTYPE Object, COMPARE Compare);
//--------------------------------------------------------------------------------------------
// Function: deleteObject(PLIST objList, PPOTYPE Object)
// Purpose:  Deletes & frees an object in the given list
//
// Inputs:	PLIST		objList		- Pointer to list the object to delete is in.
//			PPOTYPE		Object		- Pointer to the object to delete.
//
// Returns:	(Success)	0	Object was found, deleted, and freed.  List reconnected.
//			(Failure)	-1	No List to search.
//						-2	No pObject was found.  Use findObject() first.
//						-3	U.R.E (unidentified runtime error
//
//Written by: Dave Wilson (initial design and comments)						
//--------------------------------------------------------------------------------------------

int deleteObject(PLIST pObjList, PPOTYPE pObject);

PNODE GetNextListEntry(PNODE previous);
typedef void ((*(SWAP))(PPOTYPE Object1,PPOTYPE Object2)); //A FUNCTION POINTER TO SWAP OBJECT INTERNALS
void sortList(PLIST list, COMPARE SwapCompare, SWAP InternalSwapFunction, COMPARE ListInsertCompare);
//void swapNodes(PNODE first, PNODE second);


#endif