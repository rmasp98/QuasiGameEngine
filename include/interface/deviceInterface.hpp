#ifndef QGE_DEVICE_INTERFACE_HPP
#define QGE_DEVICE_INTERFACE_HPP


#include "interface/input.hpp"
#include "utils/jsonFileManager/jsonFileManager.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <map>


namespace qge {

   class logger;
   class LogWorker;

   class DeviceInterface {
      /*Notes----------------------------------------------------------------
      - May need to handle memory of keys pressed?
      ----------------------------------------------------------------------*/
   public:
      virtual ~DeviceInterface();
      virtual void update() = 0;

      virtual double getTime() = 0;
      virtual void swapBuffers() = 0;
      virtual void pollEvents() = 0;
      virtual bool isWindowOpen() = 0;

      Input *getInput() { return input; };

   protected:
      DeviceInterface(LogWorker* logWorkerIn);

      Logger *logger;
      Input *input;

   };

}  // namespave qge


#endif // QGE_DEVICE_INTERFACE_HPP
