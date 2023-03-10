/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Joel Jossie, Gergo Medveczky
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
 
class TestList;        // forward declaration for unit tests
class TestHash;

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:  
   // 
   // Construct
   //

   list();
   list(list <T> & rhs);
   list(list <T>&& rhs);
   list(size_t num, const T & t);
   list(size_t num);
   list(const std::initializer_list<T>& il);
   template <class Iterator>
   list(Iterator first, Iterator last);
  ~list()  { clear(); }

   // 
   // Assign
   //

   list <T> & operator = (list &  rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T>& il);
   void swap(list <T>& rhs);

   //
   // Iterator
   //

   class  iterator;
   iterator begin()  { return iterator(pHead); }
   iterator rbegin() { return iterator(pTail); }
   iterator end()    { return iterator(); }

   //
   // Access
   //

   T& front();
   T& back();

   //
   // Insert
   //

   void push_front(const T&  data);
   void push_front(      T&& data);
   void push_back (const T&  data);
   void push_back (      T&& data);
   iterator insert(iterator it, const T& data);
   iterator insert(iterator it, T&& data);

   //
   // Remove
   //

   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator& it);

   // 
   // Status
   //

   bool empty()  const { return (size() == 0); }
   size_t size() const { return numElements;   }


private:
   // nested linked list class
   class Node;

   // member variables
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;    // pointer to the beginning of the list
   Node * pTail;    // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T>
class list <T> :: Node
{
public:
   //
   // Construct
   //
   Node()              : pNext(nullptr), pPrev(nullptr)                         { }
   Node(const T& data) : pNext(nullptr), pPrev(nullptr), data(data)             { }
   Node(T&& data)      : data(std::move(data)), pNext(nullptr), pPrev(nullptr)  { }


   //
   // Data
   //

   T data;             // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T>
class list <T> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT>
   friend class custom::list;
public:
   // constructors, destructors, and assignment operator
   iterator()                      : p(nullptr) {}
   iterator(Node * p)              : p(p)       {}
   iterator(const iterator  & rhs) : p(rhs.p)   {}
   iterator & operator = (const iterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return (p == rhs.p); }
   bool operator != (const iterator & rhs) const { return (p != rhs.p); }

   // dereference operator, fetch a node
   T & operator * ()
   {
      if (p)
         return p->data;
      else
         throw "ERROR: unable to access data from an empty list";
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      if (p)
         p = p->pNext;
      return *this;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      if (p)
         p = p->pNext;
      return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      if (p)
         p = p->pPrev;
      return *this;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      if (p)
         p = p->pPrev;
      return *this;
   } 

   // two friends who need to access p directly
   friend iterator list <T> :: insert(iterator it, const T &  data);
   friend iterator list <T> :: insert(iterator it,       T && data);
   friend iterator list <T> :: erase(const iterator & it);

private:

   typename list <T> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) : numElements(0), pHead(nullptr), pTail(nullptr)
{
   for (int i = 0; i < num; i++)
      push_back(t);
}

/*****************************************
 * LIST :: ITERATOR constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
template <class Iterator>
list <T> ::list(Iterator first, Iterator last) : pHead(nullptr), pTail(nullptr), numElements(0)
{
   for (auto it = first; it != last; it++)
      push_back(*it);
}

/*****************************************
 * LIST :: INITIALIZER constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
list <T> ::list(const std::initializer_list<T>& il) : pHead(nullptr), pTail(nullptr), numElements(0)
{
   for (auto it = il.begin(); it != il.end(); it++)
      push_back(*it);
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num) : numElements(0), pHead(nullptr), pTail(nullptr)
{
   for (int i = 0; i < num; i++)
      push_back(T());
}

/*****************************************
 * LIST :: DEFAULT constructors
 ****************************************/
template <typename T>
list <T> ::list() : pHead(nullptr), pTail(nullptr), numElements(0) { }

/*****************************************
 * LIST :: COPY constructors
 ****************************************/
