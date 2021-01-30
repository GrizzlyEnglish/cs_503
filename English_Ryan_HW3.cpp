#include <iostream>
#include <cstddef>

using std::cout;
using std::endl;

class Node {
    int value;
public:
    Node* left;       // left child
    Node* right;      // right child
    Node* p;          // parent
    Node(int data) {
        value = data;
        left = NULL;
        right = NULL;
        p  = NULL;
    }
    ~Node() {
    }
    int d() {
        return value;
    }
    void set_d(int d) {
        value = d;
    }
    void print() {
        std::cout << value << std::endl;
    }
};

/*
* Finds the minimum value of the current tree root
*/
Node* find_minimum(Node* tree_root) {
    // If the root is null return
    if (tree_root == NULL) {
        return NULL;
    // If the left is null we found it return the current node
    } else if (tree_root->left == NULL) {
        return tree_root;
    }
    // Otherwise keep going down
    return find_minimum(tree_root->left);
}

/*
* Inserts a node in into the tree of the root
* Using parent_node and pointer to allow set of current location of node
* If parent node is null, it is the parent node
*/
void insert(Node *insert_node, Node * & parent_node) {
    // First see if we have a root node
    if (parent_node == NULL) {
        // We don't make the node we are inserting the root
        parent_node = insert_node;
    // Next we check if the insert is less than the root
    } else if (insert_node->d() < parent_node->d()) {
        // It is less than insert into left node
        insert(insert_node, parent_node->left);
    // Last we check if the insert is greater than the root
    } else {
        insert(insert_node, parent_node->right);
    }
}

/*
* Searches for a node in the tree
*/
Node* search(int value, Node *tree_root) {
    if (tree_root == NULL) {
        return NULL;
    }
    else if (tree_root->d() > value) {
        return search(value, tree_root->left);
    } else if (tree_root->d() < value) {
        return search(value, tree_root->right);
    } else {
        return tree_root;
    }
}

/*
* Removes a value from the tree given the parent node
*/
void delete_node(int value, Node * & tree_root) {
    // Lack a node just return null
    if (tree_root == NULL) {
        return;
    }

    // If the value is lower head down left
    if (tree_root->d() > value) {
        delete_node(value, tree_root->left);
    // If the value is greater head down right
    } else if (tree_root->d() < value) {
        delete_node(value, tree_root->right);
    } 
    // Value is equal, if we have children we have to do some logic
    else if (tree_root->left != NULL && tree_root->right != NULL) {
        // Find the minimum from the right tree
        Node* min = find_minimum(tree_root->right);
        // Set the current root to that value
        tree_root->set_d(min->d());
        // Remove the min
        delete_node(tree_root->d(), tree_root->right);
    } else {
        // Temp
        Node* oldNode = tree_root;
        // Reset the left if there otherwise the right
        tree_root = tree_root->left != NULL ? tree_root->left : tree_root->right;
        // Delete the node we came to delete
        delete oldNode;
    }
}

/*
* Print in preorder to verify the tree
*/
void preorder_print(Node *tree_root) {
    // Go all the way left first
    if (tree_root->left != NULL) {
        preorder_print(tree_root->left);
    }

    cout << tree_root->d();
    cout << " ";

    // Then see if we need to go right
    if (tree_root->right != NULL) {
        preorder_print(tree_root->right);
    }
}

/*
* Homework 3 entry point
* Ryan English
*/
int main()
{
    // Problem one create the tree
    cout << "Problem One" << endl;
    
    // Init the array of values we need to insert
    int array[] = {2, 1, 4, 5, 9, 3, 6, 7, 10, 12, 11};

    // Loop and insert values
    Node* tree_root = NULL;
    for(int i = 0; i < 11; i++) {
        Node* node = new Node(array[i]);
        insert(node, tree_root);
    }

    // Print out tree in preorder to verify
    cout << "Preorder Print" << endl;
    preorder_print(tree_root);
    cout << endl;

    // Problem two delete 4 then 9
    cout << "Problem Two" << endl;

    delete_node(4, tree_root);
    cout << "Deleted node with value 4, new preorder:" << endl;
    preorder_print(tree_root);
    cout << endl;

    delete_node(9, tree_root);
    cout << "Deleted node with value 9, new preorder:" << endl;
    preorder_print(tree_root);
    cout << endl;

    // Problem three search for 12 and 4
    cout << "Problem Three" << endl;

    Node* tweleve = search(12, tree_root);
    if (tweleve == NULL) {
        cout << "Twelve not found" << endl;
    } else {
        cout << tweleve->d();
        cout << " Found" << endl;
    }

    Node* four = search(4, tree_root);
    if (four == NULL) {
        cout << "Four not found" << endl;
    } else {
        cout << four->d();
        cout << " Found" << endl;
    }
}
