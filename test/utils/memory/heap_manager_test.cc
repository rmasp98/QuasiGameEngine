/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/memory/heap_manager.h"

#include <gtest/gtest.h>

namespace quasi_game_engine {

TEST(HeapManagerTest, constructor_test) { HeapManager manager(); }

}  // namespace quasi_game_engine

// tests
// return pointer to free block
//
// Throw if request too large block

//