/*
 * Comp220 - hash map.c
 *    Hash Table implementation of a Map ADT
 *
 *   Authors: Reece Whitehead, Artur Shadnik, Greagorey Markerian, Joseph Fall 
 *   Date: April 5, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "hashmap.h"
#include "map.h"

#define COL_SIZE 20
#define ARRAY_SIZE 100

unsigned long htHash( keytype Key);
int htIndex(keytype Key);
entry_t* htBin(map_t map, int row, int col);
int htGetBinIndex(map_t map, int rowIndex, keytype key);
entry_t* htGetBin(map_t map, keytype key);
void htClearBin(entry_t* bin);
bool htBinIsEmpty(entry_t* bin);
void htShiftLeft(map_t* mapref, int row, int col);
keytype keyDeepCopy(keytype key);
bool keysEqual(keytype key1,keytype key2);

/*
 * Authors: Braeden Norman, Artur Shadnik
 * Credit: Hashing algortihm by Dan Bernstein, via http://www.cse.yorku.ca/~oz/hash.html
 * Last Changed: 2018/04/06
 */
unsigned long htHash( keytype key)
{
    unsigned long hash = 5381;
    int c;
    
    while( c = *key++) {
        hash = (( hash<<5 ) + hash) + c;
    }
    return hash;
}


//Author: Greagorey Markerian
// Return the index in the hash table where the given key belongs.
int htIndex(keytype key)
{
    unsigned long hash = htHash(key);
    int index = hash%ARRAY_SIZE;
    return abs(index);  // abs is probably redundant now -- fixed by using unsigned.
}

// Return a pointer to the "bin" in the hashtable at given row, col.
// PRE: 0<= row < ARRAY_SIZE && 0<= col < COL_SIZE
entry_t* htBin(map_t map, int row, int col) 
{
    return &(map->hashtable[row][col]);
}

// Clear the contents from the given bin in the hash table
void htClearBin(entry_t* bin)
{
    bin->key = '\0';
    bin->value = 0;
}

// Return true iff the given hash table bin is empty
bool htBinIsEmpty(entry_t* bin) 
{
    return bin->key == NULL;
} 

// Return index of "bin" in given hashtable row where the key is found, 
//   or index of empty "bin" where the key should have been found,
//   or -1 if the hash table row would overflow if that key were inserted.
int htGetBinIndex(map_t map, int row, keytype key) {
    int col;
    for (col=0; col < COL_SIZE; col++) {
        entry_t* bin = htBin(map,row, col);
        if ( htBinIsEmpty( bin ) ||
             keysEqual(bin->key, key) )
            return col;
    }
    return -1; // overflow -- key not found, no empty bins to put it in.
}    

// Return a pointer to the "bin" in the hashtable where the key is found, 
//   or a pointer to the empty "bin" where the key should have been found,
//   or NULL if the hash table row would overflow if that key were inserted.
entry_t* htGetBin(map_t map, keytype key) 
{
    int row = htIndex(key);
    int col = htGetBinIndex(map, row, key);
    if (col >= 0) 
        return htBin(map,row, col);
    else
        return NULL; // overflow -- key not found, no empty bins to put it in.
}


/*
 * Author: Greagorey Markerian
 * Edited by: Reece Whitehead
 * Needs to be reviewed and tested
 */
void mapInsert(map_t* mapref, keytype key, valuetype value)
{
    map_t map = *mapref;
    entry_t* entry = htGetBin(map, key);
    if (entry == NULL) {
        printf("Hash Map Overflow for key %s \n", key);
        assert(entry != NULL);
    }
    entry->key = keyDeepCopy(key);
    entry->value = value;
}

/*
 * Author:Reece Whitehead
 * Edit:
 * last changed: 4/5/2018
 */
map_t mapCreate()
{
    map_t map;
    map = calloc(1,sizeof(Hmap_t)); // assumes all hashtable values set to NULL
    return map;
}
    
/*
 * Author:Reece Whitehead
 * Edit:
 * last changed: 4/9/2018
 */
int mapGet(map_t map, keytype key)
{
    entry_t* entry = htGetBin(map, key);
    assert(entry != NULL);  
    return entry->value;
}

