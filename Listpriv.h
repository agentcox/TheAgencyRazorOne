//--------------------------------------------------------------------------------------------
// Project:	The Capricorn Document by Team V
// Team:		Charles Cox, Dave Kondor, Dave Wilson, Hideki Saito, Paul Sequeira, Wyatt Jackson
// Desc:		Linked List Library - Private Declarations
// Authors:	Dave Wilson (initial design and code)
//				Wyatt Jackson (comments and tweaks)
// File:		listpriv.h
// Changed:	01-24-2000
//--------------------------------------------------------------------------------------------
#ifndef LISTPRIV_H
#define LISTPRIV_H


//--------------------------------------------------------------------------------------------
// Struct:	 __NODE, NODE, *PNODE
// Purpose:  This is fundamental unit of the doubly linked list.  All data for the list is
//				stored in this base unit.  The object is of POTYPE defined in potype.h. In theory,
//				object should be able to handle any data type.
//
// Members:	POTYPE		object		- The object being stored
//				PNODE			nextnode		- Pointer to the next node
//				PNODE			prevnode		- Pointer to the previous node
//--------------------------------------------------------------------------------------------
typedef struct __NODE{
	POTYPE			object;			// The object being stored
	struct __NODE	*nextnode;		// Pointer to the next node
	struct __NODE	*prevnode;		// Pointer to the previous node
}NODE, *PNODE;

//--------------------------------------------------------------------------------------------
// Struct:	 __LIST, LIST, *PLIST
// Purpose:  This structure is what connects the NODE's together. The LIST structure keeps
//				track of the number of objects (NODES) as well as pointers to the head,tail, and
//				current pointers.
//
// Members:	int			objcnt		- The number of objects in the list
//				PNODE			head			- Pointer to the the beginning of the list
//				PNODE			tail			- Pointer to the end of the list
//				PNODE			current		- Pointer to the current position in the list
//--------------------------------------------------------------------------------------------
typedef struct __LIST{
	int			objcnt;	// The number of objects in the list
	PNODE			head;		// Pointer to the the beginning of the list
	PNODE			tail;		// Pointer to the end of the list
	PNODE			current;	// Pointer to the current position in the list
}LIST,*PLIST;

#endif