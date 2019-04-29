/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/memory/heap_page_list.h"

#include <gmock/gmock.h>
using namespace ::testing;

namespace quasi_game_engine {

class HeapPageListTest : public Test {
 public:
  int page_size = 10;
  HeapPageList page_list{page_size};
};

TEST_F(HeapPageListTest, ReturnsZeroMemoryBeforePageCreation) {
  ASSERT_THAT(page_list.GetTotalMemory(), Eq(0));
}

TEST_F(HeapPageListTest, ReturnsCorrectMemoryAfterPageCreation) {
  page_list.CreatePage();
  ASSERT_THAT(page_list.GetTotalMemory(), Eq(page_size));
}

}  // namespace quasi_game_engine

// tests
//