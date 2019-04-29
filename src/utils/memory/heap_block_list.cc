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

  FreeHeader* free_block = reinterpret_cast<FreeHeader*>(new_block + 1);
  free_block->size = block_size - sizeof(BlockHeader);
  AddFreeBlock(free_block);
}

// TODO: figure out optimum allocation scheme (this one os shit)
void* HeapBlockList::GetFreeBlock(int size) {
  if (size <= 0) return nullptr;
  FreeHeader* return_block = free_list_;
  while (return_block != nullptr) {
    if (size + min_block_size_ <= return_block->size) {
      RemoveFreeBlock(return_block);
      return return_block;
    }
    return_block = return_block->next;
  }
  return nullptr;
}

void HeapBlockList::MergeFreeBlocks(void* page, int page_size) {
  (void)page;
  (void)page_size;
  BlockHeader* block = block_list_;
  while (block != nullptr && block->next != nullptr) {
    if (block->isFree && block->next->isFree) {
      FreeHeader* main_block = reinterpret_cast<FreeHeader*>(block->GetData());
      FreeHeader* merge_block =
          reinterpret_cast<FreeHeader*>(block->next->GetData());
      main_block->size += merge_block->size + sizeof(BlockHeader);
      RemoveFreeBlock(merge_block);

      block->next = block->next->next;
    }
    block = block->next;
  }
}

void HeapBlockList::RemoveFreeBlock(FreeHeader* block) {
  if (block->previous != nullptr)
    block->previous->next = block->next;
  else
    free_list_ = block->next;

  if (block->next != nullptr) block->next->previous = block->previous;
}

void HeapBlockList::AddFreeBlock(FreeHeader* new_block) {
  if (free_list_ == nullptr) {
    InsertFreeBlock(new_block, nullptr, nullptr);
  } else {
    FreeHeader* block = free_list_;
    while (block != nullptr) {
      if (block->size > new_block->size) {
        InsertFreeBlock(new_block, block->previous, block);
        break;
      } else if (block->next == nullptr) {
        InsertFreeBlock(new_block, block, nullptr);
        break;
      }

      block = block->next;
    }
  }
}

void HeapBlockList::InsertFreeBlock(FreeHeader* new_block,
                                    FreeHeader* previous_block,
                                    FreeHeader* next_block) {
  new_block->next = next_block;
  new_block->previous = previous_block;

  if (previous_block != nullptr)
    previous_block->next = new_block;
  else  // If preivous is null then we at the beginning of the list
    free_list_ = new_block;
  if (next_block != nullptr) next_block->previous = new_block;
}

}  // namespace quasi_game_engine
