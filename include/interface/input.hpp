#ifndef QGE_INPUT_HPP
#define QGE_INPUT_HPP

#include "utils/jsonFileManager/jsonFileManager.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <map>


namespace qge {

   // KEY_MAX must always be last and equal to the penultimate KEY
   // Never set a value to the KEYS as will break functionlity
   enum ActionEnum {
      KEY_UP,
      KEY_DOWN,
      KEY_RIGHT,
      KEY_LEFT,
      KEY_ESC,
      KEY_MAX = KEY_ESC
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


   class Input {
      /*Notes----------------------------------------------------------------
      - May need to handle memory of keys pressed?
      - There may be a bug if a key is not set in the file and then someone
      tries to access that key...
      ----------------------------------------------------------------------*/
   public:
      Input(Logger *loggerIn);
      virtual ~Input() {};
      virtual void update() = 0;

      void loadKeys(const char *configFileNameIn);
      void reloadKeys();

      bool isKeyActive(ActionEnum actionIn);

      void setKey(ActionEnum actionIn, uint keyValueIn, bool isHoldIn, std::string nameIn = "");
      void setKey(uint actionIn, uint keyValueIn, bool isHoldIn, std::string nameIn = "");
      const Key *getKey(ActionEnum actionIn);
      const Key *getKey(std::string nameIn);
      const double* getMouseMovement() { return diffPos; };

   protected:
      long keyState, holdKeys, activeKeys;
      std::map<ActionEnum, Key> keys;
      double cursor[2], diffPos[2];

      const char *setInput;

      JsonFileManager *jsonFileManager;
      Logger *logger;

      virtual bool validKey(uint keyValueIn) = 0;
      void parseKeys(JsonFile* configFile);

      void setBitOrder();

   };

} // namespave qge


#endif // QGE_INPUT_HPP
