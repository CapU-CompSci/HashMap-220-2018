/*
<<<<<<< HEAD
 *
 * COMP 220 Project 4: Private API for bsTree ADT
 * Author: Cap Comp220 Spring 2019
 * Date: April 1, 2019
 *
 *
=======
 * Binary Search Tree Interface
 * Comp220 project 4
 */
struct btNode_s {

    char* key;
	int count;
	struct btNode_s* right;
	struct btNode_s* left;
};
typedef struct btNode_s btNode;

struct bsTree_s {
    btNode* root;
};
typedef struct bsTree_s bsTree;
>>>>>>> c401058513038f5d9630aa1e90997b69288d164e
