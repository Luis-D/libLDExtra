#include "LD_Tricks.h"
// MACROS and functions to make and handle Linked Lists
// (Singly and Doubly Linked Lists)
// It's a kind of template library
// Luis Delgado. 2018

/***********************************************************************
MIT License

Copyright (c) 2018 Luis Delgado

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITD TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

#ifndef _LD_LINKEDLISTS_H_
#define _LD_LINKEDLISTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/******************************************************/
#define G_LinkedList_Node_Define(Datatype) void
#define S_LinkedList_Node_Define(Datatype) \
    struct {void * Next; Datatype Data;}
#define D_LinkedList_Node_Define(Datatype) \
    struct {void * Previous; void * Next; Datatype Data;}

#define _LinkedList_Define(Datatype,NodeType)	    \
    struct {					    \
	    NodeType(Datatype) * First;		    \
	    NodeType(Datatype) * Last;		    \
	    uintptr_t length;		    \
    }

#define G_LinkedList_Reset(List_ptr)		    \
    __gllptr(List_ptr)->First=NULL;	    \
    __gllptr(List_ptr)->Last=NULL;  \
    __gllptr(List_ptr)->length=0;   
    
/******************************************************/

#define __gll_g _LinkedList_Define(void*,G_LinkedList_Node_Define)
#define __gllptr(x) ((__gll_g*) x)
#define __gll_s_ptr(x) ((S_LinkedList_Node_Define(void*)*) x)
#define __gll_d_ptr(x) ((D_LinkedList_Node_Define(void*)*) x)


void * LinkedList_Create()
{
    void * RET= malloc(sizeof(__gll_g));
    if(RET==NULL){return NULL;}
    G_LinkedList_Reset(RET)    
    return RET;
}

static void * __LinkedList_node_allocate
(void * Data, uintptr_t Data_Byte_Size, uintptr_t Data_Offset)
{
    void * tmp = malloc(Data_Byte_Size+(sizeof(void*)*Data_Offset));
    if(tmp==NULL){return NULL;}  
    if(Data!=NULL){memcpy(((uintptr_t*) tmp)+Data_Offset, Data, Data_Byte_Size);}
    return tmp;
}


#define __Generic_LinkedList_add(List,Data,Size,tmp,__gll,__type)   \
    tmp = __LinkedList_node_allocate(Data, Data_Byte_Size,__type);  \
    if(tmp==NULL){return NULL;}					    \
    (__gllptr(List)->length)++;					    \
    if(__gllptr(List)->First==NULL){__gllptr(List)->First=tmp;}	    \
    else{__gll(__gllptr(List)->Last)->Next=tmp;}	    

//Add a value to a singly linked list
void * S_LinkedList_add(void * List, void * Data,uintptr_t Data_Byte_Size)
{
    void * tmp;
    __Generic_LinkedList_add(List,Data,Size,tmp,__gll_s_ptr,1);
    __gll_s_ptr(tmp)->Next=NULL;
    __gllptr(List)->Last = tmp;
    return &(__gll_s_ptr(tmp)->Data); //<- It returns a pointer to the data section of the node;
}

//Add a value to a doubly linked list
void * D_LinkedList_add(void * List, void * Data, uintptr_t Data_Byte_Size)
{
    void * tmp;
    __Generic_LinkedList_add(List,Data,Size,tmp,__gll_d_ptr,2);
    __gll_d_ptr(tmp)->Previous = __gllptr(List)->Last;
    __gll_d_ptr(tmp)->Next=NULL;
    __gllptr(List)->Last = tmp;
    return &(__gll_d_ptr(tmp)->Data); //<- It returns a pointer to the data section of the node;
}

