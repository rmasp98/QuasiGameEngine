#ifndef XXX_INPUT_HPP
#define XXX_INPUT_HPP

#include "utils/jsonFileManager/jsonFileManager.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <map>


namespace xxx {

   // KEY_MAX must always be last and equal to the penultimate KEY
   // Never set a value to the KEYS as will break functionlity
   enum ActionEnum {
      KEY_UP,
      KEY_DOWN,
      KEY_RIGHT,
      KEY_LEFT,
      KEY_MAX = KEY_LEFT
   };

   struct Key {
      Key(const char* nameIn, uint valIn) : name(nameIn), keyValue(valIn) {};
      Key() {};
      std::string name;
      uint keyValue;
      long bitLocation;
      bool isHold;
   };

   class logger;
   class LogWorker;




   class Input {
      /*Notes----------------------------------------------------------------
      - May need to handle memory of keys pressed?
      ----------------------------------------------------------------------*/
   public:
      virtual ~Input() {};
      virtual void update() = 0;

      virtual bool init(const char* configFileNameIn) = 0;

      virtual void swapBuffers() = 0;
      virtual void pollEvents() = 0;
      virtual bool isWindowOpen() = 0;

      void loadKeys(const char* configFileNameIn);
      void reloadKeys();

      bool isActive(ActionEnum actionIn);

      void setKey(ActionEnum actionIn, uint keyValueIn, bool isHoldIn, std::string nameIn = "");
      void setKey(uint actionIn, uint keyValueIn, bool isHoldIn, std::string nameIn = "");
      const Key* getKey(ActionEnum actionIn);
      const Key* getKey(std::string nameIn);


      double cursor[2];
      double lastPos[2];
      double winCen[2];
   protected:
      Input(LogWorker* logWorkerIn);
      long keyState, oldKeyState, holdKeys, activeKeys;
      std::map<ActionEnum, Key> keys;

      const char* setInput;

      JsonFileManager* jsonFileManager;
      Logger* logger;

      virtual bool validKey(uint keyValueIn) = 0;
      void parseKeys(JsonFile* configFile);

   };

}


#endif // XXX_INPUT_HPP
