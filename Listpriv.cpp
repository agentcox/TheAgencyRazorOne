//--------------------------------------------------------------------------------------------
// Project:	The Capricorn Document by Team V
// Team:		Charles Cox, Dave Kondor, Dave Wilson, Hideki Saito, Paul Sequeira, Wyatt Jackson
// Desc:		Linked List Library - Private Implementation
// Authors:	Dave Wilson (initial design and code)
//			Wyatt Jackson (comments and tweaks)
// File:	listpriv.c
// Changed:	02-25-00 Added deleteObj
//
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//                                 #includes
//--------------------------------------------------------------------------------------------
#include "agencyinclude.h"


//--------------------------------------------------------------------------------------------
// Function: createList()
// Purpose:  Initializes a new linked list object
//
// Inputs:	 None
//
// Returns:  (Success) Pointer to a linked list object
//			 (Failure) NULL if error creating list pointer
//
// Written by:	Dave Wilson (initial design and code)
//				Wyatt Jackson (comments and tweaks)
//--------------------------------------------------------------------------------------------
PLIST createList() {
	PLIST objList = (PLIST) malloc(sizeof(LIST)); // Allocate memory for pointer
	if (objList == NULL) {
		return NULL; // Return NULL if memory wasn't allocated
	}
	objList->head = objList->tail = objList->current = NULL; // Set list head, tail, and current to NULL
	objList->objcnt = 0; // Set list count to zero
	return objList; // Return list pointer
}


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
//
// Written by:	Dave Wilson (initial design and code)
//				Wyatt Jackson (comments and tweaks)
//--------------------------------------------------------------------------------------------
int addtoList(PLIST objList, POTYPE newObject, COMPARE Compare) {
	PNODE	pNewNode;
	if (checkforObject(objList, &newObject, Compare) > -2) {
		return -1; // Object failed COMPARE function and was not added
	}
	// The head of the list exists add the new node to the end
	if (objList->head != NULL) {
		objList->current = objList->tail; // Set current object to list tail
		pNewNode = (PNODE)malloc(sizeof(NODE)); // Allocate memory for the new node
		if (pNewNode == NULL) {
			return -2; // Unable to allocate memory for the new node
		}
		pNewNode->nextnode = NULL; // Set the next pointer to NULL to signify end of list
		pNewNode->prevnode = objList->tail; // Set previous pointer to former end of list
		pNewNode->object = newObject; // Associate the object with this node
		objList->current->nextnode = pNewNode; // Set the new node to be the next in the current list
		objList->tail = pNewNode; // Set the end of the curent list to point to this node
		objList->current = pNewNode; // Set current node to this node
		objList->objcnt++; // Icrement the list count by 1
		return 0; // Everything super fine - we're good to go
	}
	// The head of the list doesn't exist set the new node to be the head
	if (objList->head == NULL) {
		pNewNode = (PNODE)malloc(sizeof(NODE)); // Allocate memory for the new node
		if (pNewNode == NULL) {
			return -2; // Unable to allocate memory for the new node
		}
		pNewNode->object = newObject;	//	Associate the object with this node
		pNewNode->nextnode = NULL;		//	Initial Set to NULL
		pNewNode->prevnode = NULL;		//	Initial Set to NULL
		objList->head = pNewNode;		//	Set the new node to be the head of the current list
		objList->tail = pNewNode;		//	Set the end of the curent list to point to this node
		objList->current = pNewNode;	//	Set current node to this node
		objList->objcnt++; // Icrement the list count by 1
		return 0; // All systems go
	}
	return -3; // A very bad thing happened - U.R.E. (unidentified runtime error)
}


