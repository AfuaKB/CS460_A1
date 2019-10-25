/*
 * 
 * You will need to write your B+Tree almost entirely from scratch. 
 * 
 * B+Trees are dynamically balanced tree structures that provides efficient support for insertion, deletion, equality, and range searches. 
 * The internal nodes of the tree direct the search and the leaf nodes hold the base data..
 * 
 * For a basic rundown on B+Trees, we will refer to parts of Chapter 10 of the textbook Ramikrishnan-Gehrke 
 * (all chapters and page numbers in this assignment prompt refer to the 3rd edition of the textbook).
 *
 * Read Chapter 10 which is on Tree Indexing in general. In particular, focus on Chapter 10.3 on B+Tree.
 */


#include "data_types.h"
#include "query.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <iostream>

#ifdef _WIN32_WINNT
#define bool char
#define false 0
#define true 1
#endif
#define DEFAULT_ORDER 4
#ifndef BTREE_H
#define BTREE_H

int order = DEFAULT_ORDER;



/* 
Designing your C Structs for B+Tree nodes (Chapter 10.3.1)
How will you represent a B+Tree as a C Struct (or series of C structs that work together)? There are many valid ways to do this part of your design, and we leave it open to you to try and tune this as you progress through the project.
How will you account for a B+Tree node being an internal node or a leaf node? Will you have a single node type that can conditionally be either of the two types, or will you have two distinct struct types?
How many children does each internal node have? This is called the fanout of the B+Tree.
What is the maximum size for a leaf node? How about for an internal node?
What is the minimum threshold of content for a node, before it has to be part of a rebalancing?
*/

// TODO: here you will need to define a B+Tree node(s) struct(s)
/** Constructing B+ tree with a C Struct**/

typedef struct NodeBee{
    int * keys;
    void **pointers;
    struct NodeBee *parent;
    bool isALeaf;
    int numOfKeys;
}NodeBee;

/**Instantiating the root node*/
NodeBee *rootNode = NULL;

/**Function that makes a node*/
/*Instantiating internal node*/
NodeBee * makeNodeBee(void){
    NodeBee * beeOne;
    beeOne = malloc(sizeof(NodeBee));
    beeOne->pointers = malloc(order * sizeof(int));
    beeOne->keys = malloc(order - 1);
    beeOne->parent = NULL;
    beeOne->numOfKeys = 0;
    beeOne->isALeaf = false;
    return beeOne;
};

/*Instantiating leaf node*/
NodeBee *makeLeafNodeBee(void){
    NodeBee * leafNodeBee = makeNodeBee();
    leafNodeBee->isALeaf = true;
    return leafNodeBee;
}

/* The following are methods that can be invoked on B+Tree node(s).
 * Hint: You may want to review different design patterns for passing structs into C functions.
 */

/* FIND (Chapter 10.4)
This is an equality search for an entry whose key matches the target key exactly.
How many nodes need to be accessed during an equality search for a key, within the B+Tree? 
*/

// TODO: here you will need to define FIND/SEARCH related method(s) of finding key-values in your B+Tree.
/** The Find Algorithm*/
NodeBee findMyNode( int key){
    NodeBee * n = rootNode; //this is the root node of the find not the entire tree being assigned to n
                //iterator that allows you to create a duplicate of the tree to traverse
    int m;
    while (!n->isALeaf){
        m = 0;
        if (key >= n->keys[n->numOfKeys-1])
            n = n->pointers[n->numOfKeys];
        else{
            for (int i = 0; i < n->numOfKeys; i++) {
                if(key == n->keys[m])
                    n = n->pointers[m+1];
                else if (key < n->keys[m])
                    n = n->pointers[m];
                
                    //break;
            }
            //n = n->pointers[m];
        }

    }
    return *n;
}

/* INSERT (Chapter 10.5)
How does inserting an entry into the tree differ from finding an entry in the tree?
When you insert a key-value pair into the tree, what happens if there is no space in the leaf node? What is the overflow handling algorithm?
For Splitting B+Tree Nodes (Chapter 10.8.3)
*/

// TODO: here you will need to define INSERT related method(s) of adding key-values in your B+Tree.
/**Creating a struct for the data*/
typedef struct recordBee{
    int value;
}recordBee;

/**Instantiating the record*/
recordBee *recordBee1 = NULL;

//NodeBee *insertInParent(NodeBee *pBee, NodeBee *pBee1, int sub, NodeBee *pBee2);

/**New Record Creation*/
recordBee *makeARecord(int value){
    //recordBee newRecord = NULL;
    recordBee * recordBee2 = (struct recordBee *)malloc(sizeof(recordBee));
    if (recordBee2 == NULL) {
        exit(EXIT_FAILURE);
    }
    else
        recordBee2->value = value;
    return recordBee2;
}

/**Helper function used in finding record Your Key refers to in the Node*/
//recordBee *finder(NodeBee *rootNode, int key, )


