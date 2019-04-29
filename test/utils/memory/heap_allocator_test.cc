/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/memory/heap_allocator.h"

#include <gtest/gtest.h>

namespace quasi_game_engine {

TEST(HeapAllocatorTest, constructor_test) { HeapAllocator allocator(); }

}  // namespace quasi_game_engine

// tests
// allocate works
// allocate with constructor
// Allocate too large
// Allocate too small

// Deallocate works
// Deallocate null pointer?
// Deallocate twice