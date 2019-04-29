/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_ALLOCATOR_H
#define QGE_ALLOCATOR_H

namespace quasi_game_engine {

////////////////////////////////////////////////////////////
class MemoryManager {
 public:
  MemoryManager() = default;
  virtual ~MemoryManager() = default;

  virtual void* GetFreeBlock(int size) = 0;
  virtual void FreeBlock(void* pointer) = 0;
}

////////////////////////////////////////////////////////////

struct BlockHeader {
  BlockHeader* next;
  bool isFree;
};

struct FreeHeader {
  FreeHeader* next;
  FreeHeader* previous;
  size_t size;
};

class BlockList {
 public:
  explicit BlockList(int min_block_size) : min_block_size_(min_block_size) {}
  ~BlockList() = default;

  void AddFreeBlock(void* block_pointer);
  // Throw if size bigger than min_block_size
  BlockHeader* FindFreeBlock(int size);
  int GetAssignedMemory();

 private:
  int min_block_size_;
  BlockHeader* blocks_;
  FreeHeader* free_list_;

  void* GetDataPointer(BlockHeader* block) { return (block + 1); }
  BlockHeader* GetBlockHeader(FreeHeader* free_block) {
    return reinterpret_cast<BlockHeader*>(free_block - sizeof(BlockHeader));
  }
}

///////////////////////////////////////////////////////////////

struct PageHeader {
  PageHeader* next;
};

class PageList {
 public:
  PageList(int page_size, int min_pages, BlockList* block_list)
      : page_size_(page_size), min_pages_(min_pages), total_memory_(0) {
    for (int i_pages = 0; i_pages < min_pages_; ++i_pages) {
      CreateNewPage(block_list);
    }
  }
  ~PageList() = default;

  void CreateNewPage(BlockList* block_list);
  int GetTotalMemory();

 private:
  int page_size_;
  int min_pages_;
  int total_memory_;
  PageHeader* pages_;

  // Need to figure out if the +1 still works when not this
  BlockHeader* GetBlocks(PageHeader* page) {
    return reinterpret_cast<BlockHeader*>(page + 1);
  }
}

/////////////////////////////////////////////////////////////////

// Probably have block processor in here
class HeapManager : public MemoryManager {
 public:
  HeapManager(int page_size, int min_block_size, int min_pages)
      : block_list_(min_block_size),
        page_list_(page_size, min_pages, block_list_);
  ~HeapManager() = default;

  void* GetFreeBlock(int size) {
    BlockHeader* freeblock = block_list_.FindFreeBlock(size);
    if (freeblock == nullptr) {
      PageList.CreateNewPage();
      freeblock = block_list_.FindFreeBlock(size);
    }
    return Split(freeblock, size);
  }
  void FreeBlock(void* pointer);

 private:
  BlockList block_list_;
  PageList page_list_;

  void* Split(BlockHeader block, int size);
  void BlockProcessingLoop();

}

////////////////////////////////////////////////////////////////

class Allocator {
 public:
  Allocator(MemoryManager* manager) : manager_(manager){};
  ~Allocator() { delete manager_; };

  // Potentially return a smart pointer
  template <class T>
  T* Allocate(T data) {
    T* block = manager_->GetFreeBlock(sizeof(T));
    *block = data;
    return block;
  }

  template <class T>
  void Deallocate(T* pointer) {
    manager_->FreeBlock(pointer);
  }

 private:
  MemoryManager* manager_;
};

}  // namespace quasi_game_engine

#endif  // QGE_ALLOCATOR_H
