/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_MEMORY_MANAGER_H
#define QGE_MEMORY_MANAGER_H

namespace quasi_game_engine {

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
class MemoryManager {
 public:
  MemoryManager() = default;
  ~MemoryManager() = default;

  // Getting rid of copy/move constructors/assignment operators (may need later)
  MemoryManager(const MemoryManager&) = delete;
  MemoryManager& operator=(const MemoryManager&) = delete;
  MemoryManager(MemoryManager&&) = delete;
  MemoryManager& operator=(MemoryManager&&) = delete;

  // create block
  // delete block

 protected:
  // total assigned memory
  // vector of assigned blocks
};

}  // namespace quasi_game_engine

#endif  // QGE_MEMORY_MANAGER_H
