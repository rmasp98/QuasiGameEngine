#include "interface/deviceInterface.hpp"

#include "utils/logging/logger.hpp"

#include <math.h>


#include "interface/glfw/glfwKeyManager.hpp"


namespace xxx {

   DeviceInterface::DeviceInterface(LogWorker* logWorkerIn) {
      logger = new Logger("[ Interface  Manager ]", "logs/DeviceInterfaceManager.log", logWorkerIn);
      jsonFileManager = new JsonFileManager(logger);
   }
}
