// AVLSet.hpp
//
// ICS 46 Spring 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    ~AVLSet() noexcept override;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const noexcept;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    struct Node
    {
        ElementType key;
        Node* right;
        Node* left;
    };

    int treeHeight;
    int treeSize;

    Node* root;
    Node* current;
    bool isbalanced;

    //Recursion version for the copy constructor
    void copyConstruct(const Node*, Node*&);
    //Recursion version for the destructor
    void destruct(Node*);
    //Recursion version for function contains
    bool exists(const ElementType&, Node*) const;
    //Recursion version for function add if there is no balance needed
    void simpleAdd(const ElementType&, Node*&);
    //Recursion version for function add if balance is needed
    void balance(const ElementType&, Node*&);
    //Recursion version for function height
    int getHeight(Node*) const;
    //Recursion version for function preorder
    void preorderCall(VisitFunction, Node*) const;
    //Recursion version for function inorder
    void inorderCall(VisitFunction, Node*) const;
    //Recursion version for function postorder
    void postorderCall(VisitFunction, Node*) const;

    //Helper functions to rotate the tree
    void leftRotate(Node*&);
    void rightRotate(Node*&);
    void rotateTwice(Node*&, std::string);
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
: treeSize{0}, root{nullptr}, isbalanced{shouldBalance}
{
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    destruct(root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    treeHeight = s.treeHeight;
    treeSize = s.treeSize;
    copyConstruct(s.root, root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    treeHeight = s.treeHeight;
    treeSize = s.treeSize;
    copyConstruct(s.root, root);
    s.~AVLSet();
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if(this != &s)
    {
        this->~AVLSet();
        treeHeight = s.treeHeight;
        treeSize = s.treeSize;
        copyConstruct(s.root, root);    
    }
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    if(this != &s)
    {
        this->~AVLSet();
        treeHeight = s.treeHeight;
        treeSize = s.treeSize;
        copyConstruct(s.root, root);  
        s.~AVLSet();  
    }
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    if (contains(element))
        return;
    if (isbalanced == false)
        simpleAdd(element, root);  
    else
        balance(element, root);
    treeSize ++;   
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{   
    return exists(element, root);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return treeSize;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const noexcept
{
    return getHeight(root);
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorderCall(visit, root);

}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorderCall(visit, root);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorderCall(visit, root);
}


template<typename ElementType>
void AVLSet<ElementType>::copyConstruct(const Node* source, Node*& current)
{
    if(source == nullptr)
    {
        current = nullptr;
    }
    else
    {
        current = new Node{source->key};
        if (!source->left && !source->right)
        {
            return;
        }
        copyConstruct(source->left, current->left);
        copyConstruct(source->right, current->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::destruct(Node* n)
{
    if(n == nullptr)
        return;
    destruct(n->left);
    destruct(n->right);
    delete n;
}

template <typename ElementType>
bool AVLSet<ElementType>::exists(const ElementType& element, Node* node) const
{
    if (!node)
    {
        return false;
    }
    else if (node->key == element)
    {
        return true;
    }
    else if (node->key > element)
    {
        return exists(element, node->left);
    }
    else
    {
        return exists(element, node->right);
    }
}


template <typename ElementType>
void AVLSet<ElementType>::simpleAdd(const ElementType& element, Node*& node)
{
    if (!node)
    {
        node = new Node{element};
    }

    else if (element < node->key)
    {
        simpleAdd(element, node->left);
    }
    else
    {
        simpleAdd(element, node->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::balance(const ElementType& element, Node*& n)
{
    if(!n)
    {
        n = new Node{element};
    }
    else if(element > n->key)
    {
        balance(element, n->right);
        if (getHeight(n->right)-getHeight(n->left) >= 2)
        {
            if (element > n->right->key)
            {
                rightRotate(n);
            }
            else
            {
                std::string direction = "left";
                rotateTwice(n, direction);
            }
        }
    }
    else
    {
        balance(element, n->left);
        if (getHeight(n->left)-getHeight(n->right) >= 2)
        {
            if(element < n->left->key)
            {
                leftRotate(n);
            }
            else
            {
                std::string direction = "right";
                rotateTwice(n, direction);
            }
        }
    }
}


template <typename ElementType>
int AVLSet<ElementType>::getHeight(Node* n) const
{
    if (!n)
    {
        return -1;
    }
    int leftHeight = getHeight(n->left);
    int rightHeight = getHeight(n->right);
    if (leftHeight > rightHeight)
    {
        return leftHeight + 1;
    }
    else
    {
        return rightHeight + 1;
    }
}


template <typename ElementType>
void AVLSet<ElementType>::preorderCall(VisitFunction visit, Node* n) const
{
    if (!n)
    {
        return;
    }
    visit(n->key);
    preorderCall(visit, n->left);
    preorderCall(visit, n->right);
}


template <typename ElementType>
void AVLSet<ElementType>::inorderCall(VisitFunction visit, Node* n) const
{
    if (!n)
    {
        return;
    }
    inorderCall(visit, n->left);
    visit(n->key);
    inorderCall(visit, n->right);
}


template <typename ElementType>
void AVLSet<ElementType>::postorderCall(VisitFunction visit, Node* n) const
{
    if (!n)
    {
        return;
    }
    postorderCall(visit, n->left);
    postorderCall(visit, n->right);
    visit(n->key);
}


template <typename ElementType>
void AVLSet<ElementType>::leftRotate(Node*& current)
{
    Node* temp;
    temp = current->left;
    current->left = temp->right;
    temp->right = current;
    current = temp;
}


template <typename ElementType>
void AVLSet<ElementType>::rightRotate(Node*& current)
{
    Node* temp;
    temp = current->right;
    current->right = temp->left;
    temp->left = current;
    current = temp;
}


template <typename ElementType>
void AVLSet<ElementType>::rotateTwice(Node*& current, std::string direction)
{
    if (direction == "right")
    {
        rightRotate(current->left);
        leftRotate(current);
    }
    else
    {
        leftRotate(current->right);
        rightRotate(current);
    }
}


#endif // AVLSET_HPP