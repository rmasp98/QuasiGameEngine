#ifndef XXX_DEVICE_INTERFACE_HPP
#define XXX_DEVICE_INTERFACE_HPP


#include "interface/keyManager.hpp"
#include "utils/jsonFileManager/jsonFileManager.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <map>


namespace xxx {

   class logger;
   class LogWorker;

   class DeviceInterface {
      /*Notes----------------------------------------------------------------
      - May need to handle memory of keys pressed?
      ----------------------------------------------------------------------*/
   public:
      virtual ~DeviceInterface() {};
      virtual void update() = 0;

      virtual bool init(const char* configFileNameIn) = 0;

      virtual void swapBuffers() = 0;
      virtual void pollEvents() = 0;
      virtual bool isWindowOpen() = 0;


      virtual bool isKeyActive(ActionEnum action) = 0;
      virtual float* getMousePosDiff() = 0;

   protected:
      DeviceInterface(LogWorker* logWorkerIn);

      JsonFileManager* jsonFileManager;
      Logger* logger;

   };

}


#endif // XXX_DEVICE_INTERFACE_HPP
