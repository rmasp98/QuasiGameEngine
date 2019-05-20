/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_HEAP_BLOCK_LIST_H
#define QGE_HEAP_BLOCK_LIST_H

#include "utils/memory/heap_free_list.h"

namespace quasi_game_engine {

struct BlockHeader {
  BlockHeader* next;
  bool isFree;
};

class HeapBlockList {
 public:
  HeapBlockList() = default;
  ~HeapBlockList() = default;

  void AddBlock(BlockHeader* new_block, int block_size, bool new_page);
  void* GetFreeBlock(int size);

 private:
  BlockHeader* block_list_;
  HeapFreeList free_list_;

  void MergeFreeBlocks(void* page, int page_size);

  // This should check if bigger the min_block_size, if so split
  void SplitBlock(BlockHeader* block);
};

}  // namespace quasi_game_engine

#endif  // QGE_HEAP_BLOCK_LIST_H