//--------------------------------------------------------------------------------------------
// Function: freeList(PLIST objList)
// Purpose:  Frees memory used by the list
//
// Inputs:	 PLIST		objList		- Pointer to list to de-allocate
//
// Returns:  (Success)  0 if list was de-allocated successfully
//				 (Failure) -1 if the function was passed a NULL list
//							  -2 if the end of the list is NULL (can't free NULL memory)
//
// Written by:	Dave Wilson (initial design and code)
//				Wyatt Jackson (comments and tweaks)
//--------------------------------------------------------------------------------------------
int freeList(PLIST objList){
	if(objList == NULL) {
		return -1; // The function was passed a NULL list
	}
	objList->current = objList->tail; // Set current node to point to the tail of the list
	// Cycle through list draining it until the list count is zero
	while (objList->objcnt != 0) {
		objList->current = objList->tail->prevnode; // Set the current node to be the second to last node
		if(objList->tail == NULL) {
			return -2; // The end of the list is NULL (can't free NULL memory)
		}
		free(objList->tail); // De-allocate the end of the list
		objList->tail = objList->current; // Set the end of the list to be the current node
		objList->objcnt--; // Decrement the list count by 1
	}
	return 0; // Everything's Super!
}


//--------------------------------------------------------------------------------------------
// Function: dumpList(PLIST objList, EVALUATE Evaluate, int rate)
// Purpose:  Dumps all objects to an evaluate function for printing and the like
//
// Inputs:	 PLIST		objList		- Pointer to list to dump
//				 EVALUATE	Evaluate		- Function to use for evaluation
//				 int			rate			- Rate at which to dump list (if <=0 no pause)
//
// Returns:	(Success)	0  if list was successfully dumped
//			(Failure)	-1 if the function was passed a NULL list
//						-2 if the evaluation function was NULL
//						-3 if a very bad thing happened - U.R.E. (unidentified runtime error)
//
// Written by:	Dave Wilson (initial design and code)
//				Wyatt Jackson (comments and tweaks)
//--------------------------------------------------------------------------------------------
int dumpList(PLIST objList,EVALUATE Evaluate, int rate) {
	int cnt = 0; // Set counter to zero
	if(objList == NULL) {
		return -1; // The function was passed a NULL list
	}
	objList->current = objList->head; // Set current node to the list head
	if (Evaluate != NULL) {
		// Cycle through each object in the list
		while (cnt < objList->objcnt) {
			Evaluate(&objList->current->object); // Run the object through the evaluate function
			objList->current = objList->current->nextnode; // Set current node to the next node
			cnt++; // Increment the counter by 1
			if((rate > 0) && (cnt%rate == 0)) {
				getch(); // Pause if rate was specified (may need to change according to UI structure)
			}
		}
	}
	else {
		return -2; // The evaluation function was NULL
	}
	return 0; // It's all good
}


//--------------------------------------------------------------------------------------------
// Function: checkforObject(PLIST objList, PPOTYPE Object, COMPARE Compare)
// Purpose:  Checks for an object in the given list and returns accordingly
//
// Inputs:	 PLIST		objList		- Pointer to list to dump
//				 PPOTYPE		Object		- Pointer to the object to check for
//				 COMPARE		Compare		- Function to use for comparison
//						Compare return values:	< 0 if object1 is less than object2
//												= 0 if object1 is equal to object2
//												> 0 if object1 is greater than object2
//
// Returns:	(Success)	i position of duplicate in list if Compare returns 0
//			(Failure)	-1 if the function was passed a NULL list
//						-2 if no comparable object was found
//
// Written by:	Dave Wilson (initial design and code)
//				Wyatt Jackson (comments and tweaks)
//--------------------------------------------------------------------------------------------
int checkforObject(PLIST objList, PPOTYPE Object, COMPARE Compare){
	int i = 0; // Initialize counter to zero
	if(objList == NULL) {
		return -1; // The function was passed a NULL list
	}
	objList->current = objList->head; // Set current node to the list head
	// Cycle through objects and compare them
	while(i < objList->objcnt) {
		if(Compare != NULL) { //&& (Compare != (unsigned long)3435973836) && (Compare != (unsigned long)1836347904)) {
			if (Compare(&objList->current->object,Object) == 0) {
				return i; // position of duplicate in list if Compare returns 0
			}
			else {
				objList->current = objList->current->nextnode; // Set current node to the next node in the list
			}
		}
		i++; // Increment the counter
	}
	return -2; // No comparable object was found
}


