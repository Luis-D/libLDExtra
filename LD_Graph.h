//  To build graphs
//  Luis Delgado. 2018


/*TODO: */
/*
 *  - Node dynamic connection
 *  - Node dynamic desconnection
 * */

#ifndef _LD_GRAPH_H_
#define _LD_GRAPH_H_

#include "LD_LinkedList.h" 

#define _Graph_Node_Define(Datatype) \
    struct{			     \
	unsigned long Degree;   \
	void * Connections_Array;    \
	Datatype Data;		     \
    }

#define _Graph_Define(Datatype)					\
    struct{							\
	SinglyLinkedList(_Graph_Node_Define(Datatype))*Nodes;\
    }

#define __gg_g _Graph_Define(void*)
#define __ggptr(x)	((__gg_g*)x)
#define __gg_n_ptr(x)	((_Graph_Node_Define(void*)*)x)


void * Graph_Create()
{
    void * RET = malloc(sizeof(__gg_g));
    if(RET==NULL){return NULL;}
    __ggptr(RET)->Nodes=LinkedList_Create();
    return RET;
}

static char _Graph_Node_connect_new(void * A, void * B)
{
    void * tmp_A; void * tmp_B = NULL;
    unsigned long Size_A =(__gg_n_ptr(A)->Degree) +1;
    if(__gg_n_ptr(A)-> Degree == 0)
    {
	tmp_A = malloc(sizeof(void*));
	if(tmp_A == NULL) return 0;
    }
    else
    {
	tmp_A = realloc(__gg_n_ptr(A)->Connections_Array,sizeof(void*)*Size_A);
	if(tmp_A == NULL) return 0;
    }
    
    if(B!=NULL)
    {
	    unsigned long Size_B =(__gg_n_ptr(B)->Degree) +1;
	    if(__gg_n_ptr(B)-> Degree == 0)
	    {
		tmp_B = malloc(sizeof(void*));
		if(tmp_B == NULL) return 0;
	    }
	    else
	    {
		tmp_B = realloc(__gg_n_ptr(B)->Connections_Array,sizeof(void*)*Size_B);
		if(tmp_B == NULL) return 0;
	    }

		    
	    ((void **)tmp_B)[Size_B-1] = A;
	    __gg_n_ptr(B)->Connections_Array = tmp_B;
	    (__gg_n_ptr(B)->Degree) = Size_B;
    } 

    ((void **)tmp_A)[Size_A-1] = B;
    __gg_n_ptr(A)->Connections_Array = tmp_A;
    (__gg_n_ptr(A)->Degree) = Size_A;
    return 1;
}

static void __Graph_Node_set(void * RET,void * Data, unsigned long Data_Byte_Size)
{
    __gg_n_ptr(RET)->Degree = 0;
    __gg_n_ptr(RET)->Connections_Array =NULL;

   memcpy(((long*)RET)+2,Data,Data_Byte_Size);
}

void _Graph_Node_deallocate(void * Node)
{
    free(__gg_n_ptr(Node)->Connections_Array);
}

void * _Graph_Node_add(void * Graph,void * Parent,void * Data,unsigned long Data_Byte_Size)
{
    void * _Parent_=NULL;
    if(Parent!=NULL){_Parent_ = ((long*)Parent)-2;} //<- Get node's Head

    unsigned long Node_Size = Data_Byte_Size + sizeof(_Graph_Node_Define(void*)) - sizeof(void*);

    void*RET = S_LinkedList_add(__ggptr(Graph)->Nodes,NULL,Node_Size); //It holds the Node's head;
    if(RET==NULL){return NULL;}
    //The new node is Empty
    
    __Graph_Node_set(RET,Data,Data_Byte_Size); //Set Node data

    _Graph_Node_connect_new(RET,_Parent_); //The Nodes are connected by its heads

    return &__gg_n_ptr(RET)->Data; //<- It returns a pointer to the Node's Data section
}


#define Graph(Datatype) _Graph_Define(Datatype)
#define Graph_add(Graph_ptr,Parent_Node_ptr,Data)\
    _Graph_Node_add(Graph_ptr,Parent_Node_ptr,&Data,sizeof(Data))
#define Graph_Destroy(Graph_ptr)    \
    SinglyLinkedList_Destroy(__ggptr(Graph_ptr)->Nodes,_Graph_Node_deallocate)
#define Graph_Order(Graph_ptr) SinglyLinkedList_Length(__ggptr(Graph_ptr)->Nodes)
#define Graph_Root(Graph_ptr) SinglyLinkedList_First(__ggptr(Graph_ptr)->Nodes)

#endif
