//
// Created by Afua Kesewa Boateng on 24/10/2019.
//

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

#ifndef BTREE_H
#define BTREE_H

#include "data_types.h"
#include "query.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32_WINNT
#define bool char
#define false 0
#define true 1
#endif
#define DEFAULT_ORDER 4

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

/**New Record Creation*/
struct recordBee *makeARecord(int value){
    //recordBee newRecord = NULL;
    recordBee * recordBee2 = (struct recordBee *)malloc(sizeof(recordBee));
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