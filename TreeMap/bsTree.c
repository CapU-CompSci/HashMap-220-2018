







#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "bsTree.h"







































btNode* bstFindkey(btNode* root, char* key) {
    /*
    *   If strcmp(str1,str2) returns a negative number, it means the 
    *   first string's ASCII is smaller and also is bigger if we consider
    *   a to be the biggest and z be the smallest letter, ex: first string
    *   is aaa and the second string is zzz (ASCII of a is less than z)
    *   So if the we are looking for aaa and the node's string is ccc, we go 
    *   to the right node.
    */
    
    if ( strcmp(root -> key, key) == 0 ){
        return root;
    }
    
    if (strcmp(root -> key, key) < 0) {
        return bstFindkey(root -> left, key);
    }
    
    if (strcmp(root -> key, key) > 0) {
        return bstFindkey(root -> right, key);
    }
    
    else //Key does not exist
        return NULL;
}