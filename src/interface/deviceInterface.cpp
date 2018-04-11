#include "interface/deviceInterface.hpp"

#include "utils/logging/logger.hpp"

#include <math.h>



namespace qge {

   DeviceInterface::DeviceInterface(LogWorker* logWorkerIn) {
      logger = new Logger("[Interface  Manager]", "logs/DeviceInterfaceManager.log", logWorkerIn);
   }


   DeviceInterface::~DeviceInterface() {
      delete input;
   }


} // namespave qge
