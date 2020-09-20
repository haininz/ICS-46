// HashSet.hpp
//
// ICS 46 Spring 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;

    //Keep track of the load factor of the hash table to decide when to rehash
    double loadFactor() const;
    //Keep track of the capacity of the hash table; double when rehash
    unsigned int capacity;

private:
    //Define the hash table
    struct Node
    {
        ElementType key;
        Node* next = nullptr;
        static void deleteNode(Node* n)
        {
            if (n)
            {
                deleteNode(n->next);
            }
            delete n;
        }
    };

    HashFunction hashFunction;
    Node** hashTable;
    unsigned int hash_table_capacity;
    unsigned int hash_table_size;
    unsigned int num_key;
};


namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}



template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{ hashFunction }
{
    hash_table_capacity = DEFAULT_CAPACITY;
    hash_table_size = 0;
    num_key = 0;
    hashTable = new Node * [DEFAULT_CAPACITY];
    for (unsigned int i = 0; i < hash_table_capacity; ++i)
    {
        hashTable[i] = new Node;
    }
}



template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    for (unsigned int i = 0; i < hash_table_capacity; ++i) 
    {
        Node::deleteNode(hashTable[i]);
    }
    delete[] hashTable;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{ impl_::HashSet__undefinedHashFunction<ElementType> }
{
    hash_table_capacity = s.hash_table_capacity;
    hash_table_size = s.hash_table_size;
    hashTable = new Node * [hash_table_capacity];
    for (unsigned int i = 0; i < hash_table_capacity; ++i)
    {
        hashTable[i] = new Node;
    }
    for (unsigned int j = 0; j < hash_table_capacity; ++j) 
    {
        Node* tmp = hashTable[j];
        Node* temp2 = s.hashTable[j];
        while (temp2->next)
        {
            tmp->next = new Node;
            tmp->next->key = temp2->next->key;
            temp2 = temp2->next;
            tmp = tmp->next;
        }
    }
}

template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{ impl_::HashSet__undefinedHashFunction<ElementType> }
{
    hash_table_capacity = s.hash_table_capacity;
    hash_table_size = s.hash_table_size;
    hashTable = new Node * [hash_table_capacity];
    for (unsigned int i = 0; i < hash_table_capacity; ++i)
    {
        hashTable[i] = new Node;
    }
    for (unsigned int j = 0; j < hash_table_capacity; ++j) 
    {
        Node* tmp = hashTable[j];
        Node* temp2 = s.hashTable[j];
        while (temp2->next)
        {
            tmp->next = new Node;
            tmp->next->key = temp2->next->key;
            temp2 = temp2->next;
            tmp = tmp->next;
        }
    }
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    for (unsigned int i = 0; i < hash_table_capacity; ++i)
    {
        Node::deleteNode(hashTable[i]);
    }
    delete[] hashTable;
    hash_table_size = s.size();
    hash_table_capacity = s.hash_table_capacity;
    hashTable = new Node * [hash_table_capacity]();
    for (unsigned int j = 0; j < hash_table_capacity; ++j)
    {
        hashTable[j] = new Node;
    }
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    for (unsigned int i = 0; i < s.hash_table_capacity; ++i)
    {
        while (s.hashTable[i]->next != nullptr)
        {
            this->add(s.hashTable[i]->next->key);
        }
    }
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    unsigned int index;
    //Rehash needed
    if ((hash_table_size/hash_table_capacity) > 0.8) {
        unsigned int tempCapacity = hash_table_capacity;
        Node** tempHash = hashTable;
        hash_table_capacity *= 2;
        hashTable = new Node * [hash_table_capacity];
        num_key = 0;
        for (unsigned int i = 0; i < hash_table_capacity; ++i)
        {
            hashTable[i] = new Node;
        }
        for (unsigned int j = 0; j < tempCapacity; ++j)
        {
            Node* tempHead = tempHash[j];
            while (tempHead->next)
            {
                ElementType tempElement = tempHead->next->key;
                index = hashFunction(tempElement)%hash_table_capacity;
                if (hashTable[index]->next) 
                {
                    if (!this->isElementAtIndex(tempElement, index))
                    {
                        Node* tmp = hashTable[index];
                        while (tmp->next)
                        {
                            tmp = tmp->next;
                        }
                        tmp->next = new Node;
                        tmp->next->key = tempElement;
                        num_key ++;
                    }
                }
                else 
                {
                    hashTable[index]->next = new Node;
                    hashTable[index]->next->key = tempElement;
                    hash_table_size ++;
                    num_key ++;
                }
                tempHead = tempHead->next;
            }
        }
    }
    index = hashFunction(element)%hash_table_capacity;
    if (hashTable[index]->next)
    {
        if (!this->isElementAtIndex(element, index))
        {
            Node* tmp = hashTable[index];
            while (tmp->next)
            {
                tmp = tmp->next;
            }
            tmp->next = new Node;
            tmp->next->key = element;
            num_key ++;
        }
    }
    else
    {
        hashTable[index]->next = new Node;
        hashTable[index]->next->key = element;
        hash_table_size ++;
        num_key ++;
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int index = hashFunction(element)%hash_table_capacity;
    Node* temp = hashTable[index];
    while (temp->next)
    {
        if (temp->next->key == element)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return num_key;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    unsigned int counter = 0;
    Node* head = hashTable[index];
    while (head->next)
    {
        counter ++;
        head = head->next;
    }
    return counter;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    Node* head = hashTable[index];
    while (head->next)
    {
        if (element == head->next->key)
        {
            return true;
        }
        head = head->next;
    }
    return false;
}


#endif // HASHSET_HPP