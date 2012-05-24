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

#ifndef _FWMEMORY_H
#define _FWMEMORY_H
/**
 * FWMemory.h
 * in this header file some macros are defined for allocating objects.
 * Using macros instead of new and delete give you a chance to get more
 * control about the allocating and deallocating. You can replace the
 * new and delete operator by your customized functions.
 * <ul>
 * <li> To create an object with default constructor, you can write
 * <code>
 * Type* objPtr = NEW_DYNAMIC_DEFAULT(Type);
 * </code>
 * <li> To pass parameters the parameters must be surrounded by parentheses
 * E.G.
 * <code>
 * Type *objPtr = NEW_DYNAMIC(Type, ("abc", 5));
 * </code>
 * The parentheses are also needed even with one parameter.
 * <li> Creating an array is supported by the macro NEW_DYNAMIC_ARRAY
 * <li> The corresponding DELETE_DYNAMIC and DELETE_DYNAMIC_ARRAY are
 * used for deleting objects
 * </ol>
 */


//============================================================
// Dynamic memory allocation - By default new and delete
// are used. During the development you can use a customized function
//  to get more controls about your memory
//============================================================
/**
 * create an object with default constructor
 */
#define NEW_DYNAMIC_DEFAULT(Type)        (new Type())
/**
 * constructor with an argument list. Arguments must be surrounded by
 * parentheses
 * e.g. x = NEW_DYNAMIC(T, (5, "test"));
 *  ARGS must always have parentheses, even if it has only one argument.
 */

#define NEW_DYNAMIC(Type, ARGS)          (new Type ARGS)
#define NEW_DYNAMIC_ARRAY(Type, size)    (new Type[size])
#define DELETE_DYNAMIC(var)              (delete var)
#define DELETE_DYNAMIC_ARRAY(array)      (delete[] array)


#endif  // end of #ifndef _FWMEMORY_H


