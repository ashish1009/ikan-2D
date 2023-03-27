//
//  ikan_common.h
//  ikan
//
//  Created by Ashish . on 27/03/23.
//

#pragma once

// -------------------------------------------
// Copy and Move Constructors and Operators
// -------------------------------------------
/// This MACRO Deletes the Constructors (Default, Copy and Move ) and Operator = (Copy and Move) for any class to make pure static class
#define MAKE_PURE_STATIC(x) \
x() = delete; \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator =(const x&) = delete; \
x& operator =(x&&) = delete; \

/// This MACRO deletes the Constructors (Copy and Move ) and Operator = (Copy and Move) for any class x to make a class singleton.
/// NOTE: default constructor still need to be private
#define DELETE_COPY_MOVE_CONSTRUCTORS(x) \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator=(const x&) = delete; \
x& operator =(x&&) = delete; \

/// This MACRO Defines the Constructors (Copy and Move ) and Operator = (Copy and Move) for any class x
#define DEFINE_COPY_MOVE_CONSTRUCTORS(x) \
x(const x&); \
x(x&&); \
x& operator=(const x&); \
x& operator =(x&&); \
