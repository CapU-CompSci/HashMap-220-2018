/*
 *
 * COMP 220 Project 4: Private API for bsTree ADT
 * Author: Cap Comp220 Spring 2019
 * Date: April 1, 2019
 *
 */
struct btNode_s {

    char* key;
	int count;
	struct btNode_s* right;
	struct btNode_s* left;
};
typedef struct btNode_s btNode;

typedef btNode* bsTree;

/**********************
 * Public Interface
 **********************/

/*
 * Creates new node with given key
 * POST: Count == 1 , btNode.key == key, btNodeRight/Left == NULL
 */
btNode* btNodecreate(char* key);

/*
 *Deletes given Node
 *POST: node_p == NULL
 */
void btNodeDestroy(btNode** node_p);



/*
 * Inserts data into the tree
 * PRE: Tree Must not contain Key
 */
void bstInsert(bsTree* tree_p, char* key);

/*
 * finds a node in the tree given the key
 * returns a pointer to node if found, else return NULL
 */
btNode* bstFindkey(btNode* root, char* key);

/*
 * create a new empty tree
 * POST: Return Empty Tree
*/
bsTree bstCreate();

/*
 * destroy the whole tree
 *
*/
void bstDestroy(bsTree* tree);


/*
 *finds if tree is empty or not
 *POST: True if tree empty , False if Tree not empty
*/


void bstDestroy(bsTree* tree);


bool bstIsEmpty(bsTree tree);


/* 
* Given the root of the tree and a node,
* the function will set the node to the node
* in the tree which has the max count.
* This is a helper function of the findMax.
*/

void findMax(btNode* root, btNode* node);


/*Finds the node with max count, 
* prints the key and the count of the node,
* then deletes the node itself.
* This is a helper function of printTree.
*/

void printMax(btNode* root);


/* Prints all the nodes of the tree in descending order of 
* their counts and deletes the tree.
*/

void printTree(btNode* root);