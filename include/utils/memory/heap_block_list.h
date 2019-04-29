/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_HEAP_BLOCK_LIST_H
#define QGE_HEAP_BLOCK_LIST_H

namespace quasi_game_engine {

struct BlockHeader {
  BlockHeader* next;
  bool isFree;

  void* GetData() { return (this + 1); }
};

struct FreeHeader {
  FreeHeader* next;
  FreeHeader* previous;
  int size;
};

class HeapBlockList {
 public:
  HeapBlockList(int min_block_size)
      : free_list_(nullptr), min_block_size_(min_block_size) {}
  ~HeapBlockList() = default;

  void AddBlock(BlockHeader* new_block, int block_size, bool new_page = false);
  void* GetFreeBlock(int size);

  // This will be called by the block processor
  void MergeFreeBlocks(void* page, int page_size);

 private:
  FreeHeader* free_list_;
  BlockHeader* block_list_;
  int min_block_size_;

  void RemoveFreeBlock(FreeHeader* block);
  void AddFreeBlock(FreeHeader* new_block);
  void InsertFreeBlock(FreeHeader* new_block, FreeHeader* previous_block,
                       FreeHeader* next_block);
};

}  // namespace quasi_game_engine

#endif  // QGE_HEAP_BLOCK_LIST_H
