/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/memory/heap_free_list.h"

#include <ctime>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

#include <gmock/gmock.h>
using namespace ::testing;

namespace quasi_game_engine {

class HeapFreeListTest : public Test {
 public:
  HeapFreeList free_list;

  int normal_block_size = 128;
  FreeHeader* normal_block =
      reinterpret_cast<FreeHeader*>(malloc(normal_block_size));
  FreeHeader* another_block =
      reinterpret_cast<FreeHeader*>(malloc(normal_block_size));

  int small_block_size = normal_block_size / 2;
  FreeHeader* small_block =
      reinterpret_cast<FreeHeader*>(malloc(small_block_size));

  int large_block_size = 2 * normal_block_size;
  FreeHeader* large_block =
      reinterpret_cast<FreeHeader*>(malloc(large_block_size));

  void TearDown() override {
    free(normal_block);
    free(another_block);
    free(small_block);
    free(large_block);
  }
};

TEST_F(HeapFreeListTest, ReturnsNullForEmptyFreeList) {
  ASSERT_THAT(free_list.GetBlock(normal_block_size), IsNull());
}

TEST_F(HeapFreeListTest, ReturnsOnlyBlockAfterOneAdded) {
  free_list.AddBlock(normal_block, normal_block_size);
  ASSERT_THAT(free_list.GetBlock(normal_block_size), Eq(normal_block));
}

TEST_F(HeapFreeListTest, RemovesBlockWhenRetrieved) {
  free_list.AddBlock(normal_block, normal_block_size);
  free_list.GetBlock(normal_block_size);
  ASSERT_THAT(free_list.GetBlock(normal_block_size), IsNull());
}

TEST_F(HeapFreeListTest, AcceptsTwoBlocksAndReturnsBoth) {
  free_list.AddBlock(another_block, normal_block_size);
  free_list.AddBlock(normal_block, normal_block_size);
  void* first_block = free_list.GetBlock(normal_block_size);
  void* second_block = free_list.GetBlock(normal_block_size);
  ASSERT_TRUE((first_block == normal_block && second_block == another_block) ||
              (second_block == normal_block && first_block == another_block))
      << "Returned blocks (" << first_block << " and " << second_block
      << ") are not the same as added blocks (" << normal_block << " and "
      << another_block << ")";
}

TEST_F(HeapFreeListTest, ReturnsBlockLargerOrEqualToRequestedSize) {
  free_list.AddBlock(normal_block, normal_block_size);
  free_list.AddBlock(small_block, small_block_size);
  ASSERT_THAT(free_list.GetBlock(normal_block_size), Eq(normal_block));
}

TEST_F(HeapFreeListTest, ReturnsSmallestAvailableBlock) {
  free_list.AddBlock(normal_block, normal_block_size);
  free_list.AddBlock(large_block, large_block_size);
  ASSERT_THAT(free_list.GetBlock(normal_block_size), Eq(normal_block));
}

// TEST_F(HeapFreeListTest, PerformanceTest) {
//   int n_blocks = 20;

//   std::vector<int> block_sizes(n_blocks);
//   int total_size = 0;
//   for (auto& size : block_sizes) {
//     size = (rand() % 32) + sizeof(FreeHeader);
//     total_size += size;
//   }

//   char* memory = reinterpret_cast<char*>(malloc(total_size));

//   int offset = 0;
//   for (auto& size : block_sizes) {
//     free_list.AddBlock(reinterpret_cast<FreeHeader*>(memory + offset), size);
//     offset += size;
//   }

//   std::clock_t start = std::clock();
//   for (int i = 0; i < n_blocks; ++i) {
//     void* block = malloc(block_sizes[i]);
//     (void)block;
//   }

//   double malloc_duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

//   for (int i = 0; i < n_blocks; ++i) {
//     free_list.GetBlock(block_sizes[i]);
//   }

//   double prealloc_duration =
//       (std::clock() - start) / (double)CLOCKS_PER_SEC - malloc_duration;
//   ASSERT_THAT(prealloc_duration * 1000, Gt(malloc_duration * 1000));
// }

///////////////////////////////////////////////////////////////////////////////
//
// Concurrency tests

// class HeapFreeListConcurrentTest : public Test {
//  public:
//   HeapFreeList free_list;
//   char* memory;

//   std::vector<int> block_sizes;
//   std::vector<FreeHeader*> blocks;
//   std::mutex map_lock;

//   void AddBlocks(int n_adds, int offset = 0) {
//     for (int i_adds = 0; i_adds < n_adds; ++i_adds) {
//       int iter = offset * n_adds + i_adds;
//       free_list.AddBlock(blocks[iter], block_sizes[iter]);
//     }
//   }

//   std::unordered_map<long, int> GetBlocks(int n_gets, int offset = 0) {
//     std::unordered_map<long, int> out_blocks;
//     for (int i_gets = 0; i_gets < n_gets; ++i_gets) {
//       int iter = offset * n_gets + i_gets;
//       out_blocks.insert({(long)free_list.GetBlock(block_sizes[iter]), 0});
//     }
//     return out_blocks;
//   }

//   void RunInParallel(int n_threads, std::function<void(int)>&& function) {
//     std::vector<std::shared_ptr<std::thread>> threads;
//     for (int i = 0; i < n_threads; ++i) {
//       threads.push_back(std::make_shared<std::thread>(function, i));
//     }
//     for (auto& thread : threads) {
//       thread->join();
//     }
//   }

//   void CreateBlocks(int num_blocks) {
//     block_sizes.resize(num_blocks);
//     int total_size = 0;
//     for (auto& size : block_sizes) {
//       size = (rand() % 32) + sizeof(FreeHeader);
//       total_size += size;
//     }

//     memory = reinterpret_cast<char*>(malloc(total_size));

//     int offset = 0;
//     for (auto& size : block_sizes) {
//       blocks.push_back(reinterpret_cast<FreeHeader*>(memory + offset));
//       offset += size;
//     }
//   }

//   void TearDown() { free(memory); }
// };

// TEST_F(HeapFreeListConcurrentTest, CanAddBlocksSafely) {
//   int n_threads = 50, n_adds = 200, n_blocks = n_threads * n_adds;
//   CreateBlocks(n_blocks);
//   RunInParallel(n_threads, [&](int thread) { AddBlocks(n_adds, thread); });
//   ASSERT_THAT(GetBlocks(n_blocks).size(), Eq(n_blocks));
// }

// TEST_F(HeapFreeListConcurrentTest, CanGetBlocksSafely) {
//   int n_threads = 50, n_gets = 200, n_blocks = n_threads * n_gets;
//   CreateBlocks(n_blocks);
//   AddBlocks(n_blocks);
//   std::unordered_map<long, int> final_blocks;
//   RunInParallel(n_threads, [&](int thread) {
//     auto out_blocks = GetBlocks(n_gets, thread);
//     std::unique_lock<std::mutex> lock(map_lock);
//     final_blocks.insert(out_blocks.begin(), out_blocks.end());
//   });
//   ASSERT_THAT(final_blocks.size(), Eq(n_blocks));
// }

// TEST_F(HeapFreeListConcurrentTest, CanGetAndAddBlocksSafely) {
//   int n_threads = 200, n_gets = 200, n_blocks = n_threads * n_gets;
//   CreateBlocks(n_blocks);
//   std::unordered_map<long, int> final_blocks;
//   RunInParallel(n_threads, [&](int offset) {
//     std::unordered_map<long, int> out_blocks;
//     for (int i = 0; i < n_gets; ++i) {
//       int iter = offset * n_gets + i;
//       free_list.AddBlock(blocks[iter], block_sizes[iter]);
//       out_blocks.insert({(long)free_list.GetBlock(block_sizes[iter]), 0});
//     }
//     std::unique_lock<std::mutex> lock(map_lock);
//     final_blocks.insert(out_blocks.begin(), out_blocks.end());
//   });
//   ASSERT_THAT(final_blocks.size(), Eq(n_blocks));
// }

}  // namespace quasi_game_engine

// tests
// Threaded test for AddBlock
// Threaded test for GetBlock
// Merge neighbouring free blocks in page