/***************************************************************************
 *  openICM-framework
 ***************************************************************************
 *  Copyright 2010 Joachim Wietzke and Manh Tien Tran
 *
 *  Contact and informations:
 *  University of Applied Sciences Darmstadt, Germany
 *  	http://www.fbi.h-da.de/~j.wietzke
 *  or
 *  	http://www.fbi.h-da.de/~openicm
 *
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  	http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
*****************************************************************************/

#ifndef _TVECTOR_H
#define _TVECTOR_H
/**
 * Simple template for vector. Vectors are like dynamic array. It can
 * grow if needed.
 * Working with TVector you must take decisions about
 * <ol>
 * <li> the initial capacity. Choosing a small capacity causes the vector
 *      growing to often. A big capacity means waste of memory.
 * <li> the increment should be chosen carefully.
 * </ol>
 * Rule of thumb: the initial capacity should be the average size and
 * the increment is the standard deviation.
 * If you plan to use TVector as container for objects you must take care of
 * the following points:
 * <ol>
 * <li> The class must support default constructor, because arrays of
 *      objects must be created.
 * <li> The class must support assignment operator. The object is
 *      copied into the internal buffer of the vector.
 * <li> The class must support the comparison operator == due to the
 *      find-method
 * </ol>
 */

//============================================================
// INCLUDES
//============================================================
#ifndef _GLOBAL_H
   #include "Global.h"
#endif

#ifndef _FWASSERTION_H
   #include "FWAssertion.h"  		//  for assertion
#endif

#ifndef _FWMEMORY_H
	#include "FWMemory.h"            // for macro to get memory
#endif

//============================================================
// TEMPLATE DECLARATION
//============================================================

template<class T>
class TVector
{
public:
	/**
	 * create a vector
	 * constrcutor - creates an empty vector (no elements) with a default
	 * capacity and default increment.
	 */
	TVector( );

   /**
    *  create a vector
    *  constructor - create an empty vector (no element) with the
    *  initial capacity.
    *  @param initialCapacity the capacity of the vector at the beginning.
    *  @param increment   is used to increment the capacity if a new element
    *  is added.
    */
   TVector(Int32 initialCapacity, Int32 increment);

   /**
    * Destructor
    */
   ~TVector();

   /**
    *  add an element into the vector
    *  This method adds a new element into the vector. If needed the
    *  capacity is increased by an amount given by increment.
    *  The element is added at the end of the vector
    *
    *  @param   elem  element to be added
    */
   void add(const T& elem);


   /**
    *  add an element into the vector
    *  This method adds a new element into the vector. If needed the
    *  capacity is increased by an amount given by increment.
    *  The element is added at the end of the vector if it was not contained
    *
    *  @param   elem  element to be added
    */
   void addUnique(const T& elem);

   /**
    *  remove an element from vector
    *  This method removes the element at the position given from
    *  the vector. Note that the element is a pointer it is
    *  not deleted. It must be done by the client (calling instance). The
    *  capacity of the vector is not affected.
    *
    *  @param   pos  position of the element to be removed
    */
   void removeElementAt(Int32 pos);

   /**
    *  remove an element from vector
    *  This method removes the element given from the vector if it is
    *  stored in the vector. Note that the element is a pointer it is
    *  not deleted. It must be done by the client (calling instance). The
    *  capacity of the vector is not affected.
    *
    *  @param   elem  element to remove
    */
   void removeElement(const T& elem);

   /*
    *  find an element in the vector
    *  This method finds an element within the vector which matches the
    *  argument (if present). The capacity of the vector is not affected.
    *  Note that this procedure searches the vector from back to front,
    *  if the vector contains 2 or more elements that match the argument,
    *  the last such element in the vector will be found.
    *
    *  @param   elem  element to find
    *  @return  if found the index of the matching element, else -1
    */
   Int32 find(const T& elem) const;


   /**
    * get the size of the cector
    * @return the current length of the vector. Note that the
    *  maximum index is getSize()-1
    */
   Int32 getSize(void) const;
   /**
    *  index-operator
    *  This method returns an element at the desired position.
    *
    *  @param   pos   position in the vector
    *  @return  element at the position desired
    */
   T& operator[](Int32 pos);

   /**
    *  index-operator
    *  This method returns an element at the desired position.
    *
    *  @param   pos   position in the vector
    *  @return  element at the position desired
    */
   const T& operator[](Int32 pos) const;

   /**
    * clear all element of the vector
    */
   void clearAll(void);

private:
   // to avoid misusing
   TVector(const TVector<T>& rhs);
   const TVector& operator=(const TVector& other);

   // current size
   Int32 mSize;
   // current capacity
   Int32 mCapacity;
   // to store the elements
   T *mContent;
   // increment when the array must be grown
   Int32 mIncrement;
};


//============================================================
// TEMPLATE IMPLEMENTATION
//============================================================
/**
 * create a vector
 * constrcutor - creates an empty vector (no elements) with a default
 * capacity and default increment.
 */