template <typename T>
list <T> ::list(list& rhs) : pHead(nullptr), pTail(nullptr), numElements(0)
{
   *this = rhs;
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T>
list <T> ::list(list <T>&& rhs)  : pHead(rhs.pHead), pTail(rhs.pTail), numElements(rhs.numElements)
{
   rhs.pHead = rhs.pTail = nullptr;
   rhs.numElements = 0;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (list <T> && rhs)
{
   // Create iterators for side-by-side lists
   auto itRhs = rhs.begin();
   auto itLhs = begin();

   // Copy rhs to lhs until one list is empty
   while (itRhs != rhs.end() && itLhs != end())
   {
      *itLhs = std::move(*itRhs);
      ++itLhs;
      ++itRhs;
   }

   // If rhs is longer than lhs, create new nodes on lhs
   if (itRhs != rhs.end())
   {
      while (itRhs != rhs.end())
      {
         push_back(std::move(*itRhs));
         ++itRhs;
      }
      
   }

   // If lhs is longer than rhs, delete extra nodes
   else if(rhs.empty())
      clear();
   
   else if (itLhs != end())
      while (itLhs.p)
         itLhs = erase(itLhs);
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> & rhs)
{
   // Create iterators for side-by-side lists
   auto itRhs = rhs.begin();
   auto itLhs = begin();
   // Copy rhs to lhs until one list is empty
   while (itRhs != rhs.end() && itLhs != end())
   {
      *itLhs = *itRhs;
      ++itLhs;
      ++itRhs;
   }
   // If rhs is longer than lhs, create new nodes on lhs
   if (itRhs != rhs.end())
   {
      while (itRhs != rhs.end())
      {
         push_back(*itRhs);
         ++itRhs;
      }
      
   }
   // If lhs is longer than rhs, delete extra nodes
   else if(rhs.empty())
      clear();
   
   else if (itLhs != end())
      while (itLhs.p)
         itLhs = erase(itLhs);
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
{
   auto itLhs = begin();
   // Loop through rhs list
   for (auto item : rhs)
   {
      // Create new nodes if rhs is longer than lhs
      if (itLhs == end())
      {
         push_back(item);
      }
      // Otherwise, just assign the value over to lhs
      else
      {
         *itLhs = item;
      }
      itLhs++;
   }
   // Trim off extra space if lhs is longer than rhs
   if (itLhs != end())
      while (itLhs.p)
         itLhs = erase(itLhs);
   
   return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
void list <T> :: clear()
{
   while (pHead != nullptr)
   {
      auto pDelete = pHead;
      pHead = pHead->pNext;
      delete pDelete;
      numElements--;
   }
   pTail = nullptr;
   
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_back(const T & data)
{
   if(pHead == nullptr)
   {
      pHead = pTail = new list <T> ::Node(data);
   }
   else
   {
      auto newElement = new list <T> ::Node(data);
      newElement->pPrev = pTail;
      pTail->pNext = newElement;
      pTail = newElement;
   }
   numElements++;
   
}

template <typename T>
void list <T> ::push_back(T && data)
{
   if(pHead == nullptr)
   {
      pHead = pTail = new list <T> ::Node(data);
   }
   else
   {
      auto newElement = new list <T> ::Node(data);
      newElement->pPrev = pTail;
      pTail->pNext = newElement;
      pTail = newElement;
   }
   numElements++;
   
   
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_front(const T & data)
{
   if(pTail == nullptr)
   {
      pHead = pTail = new list <T> ::Node(data);
   }
   else
   {
      auto newElement = new list <T> ::Node(data);
      newElement->pNext = pHead;
      pHead->pPrev = newElement;
      pHead = newElement;
   }
   numElements++;
}

template <typename T>
void list <T> ::push_front(T && data)
{
   if(pTail == nullptr)
   {
      pHead = pTail = new list <T> ::Node(data);
   }
   else
   {
      auto newElement = new list <T> ::Node(data);
      newElement->pNext = pHead;
      pHead->pPrev = newElement;
      pHead = newElement;
   }
   numElements++;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_back()
{
   erase(iterator(pTail));
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_front()
{
   erase(iterator(pHead));
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: front()
{
   if (pHead)
      return pHead->data;
   else
      throw "ERROR: unable to access data from an empty list";
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: back()
{
   if (pTail)
      return pTail->data;
   else
      throw "ERROR: unable to access data from an empty list";
}

/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator  list <T> :: erase(const list <T> :: iterator & it)
{
   if (it.p == nullptr)
      return nullptr;

   if (it.p->pPrev)           // Take care of the previous node
      it.p->pPrev->pNext = it.p->pNext;
   else                       // There is no previous node, so move the head
   {
      assert(it.p == pHead);  // This is the head, right?
      pHead = it.p->pNext;
   }

   if (it.p->pNext)           // Take care of the next node
      it.p->pNext->pPrev = it.p->pPrev;
   else                       // There is no next node, so move the tail
   {
      assert(it.p == pTail);  // This is the tail, right?
      pTail = it.p->pPrev;    // If there is only one node (no pPrev OR pNext), then
                              // both of these blocks are executed, but that's ok
                              // because this will just assign pHead or pTail to nullptr.
   }

   list<T>::Node * pReturn;

   if (it.p->pNext)
      pReturn = it.p->pNext;
   else
      pReturn = nullptr;

   delete it.p;
   numElements--;
   return pReturn;
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
                                                 const T & data) 
{
   // Inserting if empty
   if (empty()) {
      pHead = pTail = new list<T>::Node(data);
      numElements = 1;
      return begin();
   }

   // Inserting at the end
   else if (it == end() )
   {
      // this is the same as push_back(data);
      auto pNew = new list<T>::Node(data);
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
      numElements++;
      return iterator(pNew);
   }

   // Inserting at the beginning or middle
   else if (it != end())
   {
      auto pNew = new list<T>::Node(data);
      pNew->pPrev = it.p->pPrev;
      pNew->pNext = it.p;

      if (pNew->pPrev)
         pNew->pPrev->pNext = pNew;
      else
         pHead = pNew;

      if (pNew->pNext)
         pNew->pNext->pPrev = pNew;
      else
         pTail = pNew;

      numElements++;
      return iterator(pNew);
   }
}

template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
   T && data)
{
   // Inserting if empty
   if (empty()) {
      pHead = pTail = new list<T>::Node(std::move(data));
      numElements = 1;
      return begin();
   }

   // Inserting at the end
   else if (it == end() )
   {
      // this is the same as push_back(data);
      auto pNew = new list<T>::Node(std::move(data));
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
      numElements++;
      return iterator(pNew);
   }

   // Inserting at the beginning or middle
   else if (it != end())
   {
      auto pNew = new list<T>::Node(std::move(data));
      pNew->pPrev = it.p->pPrev;
      pNew->pNext = it.p;

      if (pNew->pPrev)
         pNew->pPrev->pNext = pNew;
      else
         pHead = pNew;

      if (pNew->pNext)
         pNew->pNext->pPrev = pNew;
      else
         pTail = pNew;

      numElements++;
      return iterator(pNew);
   }
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
void swap(list <T> & lhs, list <T> & rhs)
{
   std::swap(lhs.pHead, rhs.pHead);
   std::swap(lhs.pTail, rhs.pTail);
   std::swap(lhs.numElements, rhs.numElements);
}

template <typename T>
void list<T>::swap(list <T>& rhs)
{
   std::swap(pHead, rhs.pHead);
   std::swap(pTail, rhs.pTail);
   std::swap(numElements, rhs.numElements);
}

//#endif
}; // namespace custom
