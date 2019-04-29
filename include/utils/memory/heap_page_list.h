/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_HEAP_PAGE_LIST_H
#define QGE_HEAP_PAGE_LIST_H

namespace quasi_game_engine {

class HeapPageList {
 public:
  HeapPageList(int page_size) : page_size_(page_size), total_memory(0) {}
  ~HeapPageList() = default;

  void CreatePage() { total_memory = page_size_; }

  void RemovePage();

  int GetTotalMemory() { return total_memory; }
  void MergeFreeBlocksOnPages();

 private:
  int page_size_;
  int total_memory;
};

}  // namespace quasi_game_engine

#endif  // QGE_HEAP_PAGE_LIST_H
