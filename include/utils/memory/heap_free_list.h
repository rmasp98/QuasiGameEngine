/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_HEAP_FREE_LIST_H
#define QGE_HEAP_FREE_LIST_H

#include <mutex>

namespace quasi_game_engine {

struct FreeHeader {
  FreeHeader* next;
  // FreeHeader* previous;
  int size;
  bool is_locked;
  // std::mutex lock;
};

class HeapFreeList {
 public:
  HeapFreeList() : free_list_(nullptr) {}
  ~HeapFreeList() = default;

  void AddBlock(FreeHeader* new_block, int size);
  void* GetBlock(int size);

 private:
  FreeHeader* free_list_;
  std::mutex mutex;

  void InsertFreeBlock(FreeHeader* new_block, FreeHeader* previous_block,
                       FreeHeader* next_block);
  void RemoveFreeBlock(FreeHeader* block, FreeHeader* previous_block);
  bool IsLocked(FreeHeader* block);
};

}  // namespace quasi_game_engine

#endif  // QGE_HEAP_FREE_LIST_H
