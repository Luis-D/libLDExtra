//A compilation of "tricks" and "hacks" to make programming easy and lazy.
//  Luis Delgado. 2018

#ifndef __LD_TRICKS_H_
#define __LD_TRICKS_H_

#include <stdio.h>
#include <stdlib.h>

#define _new(type,var_counts) (type*) malloc(sizeof(type) * var_counts)
#define _resize(type,pointer,var_counts) (type*) realloc(pointer,sizeof(type)*var_counts)
#define _repeat(counter,times) for(int counter = times;i--;)
#define _swap(a,b) a = a ^ b; b = a ^ b; a = a ^ b

#define array_foreach(type,Item,Array,Array_Size)\
    for(type * Item = Array; Item <(Array+Array_Size);(Item)++)
#define array_foreach_reverse(type,Item,Array,Array_Size)\
    for(type * Item = (Array+(Array_Size-1));Item>=Array;(Item)--)


#endif