//Given a data pointer of a node, this functions starts deleting secuent nodes
//If a function is given, it is executed for each node data section.
void _LinkedList_remove_continuous(void * Node, void (*Erasing_Function) (void *), void * List_ptr,uintptr_t Data_Offset,uintptr_t Next_Offset)
{
    //It go backwards sizeof(uintptr_t)*Data_Offset bytes to get the memory OFFSET of the node
    uintptr_t * _Node_ = ((uintptr_t*)Node) - Data_Offset;
    void * tmp;   
 
    uintptr_t Count =0;

    //This way the comparing instruction can be avoided inside the loop. 
    if(Erasing_Function==NULL)
    {
	while(_Node_!=NULL)
	{
	    tmp = (void*) _Node_[Data_Offset-Next_Offset]; //<- Getting the NEXT
	    free(_Node_);
	    _Node_=tmp;
	    Count++;
	}
    }
    else
    {
	while(_Node_!=NULL)
	{
	    tmp = (void*) _Node_[Data_Offset-Next_Offset]; //<- Getting the NEXT
	    Erasing_Function(((uintptr_t*)_Node_)+Data_Offset); //<- Sending the Data section
	    free(_Node_);
	    _Node_=tmp;
	    Count++;
	}
    }

    if(List_ptr != NULL){
	(__gllptr(List_ptr)->length)-=Count;
	if(__gllptr(List_ptr)->First == (((uintptr_t*)Node) - Data_Offset))
	{__gllptr(List_ptr)->First=NULL;}
    }
}

void D_LinkedList_remove(void * List, void * Node, void (*Deleting_Function) (void*))
{
    void * Curr = (((uintptr_t*) Node)-2);
    
    if(Deleting_Function != NULL){Deleting_Function(Curr);}    

    void * Previous = (void*) *((uintptr_t*)Curr);
    void * Next =(void*) ((uintptr_t*)Curr)[1];
 
    if(Previous!=NULL)	{__gll_d_ptr(Previous)->Next=Next;}    
    if(Next!=NULL)	{__gll_d_ptr(Next)->Previous=Previous;} 

    if(List!=NULL)
    {
	char Is_First = Previous == NULL ;
	char Is_Last = Next==NULL;
	
	if(Is_First)
	{__gllptr(List)->First = Next;} 

	if(Is_Last)
	{__gllptr(List)->Last = Previous;}

	(__gllptr(List)->length)--;
    }

    free(Curr);
}

void * S_LinkedList_prepend(void * List, void * Data, uintptr_t Data_Size)
{
    void * RET = __LinkedList_node_allocate(Data, Data_Size,1);
    if(RET==NULL){return NULL;}

    __gll_s_ptr(RET)->Next = __gllptr(List)->First;
    __gllptr(List)->First = RET;

    return &__gll_s_ptr(RET)->Data;
}

void * LinkedList_insert_EXT(void * List, void * Data, uintptr_t Data_Size,void * Node_ptr)
{
    if(List!=NULL && Node_ptr ==NULL)
    {
	return S_LinkedList_prepend(List,Data,Data_Size);
    }

    void * Ref_Node =(void*) (((uintptr_t*) Node_ptr)-1);
    void * RET = __LinkedList_node_allocate(Data, Data_Size,1);
    if(RET==NULL){return NULL;}

    __gll_s_ptr(RET)->Next = __gll_s_ptr(Ref_Node)->Next;
    __gll_s_ptr(Ref_Node)->Next = RET;
    
    if(List!=NULL){
	if(Ref_Node == __gllptr(List)->Last){__gllptr(List)->Last = RET;}
	(__gllptr(List)->length)++; 
    }
    return &__gll_s_ptr(RET)->Data;
}

void * DoublyLinkedList_insert_EXT( void * List, void * Data, uintptr_t Data_Size, 
			    void * Node_ptr, uintptr_t Next_Offset)
{
    void * Ref_Node =(void*) (((uintptr_t*) Node_ptr)-2);
    void * RET = __LinkedList_node_allocate(Data, Data_Size,2);
    if(RET==NULL){return NULL;}
    
    uintptr_t Other = Next_Offset ^ 1;
    uintptr_t Next_Other = ((uintptr_t*) Ref_Node)[Other]; 

    ((uintptr_t*)RET)[Next_Offset] =(uintptr_t)  Ref_Node;
    ((uintptr_t*)RET)[Other] = Next_Other;
    
    if(__gllptr(List) != NULL)
    {
	if(Next_Other == (uintptr_t) NULL)
	{
	    if(Next_Offset == 0) //If it was an insert after the reference node...
	    {
		__gllptr(List)->Last = RET;
	    }
	    else    //If it was an insert before the reference node...
	    {
		__gllptr(List)->First = RET;
	    }
	}

	(__gllptr(List)->length)++; 
    }

    return &__gll_d_ptr(RET)->Data;
}

