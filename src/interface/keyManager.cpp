#include "interface/keyManager.hpp"

#include "utils/logging/logger.hpp"

#include <math.h>


namespace xxx {

   KeyManager::KeyManager(Logger* loggerIn) : logger(loggerIn) {
      jsonFileManager = new JsonFileManager(logger);

      setInput = "keyboard"; //load from a config file
   }




   void KeyManager::setKey(ActionEnum actionIn, uint keyValueIn, bool isHoldIn,
                      std::string nameIn) {

      if (validKey(keyValueIn)) {
         if (nameIn != "")
            keys[actionIn].name = nameIn;

         keys[actionIn].keyValue = keyValueIn;
         keys[actionIn].isHold = isHoldIn;

         LOG(LOG_TRACE, logger) << "Setting key: Name = " << nameIn <<
                                   "; Key Value = " << keyValueIn <<
                                   "; action = " << actionIn;

      }
   }


   void KeyManager::setKey(uint actionIn, uint keyValueIn, bool isHoldIn,
                      std::string nameIn) {

      //This heavily relies on ActionEnums integrity
      ActionEnum actionConvert;
      if (actionIn <= KEY_MAX) {
         actionConvert = (ActionEnum)actionIn;
         setKey(actionConvert, keyValueIn, isHoldIn, nameIn);
      } else {
         LOG(LOG_ERROR, logger) << "Action value " << actionIn << " does not exist."
                                << " The largest value is " << KEY_MAX;
      }
   }



   const Key* KeyManager::getKey(ActionEnum actionIn) {
      return &keys[actionIn];
   }


   const Key* KeyManager::getKey(std::string nameIn) {
      for (auto const &key : keys) {
         // Need to create a better string comparison
         if (key.second.name == nameIn)
            return &key.second;
      }

      LOG(LOG_ERROR, logger) << nameIn << " key does not exist";
      return NULL;
   }


   void KeyManager::loadKeys(const char* configFileNameIn) {
      JsonFile* configFile = jsonFileManager->loadFile(configFileNameIn);
      parseKeys(configFile);
   }


   void KeyManager::reloadKeys() {
      JsonFile* configFile = jsonFileManager->loadFile();
      parseKeys(configFile);
   }


   void KeyManager::parseKeys(JsonFile* configFile) {

      if (configFile != NULL) {
         //read in json config
         try {
            JsonArrayObject* inputArray = configFile->getKey<JsonArrayObject*>("inputType");

            for (uint iArr=0; iArr<inputArray->getSize(); ++iArr) {
               const char* inputType = inputArray->getKey<const char*>(iArr, "type");
               if (strcmp(inputType, setInput) == 0) { //Keyboard should be in variable
                  JsonArrayObject* keyArray = inputArray->getKey<JsonArrayObject*>(iArr, "keys");
                  for (uint jArr=0; jArr<keyArray->getSize(); ++jArr) {
                     const char* name = keyArray->getKey<const char*>(jArr, "name");
                     uint keyValue = keyArray->getKey<uint>(jArr, "val");
                     uint action = keyArray->getKey<uint>(jArr, "action");
                     bool isHold = keyArray->getKey<bool>(jArr, "hold");

                     setKey(action, keyValue, isHold, name);

                  }
               }
            }

            //set location of key in bit state
            uint it = 0; holdKeys = 0;
            for (auto const &ent1 : keys) {
               //If key should remain active when held down
               if (keys[ent1.first].isHold)
                  holdKeys += pow(2, it);

               // Place where key is in the binary chain based on map order
               keys[ent1.first].bitLocation = pow(2, it++);
            }

         } catch (const char* msg) {
            //Throw a visible error in UI
         }
      }
   }


   //THis is shit
   bool KeyManager::isActive(ActionEnum actionIn) {
      //printf("%ld\n", activeKeys);
      return activeKeys & (int)pow(2, (int)actionIn);
   }
}