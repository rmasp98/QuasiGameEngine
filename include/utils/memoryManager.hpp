#ifndef XXX_MEMORY_MANAGER_HPP
#define XXX_MEMORY_MANAGER_HPP

namespace xxx {
   /* Notes -------------------------------------------------
      - Maybe have a seperate class for the block itself
      - Probably then need a class for single/double stack and pool based allocators
      - Need to figure out a way to have single and double buffer
      - allignment function
      - defragmentation for each of the allocators (and heep memory?)
   --------------------------------------------------------*/
   class MemoryManager {
   public:
      MemoryManager();
      ~MemoryManager();

      //create block
      //delete block

   protected:
      //total assigned memory
      //vector of assigned blocks

   };

}


#endif // XXX_MEMORY_MANAGER_HPP