/*
 * Author: Artur Shadnik
 * Edited by: Reece Whitehead
 * Last Changed: 2018/04/05
 */
void mapRemove(map_t* mapref, keytype key)
{
    map_t map = *mapref;
    int row = htIndex(key);
    int col = htGetBinIndex(map, row, key);
    if (col == -1 || htBinIsEmpty( htBin(map, row, col) ))
        return;  // no need to remove keys that are not in the map
    
    // #126 -- potential memory leak: free key string before shifting over it?
    htShiftLeft(mapref, row, col);
}

// Shift all entries in the given row of the hash table to the left
//   starting at the given col -- effectively closes a "hole" in the bins
void htShiftLeft(map_t* mapref, int row, int col)
{
    map_t map = *mapref;
    int t;
    for(t=col; t<COL_SIZE-1; t++){
        map->hashtable[row][t] = map->hashtable[row][t+1];
    }
    htClearBin( htBin(map, row, COL_SIZE-1) ); // ensure last bin (at least) is now empt
}

/*
 * Author: Greagorey Markerian
 * Last Changed: 2018/04/05
 */
bool mapHasKey(map_t map, keytype key)
{
    entry_t* bin = htGetBin(map, key);
    return bin != NULL && !htBinIsEmpty(bin);
}

/*
 * Author: Artur Shadnik
 * Edited by: Reece Whitehead
 * Last Changed: 2018/04/05
 */
void mapClear(map_t* mapref)
{
    map_t map = *mapref;
    int r, c;
    for(r = 0; r < ARRAY_SIZE-1; r++) {
        for(c = 0; c < COL_SIZE-1; c++) {
            // #126 -- potential memory leak -- free dynamic key before clearing bins
            htClearBin( htBin(map, r, c) );
        }
    }
}


/*
 * Author: Braeden Norman
 * Last Changed: 2018/04/06
 */
int mapSize(map_t map)
{
    int r, c, size = 0;
    for(r=0; r<ARRAY_SIZE-1; r++) {
        for(c=0; !htBinIsEmpty( htBin(map,r,c) ); c++) {
            size++;
        }
    }
    return size;
}

/*
 * Author: Reece Whitehead + Greagorey+ Joseph Fall
 *
 * Last edited: 4/10/2018
 */
keytype keyDeepCopy(keytype key){
    // #126 -- potential memory leak -- no code ever frees this allocation!
    keytype newKey = calloc(strlen(key)+1, sizeof(char));
    strcpy(newKey, key);
    return newKey;
}

/*
 * Author: Reece Whitehead + Greagorey+ Joseph Fall
 *
 * Last edited: 4/10/2018
 */
bool keysEqual(keytype key1,keytype key2){
    return (strcmp(key1, key2)==0);
}

/*
 * Author: Ricky Cheung
 * Last edited: 4/12/2018
 * Small Edit by: Reece Whitehead
 * Prints a text representation of the hash table for debugging purposes
 */
void mapPrint(map_t map){
    int r, c;
    printf("===MAP: HASH TABLE===\n");
    for( r = 0; r < ARRAY_SIZE-1; r++ ){
        for( c = 0; c < COL_SIZE-1; c++ ){
            entry_t* entry = htBin(map,r,c);
            if( !htBinIsEmpty( entry ) ){
                printf("[bucket:%d; key:%s; value:%d]\n", r, entry->key, entry->value);
            }
        }
    }
    printf("=====================\n");
}

/*
 * Author: Ricky Cheung
 * Last edited: 4/11/2018
 *
 * returns a dynamic array containing all the Map Keys (in any sequence)
 * it is the caller's responsibility to free the returned array.
 */
keytype* mapKeySet(map_t map){
    int size = mapSize(map);
    int r, c;
    int arrIndex = 0;
    keytype* keys = calloc(size, sizeof(keytype));
    for( r=0; r < ARRAY_SIZE; r++ ){
        for( c=0; c < COL_SIZE; c++ ){
            entry_t* bin = htBin(map,r,c);
            if ( !htBinIsEmpty( bin ) ){
                keys[arrIndex] = bin->key;
                arrIndex++;
            }
        }
    }
    return keys;
}