//--------------------------------------------------------------------------------------------
// Function: findObject(PLIST objList, PPOTYPE Object, COMPARE Compare)
// Purpose:  Finds an object in the current list and returns a pointer to it if found
//
// Inputs:	PLIST		objList		- Pointer to list to search
//			PPOTYPE		Object		- Pointer to the object to find
//			COMPARE		Compare		- Function to use for comparison
//				Compare return values:	< 0 if object1 is less than object2
//										= 0 if object1 is equal to object2
//										> 0 if object1 is greater than object2
//
// Returns:	(Success) PPOTYPE pointer to the object that was found
//			(Failure) NULL if the function was passed a NULL list or no matching object was found
//
// Written by:	Wyatt Jackson (initial design and comments)
//--------------------------------------------------------------------------------------------
PPOTYPE findObject(PLIST objList, PPOTYPE Object, COMPARE Compare){
	int i = 0; // Initialize counter to zero
	if(objList == NULL) {
		return NULL; // The function was passed a NULL list
	}
	objList->current = objList->head; // Set current node to the list head
	// Cycle through objects and compare them
	while(i < objList->objcnt) {
		if (Compare(&objList->current->object,Object) == 0){
			return &objList->current->object; // Pointer to the object that was found
		}
		else {
			objList->current = objList->current->nextnode; // Set current node to the next node in the list
		}
		i++; // Increment the counter
	}
	return NULL; // The object was not found
}

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
int deleteObject(PLIST pObjList, PPOTYPE pObject){
	PNODE pPrevNode;	//The node "prevnode" in the found object node
	PNODE pNextNode;	//The node "nextnode" in the found object node
	PNODE pTempNode;	//Node to be freed.

	//List has nothing in it.
	if (pObjList == NULL) {
		return (-1);
	}
	//Start traversal of the list at head
	pObjList->current = pObjList->head;

	//Find the Object in the List.
	while ((&(pObjList->current->object) != (pObject))  && (pObjList->current != NULL)) {
		pObjList->current = pObjList->current->nextnode;
	}
	if (pObjList->current == NULL) {
		return (-2);
	}
	pPrevNode = pObjList->current->prevnode;
	pNextNode = pObjList->current->nextnode;
	if ((pPrevNode == NULL) && (pNextNode == NULL) && (pObjList->current != NULL)) {
		//The list consists of only one object
		pTempNode = pObjList->current;
		pObjList->current = NULL;
		pObjList->head = NULL;
		pObjList->tail = NULL;
		pObjList->objcnt = 0;
		memset(&pTempNode->object,0,sizeof(POTYPE));	//garbage collector
		free(pTempNode);
		return (0);
	}
	if (pPrevNode == NULL) {		//pObjList->current->object is at the head
		pTempNode = pObjList->current;
		pObjList->current = pNextNode;
		pObjList->current->prevnode = NULL;
		pObjList->head = pObjList->current;
		pObjList->objcnt--;
		memset(&pTempNode->object,0,sizeof(POTYPE));	//garbage collector
		free(pTempNode);
		return (0);
	}
	if (pNextNode == NULL) {		//pObjList->current->object is at the tail
		pTempNode = pObjList->current;
		pObjList->current = pPrevNode;
		pObjList->current->nextnode = NULL;
		pObjList->tail = pObjList->current;
		pObjList->objcnt--;
		memset(&pTempNode->object,0,sizeof(POTYPE));	//garbage collector
		free(pTempNode);
		return (0);
	}
	if ((pNextNode != NULL) && (pPrevNode != NULL) && (pObjList->current != NULL)) {
		//Somewhere in the list other than head or tail
		pTempNode = pObjList->current;
		pObjList->current = pObjList->head;  //done with current. reset!
		pNextNode->prevnode = pPrevNode;
		pPrevNode->nextnode = pNextNode;
		pObjList->objcnt--;
		memset(&pTempNode->object,0,sizeof(POTYPE));	//garbage collector
		free(pTempNode);
		return (0);
	}
	else
		return (-3);
}