//These MACROS access the pointers located in the same node where the Data is.
//They return a pointer (void*) to the data of the next (or previous) node of the list.
#define Next_to(Data_ptr)	(void*) ((uintptr_t*) (*(((uintptr_t*) Data_ptr)-1))+1)
#define Previous_to(Data_ptr)	(void*) ((uintptr_t*) (*(((uintptr_t*) Data_ptr)-2))+2)

#define _LinkedList_First(List_ptr) (void*) __gllptr(List_ptr)->First
#define _LinkedList_Last(List_ptr) (void*) __gllptr(List_ptr)->Last
#define LinkedList_Length(List_ptr) (uintptr_t) __gllptr(List_ptr)->length
#define LinkedList_Empty(List_ptr) \
    (__gllptr(List_ptr)->First == NULL)

//This macro iterates over the list
#define _LinkedList_foreach(Item,List,__gll,Start_Node,Next_Node,Cond)	    	    \
    for(void * Item##__LD_LL__Foreach__Iterator__ = __gllptr(List)->Start_Node,		    \
      * Item = &(__gll(Item##__LD_LL__Foreach__Iterator__)->Data);			    \
    Item##__LD_LL__Foreach__Iterator__!=Cond;						    \
    Item##__LD_LL__Foreach__Iterator__ = __gll(Item##__LD_LL__Foreach__Iterator__)->Next_Node,	    \
    Item = &__gll(Item##__LD_LL__Foreach__Iterator__)->Data)


    /*** Singly Linked List ***/
#define LinkedList(Datatype) \
    _LinkedList_Define(Datatype,S_LinkedList_Node_Define)
#define LinkedList_add(List_ptr,Data) S_LinkedList_add(List_ptr,&Data,sizeof(Data))   
#define LinkedList_append(List_ptr,Data) LinkedList_add(List_ptr,Data)
#define LinkedList_prepend(List_ptr,Data) S_LinkedList_prepend(List_ptr,&Data,sizeof(Data))
#define LinkedList_insert(list_ptr,data,node_ptr) \
    LinkedList_insert_EXT(list_ptr,&data, sizeof(data),node_ptr)
#define LinkedList_remove(List_ptr,Data_ptr,Function) \
    _LinkedList_remove_continuous(Data_ptr,Function,__gllptr(List_ptr),1,1)
#define LinkedList_clear(List_ptr,Function)	\
    LinkedList_remove(NULL,&__gll_s_ptr(__gllptr(List)->First)->Data,Deleting_Function); \
    G_LinkedList_Reset(List_ptr)
#define LinkedList_Destroy(List_ptr,Function) \
    LinkedList_remove(NULL,&__gll_s_ptr(__gllptr(List_ptr)->First)->Data,Function); \
    free(List_ptr)
#define LinkedList_foreach(Item,List_ptr) _LinkedList_foreach(Item,List_ptr,__gll_s_ptr,First,Next,NULL)
#define LinkedList_First(List_ptr) &__gll_s_ptr(_LinkedList_First(List_ptr))->Data
#define LinkedList_Last(List_ptr) &__gll_s_ptr(_LinkedList_Last(List_ptr))->Data

#define SinglyLinkedList_Create LinkedList_Create
#define SinglyLinkedList(Datatype) LinkedList(Datatype)
#define SinglyLinkedList_add(Data_ptr,Func) LinkedList_add(Data_ptr,Func)
#define SinglyLinkedList_append(List_ptr,Data) SinglyLinkedList_add(List_ptr,Data)
#define SinglyLinkedList_prepend(List_ptr,Data) SinglyLinkedList_prepend(List_ptr,Data)
#define SinglyLinkedList_insert_EXT(List_ptr,Data_ptr,DataSize,node_ptr)\
    LinkedList_insert_EXT(List_ptr,Data_ptr, DataSize,node_ptr)
#define SinglyLinkedList_insert(list_ptr,data,node_ptr) \
    LinkedList_insert(list_ptr,data,node_ptr) 
#define SinglyLinkedList_remove(List_ptr,Data_ptr,Func) LinkedList_remove(List_ptr,Data_ptr,Func)
#define SinglyLinkedList_clear(List_ptr,Func) LinkedList_clear(List_ptr,Func)
#define SinglyLinkedList_Destroy(List_ptr,Func) LinkedList_Destroy(List_ptr,Func)
#define SinglyLinkedList_foreach(Item,List_ptr) LinkedList_foreach(Item,List_ptr)
#define SinglyLinkedList_First(List_ptr) &__gll_s_ptr(_LinkedList_First(List_ptr))->Data
#define SinglyLinkedList_Last(List_ptr) &__gll_s_ptr(_LinkedList_Last(List_ptr))->Data
    /**************************/

    /*** Doubly Linked List ***/ 
#define DoublyLinkedList_First(List_ptr) &__gll_d_ptr(_LinkedList_First(List_ptr))->Data
#define DoublyLinkedList_Last(List_ptr) &__gll_d_ptr(_LinkedList_Last(List_ptr))->Data
#define DoublyLinkedList_Create LinkedList_Create
#define DoublyLinkedList(Datatype) \
    _LinkedList_Define(Datatype,D_LinkedList_Node_Define)
#define DoublyLinkedList_add(List_ptr,Data) D_LinkedList_add(List_ptr,&Data,sizeof(Data))  
#define DoublyLinkedList_append(List_ptr,Data) DoublyLinkedList_add(List_ptr,Data) 
#define DoublyLinkedList_insert_after(List_ptr,Data,Node_ptr) \
    DoublyLinkedList_insert_EXT(List_ptr,&Data,sizeof(Data),Node_ptr,0)
#define DoublyLinkedList_insert_before(List_ptr,Data,Node_ptr) \
    DoublyLinkedList_insert_EXT(List_ptr,&Data,sizeof(Data),Node_ptr,1)
#define DoublyLinkedList_prepend(List_ptr,Data) \
    DoublyLinkedList_insert_before(List_ptr,Data,DoubleLinkedList_First(List_ptr))
#define DoublyLinkedList_remove(List_ptr,Data,Function) \
    D_LinkedList_remove(List_ptr,Data,Function)
#define DoublyLinkedList_remove_onwards(List_ptr,Data_ptr,Function) \
    _LinkedList_remove_continuous(Data_ptr,Function,__gllptr(List_ptr),2,1)
#define DoublyLinkedList_remove_backwards(List_ptr,Data_ptr,Function) \
    _LinkedList_remove_continuous(Data_ptr,Function,__gllptr(List_ptr),2,2)
#define DoublyLinkedList_clear(List_ptr,Function)	\
    DoublyLinkedList_remove_onwards(NULL,&__gll_d_ptr(__gllptr(List_ptr)->First)->Data,Function); \
    G_LinkedList_Reset(List_ptr)
#define DoublyLinkedList_Destroy(List_ptr,Function) \
    DoublyLinkedList_remove_onwards(NULL,&__gll_d_ptr(__gllptr(List_ptr)->First)->Data,Function); \
    free(List_ptr)
#define DoublyLinkedList_foreach(Item,List_ptr) \
    _LinkedList_foreach(Item,List_ptr,__gll_d_ptr,First,Next,NULL)
#define DoublyLinkedList_foreach_reverse(Item,List_ptr) \
    _LinkedList_foreach(Item,List_ptr,__gll_d_ptr,Last,Previous,NULL)
    /**************************/


#endif
