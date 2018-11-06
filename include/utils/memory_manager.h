#ifndef QGE_MEMORY_MANAGER_H
#define QGE_MEMORY_MANAGER_H

namespace quasi_game_engine {
/* Notes -------------------------------------------------
   - Maybe have a seperate class for the block itself
   - Probably then need a class for single/double stack and pool based
allocators
   - Need to figure out a way to have single and double buffer
   - allignment function
   - defragmentation for each of the allocators (and heep memory?)
--------------------------------------------------------*/
class MemoryManager {
 public:
  MemoryManager();
  ~MemoryManager();

  // create block
  // delete block

 protected:
  // total assigned memory
  // vector of assigned blocks
};

}  // namespace quasi_game_engine

#endif  // QGE_MEMORY_MANAGER_H
