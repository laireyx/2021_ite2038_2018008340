0. Introduction
The original In-Memory B+ Tree source code starts any operations from the leaf node.
To find the appropriate leaf node, it uses `find_by_key()` and `find_leaf()` function.

1. Insertion
![insert](uploads/0c6fdfa45f4439e155d70e340aa246bf/insert.png)
First, find a record corresponding to the key to insert from the tree, and terminates if found. This implementation does not allow key duplication.
Second, find an appropriate leaf node that can contain a record key to insert. If its free space is enough, then append the record to that leaf node and terminates. If not, it starts splitting.

Splitting a node is quite simple. Just create a new node and divide existing records with the same numbers each. Then re-align the sibling node number and add a child node branch into the parent of the original node.
If the parent node has enough space for the new node branch, then it terminates. Otherwise, it also starts splitting in the internal node.

When the splitting is finished, then check if the root node has split. If it is, create a new root node that can cover those two nodes.

Then the insertion is finished.

2. Deletion
![delete](uploads/68c2c84226af22f0af0a67a777c52cb0/delete.png)
First, find a record corresponding to the key to insert from the tree, and terminates if not found. Deletion is valid for only existing items.
Second, just remove the record from that leaf node and check the free space amount. If it has too much free space so that it causes unbalance, then it starts merging or redistributing. If the node has enough space for all the sibling node's records, it chooses to merge two nodes or redistribute otherwise.

Merging two nodes is simple. Just move all the nodes from one of the nodes to the other node. Then remove a child node branch from the parent node, and check if that parent node needs to be merged or redistributed.

Redistributing two nodes is somewhat complex. One node pushes up a child node branch from its leftmost(or rightmost) record to its parent node, and the parent node pushes down the child node branch to the other child node. Then the child node adds that node branch as a record. Redistributing does not cause any deletion of the records, so the deletion is terminated here.

After the deletion is finished, a check for the root node is required. If the root node is the internal node and does not have any child node branch, then it pulls up the leftmost child and sets the node as the new root node.

3. Design change
In this in-memory B+ tree, it assumes that all the leaf node has same record size and the leaf node has the same architecture with the internal node. But in our disk-based B+ tree, every data record has its value size, and the internal page is different from the leaf page for efficiency. It causes a major design change.
While deleting a record or page branch from the page, then the disk-based B+ tree will use its threshold, which is 124 branches for the internal page and 2500 bytes for the leaf page. The other changes are quite minor, so I think it is unnecessary to specify them.