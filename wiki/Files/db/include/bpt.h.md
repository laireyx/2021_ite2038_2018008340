

# db/include/bpt.h



## Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[record](/Classes/record)**  |
| struct | **[node](/Classes/node)**  |

## Types

|                | Name           |
| -------------- | -------------- |
| typedef struct <a href="/Classes/record">record</a> | **[record](/Files/db/include/bpt.h#typedef-record)**  |
| typedef struct <a href="/Classes/node">node</a> | **[node](/Files/db/include/bpt.h#typedef-node)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| void | **[license_notice](/Files/db/include/bpt.h#function-license_notice)**(void ) |
| void | **[print_license](/Files/db/include/bpt.h#function-print_license)**(int licence_part) |
| void | **[usage_1](/Files/db/include/bpt.h#function-usage_1)**(void ) |
| void | **[usage_2](/Files/db/include/bpt.h#function-usage_2)**(void ) |
| void | **[usage_3](/Files/db/include/bpt.h#function-usage_3)**(void ) |
| void | **[enqueue](/Files/db/include/bpt.h#function-enqueue)**(<a href="/Classes/node">node</a> * new_node) |
| <a href="/Classes/node">node</a> * | **[dequeue](/Files/db/include/bpt.h#function-dequeue)**(void ) |
| int | **[height](/Files/db/include/bpt.h#function-height)**(<a href="/Classes/node">node</a> * root) |
| int | **[path_to_root](/Files/db/include/bpt.h#function-path_to_root)**(<a href="/Classes/node">node</a> * root, <a href="/Classes/node">node</a> * child) |
| void | **[print_leaves](/Files/db/include/bpt.h#function-print_leaves)**(<a href="/Classes/node">node</a> * root) |
| void | **[print_tree](/Files/db/include/bpt.h#function-print_tree)**(<a href="/Classes/node">node</a> * root) |
| void | **[find_and_print](/Files/db/include/bpt.h#function-find_and_print)**(<a href="/Classes/node">node</a> * root, int key, bool verbose) |
| void | **[find_and_print_range](/Files/db/include/bpt.h#function-find_and_print_range)**(<a href="/Classes/node">node</a> * root, int range1, int range2, bool verbose) |
| int | **[find_range](/Files/db/include/bpt.h#function-find_range)**(<a href="/Classes/node">node</a> * root, int key_start, int key_end, bool verbose, int returned_keys[], void * returned_pointers[]) |
| <a href="/Classes/node">node</a> * | **[find_leaf](/Files/db/include/bpt.h#function-find_leaf)**(<a href="/Classes/node">node</a> * root, int key, bool verbose) |
| <a href="/Classes/record">record</a> * | **[find](/Files/db/include/bpt.h#function-find)**(<a href="/Classes/node">node</a> * root, int key, bool verbose) |
| int | **[cut](/Files/db/include/bpt.h#function-cut)**(int length) |
| <a href="/Classes/record">record</a> * | **[make_record](/Files/db/include/bpt.h#function-make_record)**(int value) |
| <a href="/Classes/node">node</a> * | **[make_node](/Files/db/include/bpt.h#function-make_node)**(void ) |
| <a href="/Classes/node">node</a> * | **[make_leaf](/Files/db/include/bpt.h#function-make_leaf)**(void ) |
| int | **[get_left_index](/Files/db/include/bpt.h#function-get_left_index)**(<a href="/Classes/node">node</a> * parent, <a href="/Classes/node">node</a> * left) |
| <a href="/Classes/node">node</a> * | **[insert_into_leaf](/Files/db/include/bpt.h#function-insert_into_leaf)**(<a href="/Classes/node">node</a> * leaf, int key, <a href="/Classes/record">record</a> * pointer) |
| <a href="/Classes/node">node</a> * | **[insert_into_leaf_after_splitting](/Files/db/include/bpt.h#function-insert_into_leaf_after_splitting)**(<a href="/Classes/node">node</a> * root, <a href="/Classes/node">node</a> * leaf, int key, <a href="/Classes/record">record</a> * pointer) |
| <a href="/Classes/node">node</a> * | **[insert_into_node](/Files/db/include/bpt.h#function-insert_into_node)**(<a href="/Classes/node">node</a> * root, <a href="/Classes/node">node</a> * parent, int left_index, int key, <a href="/Classes/node">node</a> * right) |
| <a href="/Classes/node">node</a> * | **[insert_into_node_after_splitting](/Files/db/include/bpt.h#function-insert_into_node_after_splitting)**(<a href="/Classes/node">node</a> * root, <a href="/Classes/node">node</a> * parent, int left_index, int key, <a href="/Classes/node">node</a> * right) |
| <a href="/Classes/node">node</a> * | **[insert_into_parent](/Files/db/include/bpt.h#function-insert_into_parent)**(<a href="/Classes/node">node</a> * root, <a href="/Classes/node">node</a> * left, int key, <a href="/Classes/node">node</a> * right) |
| <a href="/Classes/node">node</a> * | **[insert_into_new_root](/Files/db/include/bpt.h#function-insert_into_new_root)**(<a href="/Classes/node">node</a> * left, int key, <a href="/Classes/node">node</a> * right) |
| <a href="/Classes/node">node</a> * | **[start_new_tree](/Files/db/include/bpt.h#function-start_new_tree)**(int key, <a href="/Classes/record">record</a> * pointer) |
| <a href="/Classes/node">node</a> * | **[insert](/Files/db/include/bpt.h#function-insert)**(<a href="/Classes/node">node</a> * root, int key, int value) |
| int | **[get_neighbor_index](/Files/db/include/bpt.h#function-get_neighbor_index)**(<a href="/Classes/node">node</a> * n) |
| <a href="/Classes/node">node</a> * | **[adjust_root](/Files/db/include/bpt.h#function-adjust_root)**(<a href="/Classes/node">node</a> * root) |
| <a href="/Classes/node">node</a> * | **[coalesce_nodes](/Files/db/include/bpt.h#function-coalesce_nodes)**(<a href="/Classes/node">node</a> * root, <a href="/Classes/node">node</a> * n, <a href="/Classes/node">node</a> * neighbor, int neighbor_index, int k_prime) |
| <a href="/Classes/node">node</a> * | **[redistribute_nodes](/Files/db/include/bpt.h#function-redistribute_nodes)**(<a href="/Classes/node">node</a> * root, <a href="/Classes/node">node</a> * n, <a href="/Classes/node">node</a> * neighbor, int neighbor_index, int k_prime_index, int k_prime) |
| <a href="/Classes/node">node</a> * | **[delete_entry](/Files/db/include/bpt.h#function-delete_entry)**(<a href="/Classes/node">node</a> * root, <a href="/Classes/node">node</a> * n, int key, void * pointer) |
| <a href="/Classes/node">node</a> * | **[db_delete](/Files/db/include/bpt.h#function-db_delete)**(<a href="/Classes/node">node</a> * root, int key) |
| void | **[destroy_tree_nodes](/Files/db/include/bpt.h#function-destroy_tree_nodes)**(<a href="/Classes/node">node</a> * root) |
| <a href="/Classes/node">node</a> * | **[destroy_tree](/Files/db/include/bpt.h#function-destroy_tree)**(<a href="/Classes/node">node</a> * root) |

## Attributes

|                | Name           |
| -------------- | -------------- |
| int | **[order](/Files/db/include/bpt.h#variable-order)**  |
| <a href="/Classes/node">node</a> * | **[queue](/Files/db/include/bpt.h#variable-queue)**  |
| bool | **[verbose_output](/Files/db/include/bpt.h#variable-verbose_output)**  |

## Defines

|                | Name           |
| -------------- | -------------- |
|  | **[DEFAULT_ORDER](/Files/db/include/bpt.h#define-default_order)**  |
|  | **[MIN_ORDER](/Files/db/include/bpt.h#define-min_order)**  |
|  | **[MAX_ORDER](/Files/db/include/bpt.h#define-max_order)**  |
|  | **[LICENSE_FILE](/Files/db/include/bpt.h#define-license_file)**  |
|  | **[LICENSE_WARRANTEE](/Files/db/include/bpt.h#define-license_warrantee)**  |
|  | **[LICENSE_WARRANTEE_START](/Files/db/include/bpt.h#define-license_warrantee_start)**  |
|  | **[LICENSE_WARRANTEE_END](/Files/db/include/bpt.h#define-license_warrantee_end)**  |
|  | **[LICENSE_CONDITIONS](/Files/db/include/bpt.h#define-license_conditions)**  |
|  | **[LICENSE_CONDITIONS_START](/Files/db/include/bpt.h#define-license_conditions_start)**  |
|  | **[LICENSE_CONDITIONS_END](/Files/db/include/bpt.h#define-license_conditions_end)**  |

## Types Documentation

### typedef record

```cpp
typedef struct record record;
```


### typedef node

```cpp
typedef struct node node;
```



## Functions Documentation

### function license_notice

```cpp
void license_notice(
    void 
)
```


### function print_license

```cpp
void print_license(
    int licence_part
)
```


### function usage_1

```cpp
void usage_1(
    void 
)
```


### function usage_2

```cpp
void usage_2(
    void 
)
```


### function usage_3

```cpp
void usage_3(
    void 
)
```


### function enqueue

```cpp
void enqueue(
    node * new_node
)
```


### function dequeue

```cpp
node * dequeue(
    void 
)
```


### function height

```cpp
int height(
    node * root
)
```


### function path_to_root

```cpp
int path_to_root(
    node * root,
    node * child
)
```


### function print_leaves

```cpp
void print_leaves(
    node * root
)
```


### function print_tree

```cpp
void print_tree(
    node * root
)
```


### function find_and_print

```cpp
void find_and_print(
    node * root,
    int key,
    bool verbose
)
```


### function find_and_print_range

```cpp
void find_and_print_range(
    node * root,
    int range1,
    int range2,
    bool verbose
)
```


### function find_range

```cpp
int find_range(
    node * root,
    int key_start,
    int key_end,
    bool verbose,
    int returned_keys[],
    void * returned_pointers[]
)
```


### function find_leaf

```cpp
node * find_leaf(
    node * root,
    int key,
    bool verbose
)
```


### function find

```cpp
record * find(
    node * root,
    int key,
    bool verbose
)
```


### function cut

```cpp
int cut(
    int length
)
```


### function make_record

```cpp
record * make_record(
    int value
)
```


### function make_node

```cpp
node * make_node(
    void 
)
```


### function make_leaf

```cpp
node * make_leaf(
    void 
)
```


### function get_left_index

```cpp
int get_left_index(
    node * parent,
    node * left
)
```


### function insert_into_leaf

```cpp
node * insert_into_leaf(
    node * leaf,
    int key,
    record * pointer
)
```


### function insert_into_leaf_after_splitting

```cpp
node * insert_into_leaf_after_splitting(
    node * root,
    node * leaf,
    int key,
    record * pointer
)
```


### function insert_into_node

```cpp
node * insert_into_node(
    node * root,
    node * parent,
    int left_index,
    int key,
    node * right
)
```


### function insert_into_node_after_splitting

```cpp
node * insert_into_node_after_splitting(
    node * root,
    node * parent,
    int left_index,
    int key,
    node * right
)
```


### function insert_into_parent

```cpp
node * insert_into_parent(
    node * root,
    node * left,
    int key,
    node * right
)
```


### function insert_into_new_root

```cpp
node * insert_into_new_root(
    node * left,
    int key,
    node * right
)
```


### function start_new_tree

```cpp
node * start_new_tree(
    int key,
    record * pointer
)
```


### function insert

```cpp
node * insert(
    node * root,
    int key,
    int value
)
```


### function get_neighbor_index

```cpp
int get_neighbor_index(
    node * n
)
```


### function adjust_root

```cpp
node * adjust_root(
    node * root
)
```


### function coalesce_nodes

```cpp
node * coalesce_nodes(
    node * root,
    node * n,
    node * neighbor,
    int neighbor_index,
    int k_prime
)
```


### function redistribute_nodes

```cpp
node * redistribute_nodes(
    node * root,
    node * n,
    node * neighbor,
    int neighbor_index,
    int k_prime_index,
    int k_prime
)
```


### function delete_entry

```cpp
node * delete_entry(
    node * root,
    node * n,
    int key,
    void * pointer
)
```


### function db_delete

```cpp
node * db_delete(
    node * root,
    int key
)
```


### function destroy_tree_nodes

```cpp
void destroy_tree_nodes(
    node * root
)
```


### function destroy_tree

```cpp
node * destroy_tree(
    node * root
)
```



## Attributes Documentation

### variable order

```cpp
int order;
```


### variable queue

```cpp
node * queue;
```


### variable verbose_output

```cpp
bool verbose_output;
```



## Macros Documentation

### define DEFAULT_ORDER

```cpp
#define DEFAULT_ORDER 4
```


### define MIN_ORDER

```cpp
#define MIN_ORDER 3
```


### define MAX_ORDER

```cpp
#define MAX_ORDER 20
```


### define LICENSE_FILE

```cpp
#define LICENSE_FILE "LICENSE.txt"
```


### define LICENSE_WARRANTEE

```cpp
#define LICENSE_WARRANTEE 0
```


### define LICENSE_WARRANTEE_START

```cpp
#define LICENSE_WARRANTEE_START 592
```


### define LICENSE_WARRANTEE_END

```cpp
#define LICENSE_WARRANTEE_END 624
```


### define LICENSE_CONDITIONS

```cpp
#define LICENSE_CONDITIONS 1
```


### define LICENSE_CONDITIONS_START

```cpp
#define LICENSE_CONDITIONS_START 70
```


### define LICENSE_CONDITIONS_END

```cpp
#define LICENSE_CONDITIONS_END 625
```


## Source code

```cpp
#ifndef __BPT_H__
#define __BPT_H__

// Uncomment the line below if you are compiling on Windows.
// #define WINDOWS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

// Default order is 4.
#define DEFAULT_ORDER 4

// Minimum order is necessarily 3.  We set the maximum
// order arbitrarily.  You may change the maximum order.
#define MIN_ORDER 3
#define MAX_ORDER 20

// Constants for printing part or all of the GPL license.
#define LICENSE_FILE "LICENSE.txt"
#define LICENSE_WARRANTEE 0
#define LICENSE_WARRANTEE_START 592
#define LICENSE_WARRANTEE_END 624
#define LICENSE_CONDITIONS 1
#define LICENSE_CONDITIONS_START 70
#define LICENSE_CONDITIONS_END 625

// TYPES.

/* Type representing the record
 * to which a given key refers.
 * In a real B+ tree system, the
 * record would hold data (in a database)
 * or a file (in an operating system)
 * or some other information.
 * Users can rewrite this part of the code
 * to change the type and content
 * of the value field.
 */
typedef struct record {
    int value;
} record;

/* Type representing a node in the B+ tree.
 * This type is general enough to serve for both
 * the leaf and the internal node.
 * The heart of the node is the array
 * of keys and the array of corresponding
 * pointers.  The relation between keys
 * and pointers differs between leaves and
 * internal nodes.  In a leaf, the index
 * of each key equals the index of its corresponding
 * pointer, with a maximum of order - 1 key-pointer
 * pairs.  The last pointer points to the
 * leaf to the right (or NULL in the case
 * of the rightmost leaf).
 * In an internal node, the first pointer
 * refers to lower nodes with keys less than
 * the smallest key in the keys array.  Then,
 * with indices i starting at 0, the pointer
 * at i + 1 points to the subtree with keys
 * greater than or equal to the key in this
 * node at index i.
 * The num_keys field is used to keep
 * track of the number of valid keys.
 * In an internal node, the number of valid
 * pointers is always num_keys + 1.
 * In a leaf, the number of valid pointers
 * to data is always num_keys.  The
 * last leaf pointer points to the next leaf.
 */
typedef struct node {
    void** pointers;
    int* keys;
    struct node* parent;
    bool is_leaf;
    int num_keys;
    struct node* next;  // Used for queue.
} node;

// GLOBALS.

/* The order determines the maximum and minimum
 * number of entries (keys and pointers) in any
 * node.  Every node has at most order - 1 keys and
 * at least (roughly speaking) half that number.
 * Every leaf has as many pointers to data as keys,
 * and every internal node has one more pointer
 * to a subtree than the number of keys.
 * This global variable is initialized to the
 * default value.
 */
extern int order;

/* The queue is used to print the tree in
 * level order, starting from the root
 * printing each entire rank on a separate
 * line, finishing with the leaves.
 */
extern node* queue;

/* The user can toggle on and off the "verbose"
 * property, which causes the pointer addresses
 * to be printed out in hexadecimal notation
 * next to their corresponding keys.
 */
extern bool verbose_output;

// FUNCTION PROTOTYPES.

// Output and utility.

void license_notice(void);
void print_license(int licence_part);
void usage_1(void);
void usage_2(void);
void usage_3(void);
void enqueue(node* new_node);
node* dequeue(void);
int height(node* root);
int path_to_root(node* root, node* child);
void print_leaves(node* root);
void print_tree(node* root);
void find_and_print(node* root, int key, bool verbose);
void find_and_print_range(node* root, int range1, int range2, bool verbose);
int find_range(node* root, int key_start, int key_end, bool verbose,
               int returned_keys[], void* returned_pointers[]);
node* find_leaf(node* root, int key, bool verbose);
record* find(node* root, int key, bool verbose);
int cut(int length);

// Insertion.

record* make_record(int value);
node* make_node(void);
node* make_leaf(void);
int get_left_index(node* parent, node* left);
node* insert_into_leaf(node* leaf, int key, record* pointer);
node* insert_into_leaf_after_splitting(node* root, node* leaf, int key,
                                       record* pointer);
node* insert_into_node(node* root, node* parent, int left_index, int key,
                       node* right);
node* insert_into_node_after_splitting(node* root, node* parent, int left_index,
                                       int key, node* right);
node* insert_into_parent(node* root, node* left, int key, node* right);
node* insert_into_new_root(node* left, int key, node* right);
node* start_new_tree(int key, record* pointer);
node* insert(node* root, int key, int value);

// Deletion.

int get_neighbor_index(node* n);
node* adjust_root(node* root);
node* coalesce_nodes(node* root, node* n, node* neighbor, int neighbor_index,
                     int k_prime);
node* redistribute_nodes(node* root, node* n, node* neighbor,
                         int neighbor_index, int k_prime_index, int k_prime);
node* delete_entry(node* root, node* n, int key, void* pointer);
node* db_delete(node* root, int key);

void destroy_tree_nodes(node* root);
node* destroy_tree(node* root);

#endif /* __BPT_H__*/
```


-------------------------------

Updated on 2021-10-01 at 18:38:11 +0900
