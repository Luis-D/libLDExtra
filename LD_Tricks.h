//A compilation of "tricks" and "hacks" to make programming easy and lazy.
//  Luis Delgado. 2018

#ifndef __LD_TRICKS_H_
#define __LD_TRICKS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define _new(type,var_counts) (type*) malloc(sizeof(type) * var_counts)
#define _resize(type,pointer,var_counts) (type*) realloc(pointer,sizeof(type)*var_counts)
#define _repeat(counter,times) for(int counter = times;i--;)
#define _swap(a,b) a = a ^ b; b = a ^ b; a = a ^ b
#define _zero(a) a ^= a
#define _NULL(a) _zero(*(int*)&a)
#define _test(var,bitmask) ((var & bitmask) == bitmask)
#define _checkbits(var,bitmask) _test(var,bitmask)

#define array_range(type,Item,from,to)\
    for (type * Item = from; Item<=to; (Item)++)
#define array_range_reverse(type,Item,from,to)\
    for (type * Item = from; Item>=to; (Item)--)
#define array_range_omni(type,Item,from,to)\
    for (type * Item = from; Item!=to+(from<=to); (Item)+=((from<=to) - (!(from<=to))))

#define array_foreach(type,Item,Array,Array_Size)\
    for(type * Item = Array; Item <(Array+Array_Size);(Item)++)
#define array_foreach_reverse(type,Item,Array,Array_Size)\
    for(type * Item = (Array+(Array_Size-1));Item>=Array;(Item)--)

#define printV4(x) printf("(%f,%f,%f,%f)",x[0],x[1],x[2],x[3])
#define printV3(x) printf("(%f,%f,%f)",x[0],x[1],x[2])
#define printV2(x) printf("(%f,%f)",x[0],x[1])
#define printM4(x) \
    printf("(%f,%f,%f,%f)",x[0],x[1],x[2],x[3]); \
    printf("(%f,%f,%f,%f)",x[4],x[5],x[6],x[7]); \
    printf("(%f,%f,%f,%f)",x[8],x[9],x[10],x[11]); \
    printf("(%f,%f,%f,%f)",x[12],x[13],x[14],x[15]) 

#endif