/**The helper function used in insertInParent for finding the index of the parent's
 * pointer to the node to the left of the key to be inserted
 */
 int getLIndex(NodeBee *parent, NodeBee *left){
     int lIndex = 0;
     while(lIndex <= parent->numOfKeys && parent->pointers[lIndex] != left)
         lIndex++;
    return lIndex;
 }

/**Inserting a new pointer to a record with its corresponding key in a leaf
 * to return changed leaf*/
NodeBee *insertInLeaf (NodeBee *leafNodeBee1, int key, recordBee *recPointer){
    int i, k;
    int pointOfInsertion;

    pointOfInsertion = 0;
    while (pointOfInsertion < leafNodeBee1->numOfKeys && leafNodeBee1 ->keys[pointOfInsertion
    <key])
        pointOfInsertion++;

    for (i = leafNodeBee1->numOfKeys; i > pointOfInsertion; i--)
        leafNodeBee1 -> keys[i] = leafNodeBee1 -> keys [i-1];
        leafNodeBee1->pointers[i] = leafNodeBee1->pointers[i-1];

    leafNodeBee1->keys[pointOfInsertion] = key;
    leafNodeBee1->pointers[pointOfInsertion] = recPointer;
    leafNodeBee1->numOfKeys++;
    return leafNodeBee1;
}

/**Location for splitting node*/
int location(int len){
    if (len%2 == 0)
        return len/2;
    else
        return len/2 + 1;
}

/**Inserting into new root by creating a new root for two subtrees and inserting
 * target key into new root
 */
NodeBee *insertInNewRoot(NodeBee *leftNodeBee, NodeBee *rightNodeBee, int key){
    NodeBee *newRoot = makeNodeBee();
    newRoot->keys[0] = key;
    newRoot->pointers[0] = leftNodeBee;
    newRoot->pointers[1] = rightNodeBee;
    newRoot->numOfKeys++;
    newRoot->parent = NULL;
    leftNodeBee->parent = newRoot;
    rightNodeBee->parent = newRoot;
    return newRoot;
}

/**Inserting a new key and pointer into a node*/
NodeBee *insertInNode(NodeBee *rootNodeBee, NodeBee *newNode, int lIndex, int key, NodeBee *rightNodeBee){
    for (int i = newNode->numOfKeys; i > lIndex ; i--) {
        newNode->pointers[i+1] = newNode->pointers[i];
        newNode->keys[i] = newNode->keys[i-1];
    }
    newNode->pointers[lIndex+1] = rightNodeBee;
    newNode->keys[lIndex] = key;
    newNode->numOfKeys++;
    return rootNodeBee;
}

/**Inserting into node with new key and pointer causing splitting of the node
 * when node exceeds its order
 */
 NodeBee *insertInNodeAfterSplitting(NodeBee *rootNodeBee, NodeBee *OGnode, NodeBee *rightNodeBee, int lIndex, int key){
     int split, sub;
     int *temKeys;
     NodeBee *newNode, *childNode;
     NodeBee **temKeyPointers;

     //temporary set of keys and pointers including new key and pointer
     //in their correct positions

     temKeyPointers = malloc((order+1) * sizeof(NodeBee *));
     if (temKeyPointers == NULL)
         exit(EXIT_FAILURE);
     temKeys = malloc(order * sizeof(int));
     if (temKeys == NULL)
         exit(EXIT_FAILURE);

    for (int i = 0, j = 0; i < OGnode->numOfKeys+1; i++, j++) {
        if (j == lIndex+1)
            j++;
        temKeyPointers[j] = OGnode->pointers[i];
    }

    for (int i = 0, j = 0; i < OGnode->numOfKeys; i++, j++) {
        if (j == lIndex)
            j++;
        temKeys[j] = OGnode->keys[i];
    }

    temKeyPointers[lIndex+1] = rightNodeBee;
    temKeys[lIndex] = key;

    //Create new node
    //Copy half of the keys and pointers to the new node
    split = location(order);
    newNode = makeNodeBee();
    OGnode->numOfKeys = 0;
    for (int i = 0; i < split-1; i++) {
        OGnode->pointers[i] = temKeyPointers[i];
        OGnode->keys[i] = temKeys[i];
        OGnode->numOfKeys++;
    }
    int i,j;
    OGnode->pointers[i] = temKeyPointers[i];
    sub = temKeys[split-1];
    for (++i, j=0; i < order; i++, j++) { //!!!!This ++i code
        newNode->pointers[j] = temKeyPointers[i];
        newNode->keys[j] = temKeys[i];
        newNode->numOfKeys++;
    }

    newNode->pointers[j] = temKeyPointers[i];
    free(temKeyPointers);
    free(temKeys);
    newNode->parent = OGnode->parent;
    for (i=0; i <= newNode->numOfKeys; i++) {
        childNode = newNode->pointers[i];
        childNode->parent = newNode;
    }

    return insertInParent(rootNodeBee, OGnode, newNode, sub); //!!!ERROR! IMPLICIT DECLARATION
 }

