// To make and handle Dynamic stacks and Dynamic queues
// These Data structures are in fact Linked Lists
// These instructions are an abstraction layer over the Linked Lists
// Luis Delgado. 2018

#ifndef _LD_STACK_QUEUE_H_
#define _LD_STACK_QUEUE_H_

#include "LD_LinkedList.h"

    /*** Stack ***/
#define Stack_Create LinkedList_Create
#define Stack(Datatype) DoublyLinkedList(Datatype)
#define Stack_Top(stack) DoublyLinkedList_Last(stack)
#define Stack_Head(stack) Stack_Top(stack)
#define Stack_Bottom(stack) DoublyLinkedList_First(stack)
#define Stack_Tail(stack) Stack_Bottom(stack)
#define Stack_Length(stack) LinkedList_Length(stack)
#define Stack_push(stack,data) DoublyLinkedList_add(stack,data)
#define Stack_pop(stack) DoublyLinkedList_remove(stack,Stack_Head(stack),NULL)
#define Stack_foreach(Item,stack) DoublyLinkedList_foreach_reverse(Item,stack)
#define Stack_Destroy(stack) DoublyLinkedList_Destroy(stack,NULL)
    /*************/

    /*** Queue ***/
#define Queue_Create Stack_Create //<- Is the same code
#define Queue(Datatype) SinglyLinkedList(Datatype)
#define Queue_Top(queue) SinglyLinkedList_First(queue)
#define Queue_Head(queue) Queue_Top(queue)
#define Queue_Bottom(queue) SinglyLinkedList_Last(queue)
#define Queue_Tail(queue) Queue_Bottom(queue)
#define Queue_Length(queue) LinkedList_Length(queue)
#define Queue_enqueue(queue,data) SinglyLinkedList_add(queue,data)
#define Queue_dequeue(queue) SingleLinkedList_remove(queue,Queue_Head(queue),NULL)
#define Queue_foreach(Item,queue) SinglyLinkedList_foreach(Item,queue)
#define Queue_Destroy(queue) SinglyLinkedList_Destroy(queue,NULL)

	/** Priority Queue **/
void * _Queue_priority_enqueue(void * queue,void * data_ptr, uintptr_t Data_Byte_Size,
    char (*comparition_callback) (void*,void*))
{
    if(Queue_Length(queue) > 0)
    {
	void * Insertion = NULL;
	Queue_foreach(Item,queue)
	{
	    if(comparition_callback(data_ptr,Item))
	    {
		return S_LinkedList_insert(queue,data_ptr,Data_Byte_Size,Insertion);
	    }
	    Insertion = Item;
	}
    }
    return S_LinkedList_add(queue,data_ptr,Data_Byte_Size);
}
#define Queue_priority_enqueue(queue,Data,Function)\
    _Queue_priority_enqueue(queue,&Data,sizeof(Data),Function)
	/********************/

    /*************/

#endif