template<class T>
TVector<T>::TVector(  )
   : mSize(0),
     mCapacity(8),
     mContent(NEW_DYNAMIC_ARRAY(T,8)),
     mIncrement(8)
{
}


/*
 *  create a vector
 *  constructor - create an empty vector (no element) with the
 *  initial capacity.
 *  @param initialCapacity the capacity of the vector at the beginning.
 *  @param increment   is used to increment the capacity if a new element
 *  is added.
 */
template<class T>
TVector<T>::TVector( Int32 initialCapacity, Int32 increment )
   : mSize(0),
     mCapacity(initialCapacity),
     mContent(NEW_DYNAMIC_ARRAY(T,initialCapacity)),
     mIncrement(increment)
{
   ASSERTION( 0 < initialCapacity );
   ASSERTION( 0 < increment ) ;
}


/*
 *  Destructor
 */
template<class T>
TVector<T>::~TVector()
{
   DELETE_DYNAMIC_ARRAY(mContent);
}


/*
 *  add an element into the vector
 *  This method adds a new element into the vector. If needed the
 *  capacity is increased by an amount given by increment.
 *  The element is added at the end of the vector
 *
 *  @param   elem  element to be added
 */
template<class T>
void TVector<T>::add(const T& elem)
{
   if ( mCapacity == mSize )
   {
      T *tmp = NEW_DYNAMIC_ARRAY(T, mCapacity + mIncrement);
      // copy old content
      for (Int32 i = 0; i < mSize; i++)
      {
         tmp[i] = mContent[i];
      }
      // delete old content
      DELETE_DYNAMIC_ARRAY(mContent);
      mContent = tmp;
      mCapacity += mIncrement;
   }
   mContent[mSize] = elem;
   mSize++;
}  // end of add


/*
 *  add an element into the vector
 *  This method adds a new element into the vector. If needed the
 *  capacity is increased by an amount given by increment.
 *  The element is added at the end of the vector if it was not contained
 *
 *  @param   elem  element to be added
 */
template<class T>
void TVector<T>::addUnique(const T& elem)
{
   if (-1 == find(elem))
   {
      add(elem);
   }
}


/*
 *  remove an element from vector
 *  This method removes the element at the position given from
 *  the vector. Note that the element is a pointer it is
 *  not deleted. It must be done by the client (calling instance). The
 *  capacity of the vector is not affected.
 *
 *  @param   pos  position of the element to be removed
 */
template<class T>
void TVector<T>::removeElementAt(Int32 pos)
{
   Int32 i;                      // for -index
   ASSERTION(pos < mSize);

   if ((pos >= mSize) || (pos < 0))
   {
      return;
   }

   mSize--;
   // delete
   for (i = pos; i < mSize; i++)
   {
      mContent[i] = mContent[i+1];
   }
}



/*
 *  remove an element from vector
 *  This method removes the element given from the vector if it is
 *  stored in the vector. Note that the element is a pointer it is
 *  not deleted. It must be done by the client (calling instance). The
 *  capacity of the vector is not affected.
 *
 *  @param   elem  element to remove
 */
template<class T>
void TVector<T>::removeElement(const T& elem)
{
   Int32 i = find( elem );
   if ( 0 <= i )
      removeElementAt(i);
}

/*
 *  find an element in the vector
 *  This method finds an element within the vector which matches the
 *  argument (if present). The capacity of the vector is not affected.
 *  Note that this procedure searches the vector from back to front,
 *  if the vector contains 2 or more elements that match the argument,
 *  the last such element in the vector will be found.
 *
 *  @param   elem  element to find
 *  @return  if found the index of the matching element, else -1
 */
template<class T>
Int32 TVector<T>::find(const T& elem) const
{
   for (Int32 i = 0; i < mSize; i++)
   {
      if (mContent[i] == elem)
      {
         return i;
      }
   }
   return -1;
}


/*
 *  @return the current length of the vector. Note that the
 *  maximum index is getSize()-1
 */
template<class T>
inline Int32 TVector<T>::getSize(void) const
{
   return mSize;
}



/*
 *  index-operator
 *  This method returns an element at the desired position.
 *
 *  @param   pos   position in the vector
 *  @return  element at the position desired
 */
template<class T>
inline T & TVector<T>::operator[](Int32 pos)
{
   ASSERTION( (pos >= 0) && (pos < mSize));
   return mContent[pos];
}


/*
 *  index-operator
 *  This method returns an element at the desired position.
 *
 *  @param   pos   position in the vector
 *  @return  element at the position desired
 */
template<class T>
inline const T & TVector<T>::operator[](Int32 pos) const
{
   ASSERTION( (pos >= 0) && (pos < mSize));
   return mContent[pos];
}


/*
 * clear all element of the vector
 */
template<class T>
inline void TVector<T>::clearAll(void)
{
   mSize = 0;
}


typedef TVector<Int32> Int32Vector;
typedef TVector<void*> VoidPtrVector;
#endif // end of _TVECTOR_H
