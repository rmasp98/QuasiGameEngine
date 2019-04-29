/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_MEMORY_MANAGER_H
#define QGE_MEMORY_MANAGER_H

#include "utils/data_types/qge_queue.h"

#include <memory>

namespace quasi_game_engine {

// class DoubleStackAllocator {
//  public:
//   DoubleStackAllocator() = default;
//   ~DoubleStackAllocator() = default;

//   bool IsInitialised() { return buffer_ != nullptr ? true : false; }
//   void CreateBuffer(int size, HeapAllocator* alloc);

//   template <class T>
//   std::shared_ptr<T> Allocate(T data);

//   template <class T>
//   void Deallocate(std::shared_ptr<T>);

//  private:
//   char* buffer_;
//   int size_;
//   int allocated_;
// };

// class PoolAllocator {
//  public:
//   PoolAllocator() = default;
//   ~PoolAllocator() = default;

//   bool IsInitialised() { return buffer_ != nullptr ? true : false; }
//   void CreateBuffer(int size, HeapAllocator* alloc);

//   template <class T>
//   std::shared_ptr<T> Allocate(T data);

//   template <class T>
//   void Deallocate(std::shared_ptr<T>);

//  private:
//   char* buffer_;
//   int size_;
//   int allocated_;
// };

// class GlobalMemoryManager {
//  public:
//   GlobalMemoryManager() = default;
//   ~GlobalMemoryManager() = default;

//   // Getting rid of copy/move constructors/assignment operators (may need
//   later) GlobalMemoryManager(const GlobalMemoryManager&) = delete;
//   GlobalMemoryManager& operator=(const GlobalMemoryManager&) = delete;
//   GlobalMemoryManager(GlobalMemoryManager&&) = delete;
//   GlobalMemoryManager& operator=(GlobalMemoryManager&&) = delete;

//   void Allocate();
//   void Deallocate();

//  private:
//   HeapAllocator* heap_allocator_;
// };

/*------------------------------------------------------------------------------
This class is responsible for retrieving memory from OS and trcking the use of
memory in different subsystems
 Notes
   - Maybe have a seperate class for the block itself
   - Probably then need a class for single/double stack and pool based
allocators
   - Need to figure out a way to have single and double buffer
   - allignment function
   - defragmentation for each of the allocators (and heep memory?)
-----------------------------------------------------------------------------*/
// class MemoryManager {
//  public:
//   explicit MemoryManager(int size);
//   ~MemoryManager() = default;

//   // Getting rid of copy/move constructors/assignment operators (may need
//   later) MemoryManager(const MemoryManager&) = delete; MemoryManager&
//   operator=(const MemoryManager&) = delete; MemoryManager(MemoryManager&&) =
//   delete; MemoryManager& operator=(MemoryManager&&) = delete;

//   StackAllocator* GetStackAllocator();
//   // DoubleStackAllocator* GetDoubleStackAllocator();
//   // PoolAllocator* GetPoolAllocator();

//  private:
//   HeapAllocator manager_allocator_;
//   StackAllocator stack_allocator_;
//   // DoubleStackAllocator double_stack_allocator_;
//   // PoolAllocator pool_allocator_;
// };

}  // namespace quasi_game_engine

#endif  // QGE_MEMORY_MANAGER_H