PNODE GetNextListEntry(PNODE previous)
{
	if (previous->nextnode == NULL){
		return NULL;
	}
	return previous->nextnode;
}
/*
void sortList(PLIST objlist, COMPARE Compare, SWAP InternalSwapFunction)
{

	PLIST walker = objlist;
	bool Sorted = true;
	int RetValue;
	int Counter = 0;

	if(!Compare){
		return;
	}

	if(objlist == NULL || objlist->objcnt < 2){
		return;
	}

	do
	{
		Sorted = true; //ASSUME SORTED IS TRUE UNLESS OTHERWISE DISCOVERED.
		for(walker->current = walker->head; walker->current->nextnode != NULL; walker->current = walker->current->nextnode, Counter++)
		{
			
			RetValue = Compare(&walker->current->object, &walker->current->nextnode->object);
			//IF COMPARE RETURNS 0, EQUAL. OK.
			//IF COMPARE RETURNS > 0, SECOND COMES AFTER FIRST. OK.
			
			if(RetValue < 0 && walker->current && walker->current->nextnode){//IF COMPARE RETURNS < 0, FIRST COMES AFTER SECOND. BAD! SWAP.
				Sorted = false; //WE'LL HAVE TO RUN THROUGH AGAIN.
					if(InternalSwapFunction){
				InternalSwapFunction(&walker->current->object, &walker->current->nextnode->object);
				}
				swapNodes(walker->current, walker->current->nextnode);
			
			}
		}
	}while(!Sorted);
	return;
}

void swapNodes(PNODE first, PNODE second)
{
	if(second == NULL || first == NULL){
		return;
	}
	
	first->nextnode = second->nextnode;
	second->prevnode = first->prevnode;
	first->prevnode = second;
	second->nextnode = first;

	//CHANGE THE BOUNDING NODES.
	if(first->prevnode != NULL){
		first->prevnode->nextnode = second;
	}
	if(second->nextnode != NULL){
		second->nextnode->prevnode = first;
	}
}
*/

void sortList(PLIST list, COMPARE SwapCompare, SWAP InternalSwapFunction, COMPARE ListInsertCompare)
{
	if(!list || list->objcnt < 2){
		return;
	}
	int i = 0;
	PLIST Walker = list;
	POTYPE Temp;
		bool Sorted = true;
	int RetValue;
	int Counter = 0;

	if(!SwapCompare || !ListInsertCompare){
		return;
	}
	
	PPOTYPE Array = (PPOTYPE)malloc(sizeof(POTYPE) * list->objcnt); //ARRAY OF POTYPES
	int ObjectCount = list->objcnt;
	for(i = 0, Walker->current = Walker->head; i < ObjectCount, Walker->current != NULL; i++, Walker->current = Walker->current->nextnode){
		if(Walker->current->prevnode){
			deleteObject(list, &Walker->current->prevnode->object);
		}
		Array[i] = Walker->current->object;
	}
	deleteObject(list, &Walker->head->object);

		//We have an array. Run through and swap the segments.
	do
	{
		Sorted = true; //ASSUME SORTED IS TRUE UNLESS OTHERWISE DISCOVERED.
		for(i= 0; i < ObjectCount - 1; i++)
		{
			
			RetValue = SwapCompare(&(Array[i]), &(Array[i+1]));
			//IF COMPARE RETURNS 0, EQUAL. OK.
			//IF COMPARE RETURNS > 0, SECOND COMES AFTER FIRST. OK.
			
			if(RetValue < 0){//IF COMPARE RETURNS < 0, FIRST COMES AFTER SECOND. BAD! SWAP.
				
				Sorted = false; //WE'LL HAVE TO RUN THROUGH AGAIN.
				
					if(InternalSwapFunction){
				InternalSwapFunction(&(Array[i]), &(Array[i+1]));
				}
				Temp = Array[i];
				Array[i] = Array[i+1];
				Array[i+1] = Temp;
			}
		}
	}while(!Sorted);
	
	//ARRAY IS SORTED. RE-INSERT INTO LIST.
	for(i = 0; i < ObjectCount; i++){
	Temp = Array[i];
	addtoList(list, Temp, ListInsertCompare);
	}
	free(Array);
	
	return;

}