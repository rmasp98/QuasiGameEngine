/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/memory/heap_free_list.h"

#include <assert.h>
#include <iostream>

namespace quasi_game_engine {

void HeapFreeList::AddBlock(FreeHeader* new_block, int size) {
  assert((unsigned long)size >= sizeof(FreeHeader));

  new_block->size = size;
  std::unique_lock<std::mutex> lock(mutex);
  if (free_list_ == nullptr) {
    InsertFreeBlock(new_block, nullptr, nullptr);
  } else {
    FreeHeader* block = free_list_;
    FreeHeader* previous_block = nullptr;
    while (block != nullptr) {
      if (block->size >= new_block->size) {
        InsertFreeBlock(new_block, previous_block, block);
        return;
      } else if (block->next == nullptr) {
        InsertFreeBlock(new_block, block, nullptr);
        return;
      }
      previous_block = block;
      block = block->next;
    }
  }
}

void* HeapFreeList::GetBlock(int size) {
  assert(size > 0);
  std::unique_lock<std::mutex> lock(mutex);

  FreeHeader* return_block = free_list_;
  FreeHeader* previous_block = nullptr;
  while (return_block != nullptr) {
    if (size <= return_block->size) {  // check if block is locked
      RemoveFreeBlock(return_block, previous_block);
      return return_block;
    }
    previous_block = return_block;
    return_block = return_block->next;
  }
  return nullptr;
}

void HeapFreeList::RemoveFreeBlock(FreeHeader* block,
                                   FreeHeader* previous_block) {
  if (previous_block != nullptr)
    previous_block->next = block->next;
  else
    free_list_ = block->next;
}

void HeapFreeList::InsertFreeBlock(FreeHeader* new_block,
                                   FreeHeader* previous_block,
                                   FreeHeader* next_block) {
  new_block->next = next_block;
  new_block->is_locked = false;

  // No block level lock needed as we add to list last
  if (previous_block != nullptr)
    previous_block->next = new_block;
  else  // If preivous is null then we at the beginning of the list
    free_list_ = new_block;
}

// bool HeapFreeList::IsLocked(FreeHeader* block) {
//   std::unique_lock<std::mutex> block_lock(temp2);
//   if (!block->is_locked) {
//     block->is_locked = true;
//     return false;
//   }
//   return true;
// }

}  // namespace quasi_game_engine
