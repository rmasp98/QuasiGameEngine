/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/memory/heap_block_list.h"

#include <iostream>

#include <gmock/gmock.h>
using namespace ::testing;

namespace quasi_game_engine {

class HeapBlockListTest : public Test {
 public:
  int min_block_size = 32;  // Needs to be big enough to fit FreeHeader
  HeapBlockList block_list{min_block_size};

  int base_block_size = 32;
  int requested_block_size = base_block_size - sizeof(BlockHeader);

  int normal_block_size = base_block_size + min_block_size;
  BlockHeader* normal_block =
      reinterpret_cast<BlockHeader*>(malloc(normal_block_size));
  BlockHeader* another_block =
      reinterpret_cast<BlockHeader*>(malloc(normal_block_size));

  int small_block_size = base_block_size;
  BlockHeader* small_block =
      reinterpret_cast<BlockHeader*>(malloc(small_block_size));

  int large_block_size = 2 * (base_block_size + min_block_size);
  BlockHeader* large_block =
      reinterpret_cast<BlockHeader*>(malloc(large_block_size));

  char* double_block;

  void AddTwoAdjacentBlocks() {
    double_block = reinterpret_cast<char*>(malloc(large_block_size));
    BlockHeader* first_block = reinterpret_cast<BlockHeader*>(double_block);
    BlockHeader* second_block =
        reinterpret_cast<BlockHeader*>(double_block + normal_block_size);
    block_list.AddBlock(first_block, normal_block_size, true);
    block_list.AddBlock(second_block, normal_block_size, true);
  }

  void TearDown() override {
    // TODO: Figure out how to free
    // free(normal_block);
    // free(another_block);
    // free(small_block);
    // free(large_block);
  }
};

TEST_F(HeapBlockListTest, ReturnsNullForEmptyFreeList) {
  ASSERT_THAT(block_list.GetFreeBlock(requested_block_size), IsNull());
}

TEST_F(HeapBlockListTest, ReturnsOnlyBlockAfterOneAdded) {
  block_list.AddBlock(normal_block, normal_block_size, true);
  ASSERT_THAT(block_list.GetFreeBlock(requested_block_size),
              Eq(normal_block + 1));
}

TEST_F(HeapBlockListTest, RemovesBlockWhenRetrieved) {
  block_list.AddBlock(normal_block, normal_block_size, true);
  block_list.GetFreeBlock(requested_block_size);
  ASSERT_THAT(block_list.GetFreeBlock(requested_block_size), IsNull());
}

TEST_F(HeapBlockListTest, ReturnsNullForSizeZero) {
  block_list.AddBlock(normal_block, normal_block_size, true);
  ASSERT_THAT(block_list.GetFreeBlock(0), IsNull());
}

TEST_F(HeapBlockListTest, ReturnsNullForSizeLessThanZero) {
  block_list.AddBlock(normal_block, normal_block_size, true);
  ASSERT_THAT(block_list.GetFreeBlock(-5), IsNull());
}

TEST_F(HeapBlockListTest, AcceptsTwoBlocksAndReturnsBoth) {
  block_list.AddBlock(another_block, normal_block_size, true);
  block_list.AddBlock(normal_block, normal_block_size, true);
  void* first_block = block_list.GetFreeBlock(requested_block_size);
  void* second_block = block_list.GetFreeBlock(requested_block_size);
  ASSERT_TRUE(
      (first_block == normal_block + 1 && second_block == another_block + 1) ||
      (second_block == normal_block + 1 && first_block == another_block + 1))
      << "Returned blocks (" << first_block << " and " << second_block
      << ") are not the same as added blocks (" << normal_block + 1 << " and "
      << another_block + 1 << ")";
}

TEST_F(HeapBlockListTest, ReturnsMinimumBlockSizeLargerThanRequired) {
  block_list.AddBlock(normal_block, normal_block_size, true);
  block_list.AddBlock(small_block, small_block_size, true);
  ASSERT_THAT(block_list.GetFreeBlock(requested_block_size),
              Eq(normal_block + 1));
}

TEST_F(HeapBlockListTest, ReturnsSmallestAvailableBlock) {
  block_list.AddBlock(normal_block, normal_block_size, true);
  block_list.AddBlock(large_block, large_block_size, true);
  ASSERT_THAT(block_list.GetFreeBlock(requested_block_size), normal_block + 1);
}

TEST_F(HeapBlockListTest, MergesTwoAdjacentBlocks) {
  AddTwoAdjacentBlocks();
  block_list.MergeFreeBlocks(double_block, large_block_size);
  block_list.GetFreeBlock(requested_block_size);
  ASSERT_THAT(block_list.GetFreeBlock(requested_block_size), IsNull());
}

}  // namespace quasi_game_engine

// tests
// Threaded test for AddBlock
// Threaded test for GetFreeBlock
// Merges several adjacent free blocks
// Does not merge non adjacent free blocks
// Does not merge adjacent free blocks on seperate pages