/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/memory/heap_allocator.h"
#include "utils/logging/log_capture.h"

namespace quasi_game_engine {

// TODO: start with a number of blocks to prevent
HeapAllocator::HeapAllocator(int page_size, int min_block_size, int min_pages)
    : page_list_(nullptr),
      block_list_(nullptr),
      free_list_(nullptr),
      page_size_(page_size),
      min_block_size_(min_block_size),
      min_pages_(min_pages) {
  // Create several pages so we always have several places to allocate
  for (int i = 0; i < min_pages; ++i) {
    CreateNewPage();
  }
  // TODO: create thread for proccessing blocks
}

HeapAllocator::~HeapAllocator() {
  while (page_list_ != nullptr) {
    PageHeader* next_page = page_list_->next;
    // Double check this will actually free all the memory
    free(page_list_);

    page_list_ = next_page;
  }
}

// This should never run in parallel so could happily put a lock in
void HeapAllocator::CreateNewPage() {
  PageHeader* new_page = reinterpret_cast<PageHeader*>(malloc(page_size_));
  new_page->next = page_list_;
  page_list_ = new_page;

  BlockHeader* new_block = new_page->GetBlocks();
  new_block->next = block_list_;
  new_block->isFree = true;
  block_list_ = new_block;

  FreeHeader* free_block =
      reinterpret_cast<FreeHeader*>(new_block->GetDataPointer());
  free_block->next = free_list_;
  free_block->previous = nullptr;
  // TODO: What is the real size?
  free_block->size = page_size_ - sizeof(PageHeader) - sizeof(BlockHeader);
  // new_block >= biggest block so just shove at front of list
  free_list_ = free_block;

  // maybe do -1 because this should probably be assignable memory
  total_memory_ += free_block->size;
}

// TODO: Think about what happens when two blocks allocate at same time
// 1) both try to go for same block
// 2) both try and create new page
// For both could detect with locks and immediately call FindFreeBlock again
void* HeapAllocator::FindFreeBlock(size_t block_size) {
  if (block_size < page_size_) {
    FreeHeader* next_block = free_list_;
    // Could make this more complex to find best fit
    while (next_block) {
      // smallest_block_size ensures we don't end with too small block_size
      if (next_block->next == nullptr ||
          next_block->next->size < block_size + min_block_size_) {
        return AssignBlock(next_block, block_size);
      }
      next_block = next_block->next;
    }

    // If it gets this far, no space for this sized block
    CreateNewPage();
    return AssignBlock(
        reinterpret_cast<FreeHeader*>(block_list_->GetDataPointer()),
        block_size);
  }
  // If it gets this far, there is no memory or block too big
  throw;
}

void* HeapAllocator::AssignBlock(FreeHeader* block, int block_size) {
  block->GetBlockHeader()->isFree = false;
  block_process_queue_.Push(ProcessBlock(block->GetBlockHeader(), block_size));

  // TODO: Try to think of a better way to do this
  // Remove this block from free_list_
  if (block->next) block->next->previous = block->previous;
  if (block->previous) {
    block->previous->next = block->next;
  } else {
    free_list_ = block->next;
  }

  return block;
}

void HeapAllocator::BlockProcessLoop() {
  while (true) {
    // Need a wait, which can be triggered by an allocate

    // Check queue for any blocks that need processing
    while (block_process_queue_.Size() > 0) {
      ProcessBlock assigned_block = block_process_queue_.Pop();

      // TODO: figure out size
      BlockHeader* new_block = assigned_block.block +
                               assigned_block.assigned_size +
                               sizeof(BlockHeader);
      new_block->isFree = true;
      new_block->next = assigned_block.block->next;
      AddFreeBlock(new_block);

      assigned_block.block->next = new_block;
    }

    // Merge any neighbouring blocks
    BlockHeader* next_block = block_list_;
    bool merge_flag = false;
    // Could add a flag to this to tell it to skip if something in queue
    while (block_process_queue_.Size() == 0 && next_block) {
      if (merge_flag && next_block->isFree) {
        // Check it is in same page
        // Lock
        // Merge
      }
      merge_flag = next_block->isFree;
      next_block = next_block->next;
    }

    // Check if we can release any pages (must check against min_pages_)

    // Check if we can remove page (e.g. if empty and utilisation is not too
    // high)

    // Potentially update metadata and print?

    // Safety checks?
  }
}

// TODO: Think about any locking needed here
void HeapAllocator::AddFreeBlock(BlockHeader* block) {
  FreeHeader* free_block =
      reinterpret_cast<FreeHeader*>(block->GetDataPointer());
  // TODO: need to check this size correctly (subtract the header?)
  free_block->size = block->next - block - sizeof(BlockHeader);

  // TODO: This only works if we go from large to small
  FreeHeader* next_block = free_list_;

  while (next_block) {
    if (next_block->size < free_list_->size) {
      free_block->previous = next_block->previous;
    }
  }

  if (next_block->size < free_block->size) {
    free_block->next = free_list_->next;
    free_block->previous = nullptr;
    free_list_ = free_block;
  } else {
    while (next_block) {
      next_block = next_block->next;
    }
  }
}

}  // namespace quasi_game_engine
