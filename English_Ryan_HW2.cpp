/**
 * A sample code of Creating C++ linked lists,
 * Including definitions the list node class and linked list class,
 * and how to create a blank linked list and a one-node linked list.
 *
 * Outline: understand the definition and structure of the linked
 * list and build a linked list based on it.
 */

#include <iostream>
#include <cstddef>

using std::cout;
using std::endl;

/* definition of the list node class */
class Node
{
    friend class LinkedList;
private:
    /* The value of this node */
    int value; 
    /* The next node in the list */
    Node *pNext; 
    /* The previous node */
    Node *pPrevious; 
    
public:
    /* Constructors with No Arguments */
    Node(void)
    : pNext(NULL), pPrevious(NULL)
    { }
    
    /* Constructors with a given value */
    Node(int val)
    : value(val), pNext(NULL), pPrevious(NULL)
    { }
    
    /* Constructors with a given value and a link of the next node */
    Node(int val, Node* next)
    : value(val), pNext(next), pPrevious(NULL)
    {}

    /* Constructors with a given value and a link of the next node and the previous */
    Node(int val, Node* next, Node* previous)
    : value(val), pNext(next), pPrevious(previous)
    {}
    
    /* Getters */
    int getValue(void)
    { return value; }
    
    /* Gets the next node in the list */
    Node* getNext(void)
    { return pNext; }

    /* Gets the previous node in the list */
    Node* getPrevious(void) 
    { return pPrevious; }
};

/* definition of the linked list class */
class LinkedList
{
private:
    /* pointer of head node */
    Node *pHead;
    /* pointer of tail node */
    Node *pTail;
    
public:
    /* Constructors with No Arguments */
    LinkedList(void);
    /* Constructors with a given value of a list node */
    LinkedList(int val);
    /* Destructor */
    ~LinkedList(void);
    
    /* Traversing the list and printing the value of each node */
    void traverse_and_print(bool reverse);

    /* Adds a node to the end of the list */
	void push_back(int val);
};

LinkedList::LinkedList()
{
    /* Initialize the head and tail node */
    pHead = pTail = NULL;
}

LinkedList::LinkedList(int val)
{
    /* Create a new node, acting as both the head and tail node */
    pHead = new Node(val);
    pTail = pHead;
}

LinkedList::~LinkedList()
{
    
}

void LinkedList::traverse_and_print(bool reverse = false)
{
    Node *p;

    /* If we are going in reverse we need to start at the tail */
    if (reverse) p = pTail;
    /* Otherwise start at the head */
    else p = pHead;
    
    /* The list is empty? */
    if (p == NULL) {
        cout << "The list is empty" << endl;
        return;
    }
    
    cout << "LinkedList: ";
    /* A basic way of traversing a linked list */
    while (p != NULL) { /* while there are some more nodes left */
        /* output the value */
        cout << p->value << " ";
        /* The pointer moves along to the next one */
        if (reverse) p = p->pPrevious;
        else p = p->pNext;
    }
    cout << endl;
}

/* 
*  This method pushes a new value onto the tail end
*  of the list
*/
void LinkedList::push_back(int val){
    /* Create the new node and set to the tails next node */
    pTail->pNext = new Node(val, NULL, pTail);
    /* Update the tail to be the new node, or current tails next node */
    pTail = pTail->pNext;
}

int main()
{
    /* Create an empty list */
    LinkedList list1;
    cout << "Created an empty list named list1." << endl;
    list1.traverse_and_print();
    
    /* Create a list with only one node */
    LinkedList list2(10);
    cout << "Created a list named list2 with only one node." << endl;
    list2.traverse_and_print();
	
    /*
    * Push the countdown from 10 on to the end of the
    * linked list then out put the countdown from head to tail
    */
    cout << "Utilizing list2 push a countdown from 10 to 0 onto the end of the list." << endl;
	for (int i = 9 ; i >= 0; i--){
		list2.push_back(i);
	}
	list2.traverse_and_print();

    /*
    * Utilizing the same list we will traverse the list
    * in reverse order in order to show the double linked list
    * implementation
    */
    cout << "Now reverse print list2." << endl;
	list2.traverse_and_print(true);

    return 0;
}