/**Inserting a new node into the tree to return the root after insertion*/
NodeBee *insertInParent(NodeBee *rootNodeBee2, NodeBee *leftNodeBee1, NodeBee *rightNodeBee, int key){
    int lIndex;
    NodeBee *parent;
    parent = leftNodeBee1->parent;

    /*If a new root is required*/
    if (parent == NULL)
        return insertInNewRoot(leftNodeBee1, rightNodeBee, key);

    /*Leaf or Node (Remainder of function body)*/
    lIndex = getLIndex(parent,leftNodeBee1); //Parent ptr to left node

    if (parent->numOfKeys < order-1)
        return insertInNode(rootNodeBee2, parent,lIndex,key,rightNodeBee);

    return insertInNodeAfterSplitting(rootNodeBee2,parent,rightNodeBee,lIndex,key);

}

/**Inserting a new key and pointer to a new record into a leaf to prevent exceeding
 * the tree's order by splitting in two
 */
NodeBee *leafInsertAfterSplit(NodeBee *rootNodeBee1, NodeBee *leafNodeBee2, int key, recordBee *recPointer1){
    NodeBee *newLeaf;
    int *temKeys;
    int **temKeyPointers;
    int i, j, newKey, indexOfInsertion, split;

    newLeaf = makeLeafNodeBee();

    temKeys = malloc(order * sizeof(int));
    if (temKeys == NULL){
        exit(EXIT_FAILURE);
    }

    indexOfInsertion = 0;
    while (indexOfInsertion < order-1 && leafNodeBee2->keys[indexOfInsertion] < key){
        indexOfInsertion++;
    }

    for (i = 0, j =0; i < leafNodeBee2->numOfKeys; i++, j++) {//ERROR!
        if (j == indexOfInsertion)
            j++;
        temKeys[j] = leafNodeBee2->keys[i];
        temKeyPointers[j] = leafNodeBee2->pointers[i];
    }

    temKeys[indexOfInsertion] = key;
    temKeyPointers[indexOfInsertion] = recPointer1;

    leafNodeBee2->numOfKeys = 0;

    split = location(order-1);

    for (i = 0; i < split; ++i) {
        leafNodeBee2->pointers[i] = temKeyPointers[i];
        leafNodeBee2->keys[i] = temKeys;
        leafNodeBee2->numOfKeys++;
    }

    for (i = split, j = 0; i < order; i++, j++) {
        newLeaf->pointers[j] = temKeyPointers[i];
        newLeaf->keys[i] = temKeys[i];
        newLeaf->numOfKeys++;
    }

    free(temKeyPointers);
    free(temKeys);

    newLeaf->pointers[order-1] = leafNodeBee2->pointers[order-1];
    leafNodeBee2->pointers[order-1] = newLeaf;

    for (i = leafNodeBee2->numOfKeys; i < order-1; ++i) {
        leafNodeBee2->pointers[i] = NULL;
    }

    for (i = newLeaf->numOfKeys; i < order-1; ++i) {
        newLeaf->pointers[i] = NULL;
    }

    newLeaf->parent = leafNodeBee2->parent;
    newKey = newLeaf->keys[0];

    return insertInParent(rootNodeBee1,leafNodeBee2, newLeaf, newKey);

}

/**
 * Starting a New Tree
 */

NodeBee *newTree(void *recPointer, int key){
    NodeBee *rootNode = makeLeafNodeBee();
    rootNode->keys[0] = key;
    rootNode->pointers[0] = recPointer;
    rootNode->pointers[order-1] = NULL;
    rootNode->parent = NULL;
    rootNode->numOfKeys++;
    return rootNode;
}


/* BULK LOAD (Chapter 10.8.2)
Bulk Load is a special operation to build a B+Tree from scratch, from the bottom up, when beginning with an already known dataset.
Why might you use Bulk Load instead of a series of inserts for populating a B+Tree? Compare the cost of a Bulk Load of N data entries versus that of an insertion of N data entries? What are the tradeoffs?
*/

// TODO: here you will need to define BULK LOAD related method(s) of initially adding all at once some key-values to your B+Tree.
// BULK LOAD only can happen at the start of a workload


/*RANGE (GRADUATE CREDIT)
Scans are range searches for entries whose keys fall between a low key and high key.
Consider how many nodes need to be accessed during a range search for keys, within the B+Tree?
Can you describe two different methods to return the qualifying keys for a range search? 
(Hint: how does the algorithm of a range search compare to an equality search? What are their similarities, what is different?)
Can you describe a generic cost expression for Scan, measured in number of random accesses, with respect to the depth of the tree?
*/

// TODO GRADUATE: here you will need to define RANGE for finding qualifying keys and values that fall in a key range.



#endif