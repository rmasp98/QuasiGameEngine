/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/memory/heap_block_list.h"

#include <iostream>

namespace quasi_game_engine {

void HeapBlockList::AddBlock(BlockHeader* new_block, int block_size,
                             bool new_page) {
  if (new_page) {
    new_block->isFree = true;
    new_block->next = block_list_;
    block_list_ = new_block;
  } else {
    new_block->isFree = true;
    BlockHeader* block = block_list_;
    while (block != nullptr) {
      if (block->next == new_block->next) {
        block->next = new_block;
      }
      block = block->next;
    }
  }

  free_list_.AddBlock(reinterpret_cast<FreeHeader*>(new_block + 1),
                      block_size - sizeof(BlockHeader));
}

// TODO: figure out optimum allocation scheme (this one os shit)
void* HeapBlockList::GetFreeBlock(int size) {
  return free_list_.GetBlock(size);
}

void HeapBlockList::MergeFreeBlocks(void* page, int page_size) {
  (void)page;
  (void)page_size;
  // BlockHeader* block = block_list_;
  // while (block != nullptr && block->next != nullptr) {
  //   if (block->isFree && block->next->isFree) {
  //     FreeHeader* main_block =
  //     reinterpret_cast<FreeHeader*>(block->GetData()); FreeHeader*
  //     merge_block =
  //         reinterpret_cast<FreeHeader*>(block->next->GetData());
  //     main_block->size += merge_block->size + sizeof(BlockHeader);
  //     RemoveFreeBlock(merge_block);

  //     block->next = block->next->next;
  //   }
  //   block = block->next;
  // }
}

// This should check if bigger the min_block_size, if so split
void HeapBlockList::SplitBlock(BlockHeader* block) { (void)block; }

}  // namespace quasi_game_engine
