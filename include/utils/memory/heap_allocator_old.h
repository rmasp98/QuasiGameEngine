/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_HEAP_ALLOCATOR_H
#define QGE_HEAP_ALLOCATOR_H

#include "utils/data_types/qge_queue.h"

#include <memory>

namespace quasi_game_engine {

struct BlockHeader {
  BlockHeader* next;
  bool isFree;

  // Need to work out how we do this
  void* GetDataPointer(void) { return (this + 1); }
};

struct FreeHeader {
  FreeHeader* next;
  FreeHeader* previous;
  size_t size;
  // Need a mutex too

  BlockHeader* GetBlockHeader(void) {
    return reinterpret_cast<BlockHeader*>(this - sizeof(BlockHeader));
  }
};

struct PageHeader {
  PageHeader* next;

  // helper function that gives the first block
  BlockHeader* GetBlocks(void) {
    return reinterpret_cast<BlockHeader*>(this + 1);
  }
};

struct ProcessBlock {
  ProcessBlock(){};
  ProcessBlock(BlockHeader* block_in, size_t size)
      : block(block_in), assigned_size(size) {}
  BlockHeader* block;
  size_t assigned_size;
};

class HeapAllocator {
 public:
  HeapAllocator(int page_size, int min_block_size, int init_num_blocks);
  ~HeapAllocator();

  // Potentially return a smart pointer
  template <class T>
  T* Allocate(T data) {
    T* block = FindFreeBlock(sizeof(T));
    *block = data;
    return block;
  }

  template <class T>
  void Deallocate(T* pointer) {
    (void)pointer;
    // int size = sizeof(T);
    // BlockHeader* block = reinterpret_cast<BlockHeader*>(pointer);
  }

 private:
  PageHeader* page_list_;
  BlockHeader* block_list_;
  FreeHeader* free_list_;
  size_t page_size_;
  int total_memory_;
  int allocated_memory_;
  int min_block_size_;
  int min_pages_;
  QgeQueue<ProcessBlock> block_process_queue_;

  void* buffer_;
  int size_;
  int allocated_;

  // void Defragment();
  void CreateNewPage();
  void* AssignBlock(FreeHeader* block, int block_size);
  void* FindFreeBlock(size_t block_size);
  void AddFreeBlock(BlockHeader* new_block);
  void BlockProcessLoop();
};

}  // namespace quasi_game_engine

#endif  // QGE_HEAP_ALLOCATOR_H
