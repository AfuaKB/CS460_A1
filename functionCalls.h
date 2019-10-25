
/**
 * These functions are of the btree file
 */
 
 NodeBee * makeNodeBee(void);

 NodeBee *makeLeafNodeBee(void);

 recordBee *makeARecord(int value);

 NodeBee findMyNode( int key);

 recordBee *finder(NodeBee *rootNode, int key, NodeBee **lLeaf);

 int getLIndex(NodeBee *parent, NodeBee *left);

 NodeBee *insertInLeaf (NodeBee *leafNodeBee1, int key, recordBee *recPointer);

 int location(int len);

 NodeBee *insertInNewRoot(NodeBee *leftNodeBee, NodeBee *rightNodeBee, int key);

 NodeBee *insertInNode(NodeBee *rootNodeBee, NodeBee *newNode, int lIndex, int key, NodeBee *rightNodeBee);

 NodeBee *insertInNodeAfterSplitting(NodeBee *rootNodeBee, NodeBee *OGnode, NodeBee *rightNodeBee, int lIndex, int key);

 NodeBee *insertInParent(NodeBee *rootNodeBee2, NodeBee *leftNodeBee1, NodeBee *rightNodeBee, int key);

 NodeBee *leafInsertAfterSplit(NodeBee *rootNodeBee1, NodeBee *leafNodeBee2, int key, recordBee *recPointer1);

 NodeBee *newTree(void *recPointer, int key);

 NodeBee *insertM(NodeBee *rootNode, int key, int data);