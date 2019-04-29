/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "utils/memory/memory_manager.h"
#include "utils/logging/log_capture.h"

namespace quasi_game_engine {

//////////////////////////////////////////////////////////////////////////////////
// MemoryManager::MemoryManager(int size) {
//   manager_allocator_.CreateBuffer(size);
// }

// StackAllocator* MemoryManager::GetStackAllocator() {
//   if (!stack_allocator_.IsInitialised()) {
//     stack_allocator_.CreateBuffer(10, &manager_allocator_);
//   }
//   return &stack_allocator_;
// }

// DoubleStackAllocator* MemoryManager::GetDoubleStackAllocator() {
//   if (double_stack_allocator_.IsInitialised()) {
//     double_stack_allocator_.CreateBuffer(10, &manager_allocator_);
//   }
//   return &double_stack_allocator_;
// }

// PoolAllocator* MemoryManager::GetPoolAllocator() {
//   if (pool_allocator_.IsInitialised()) {
//     pool_allocator_.CreateBuffer(10, &manager_allocator_);
//   }
//   return &pool_allocator_;
// }

//////////////////////////////////////////////////////////////////////////////////

void StackAllocator::CreateBuffer(int size, HeapAllocator* alloc) {
  // buffer_ = alloc->Allocate(T data)
}

}  // namespace quasi_game_engine
