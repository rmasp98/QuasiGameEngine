#include "main/main.hpp"
#include "resource/resourceManager.hpp"
#include "renderer/renderManager.hpp"
#include "resource/texture.hpp"
#include "interface/deviceInterface.hpp"
#include "interface/glfw/glfwInterface.hpp" //this isn't great...
#include "utils/logging/logger.hpp"
#include "utils/logging/logWorker.hpp"

#include "utils/shader.hpp"




using namespace xxx;

int main() {

   std::cout << "It has begun!" << std::endl;

   LogWorker* logWorker = new LogWorker();
   logWorker->init();

   DeviceInterface* input = new GlfwInterface(logWorker);
   input->init("config/keyMap.cfg");

   //const Key* tempKey = input->getKey(KEY_LEFT);
   //input->setKey(KEY_LEFT, 100, true);

   RenderManager* renderMan = new RenderManager(logWorker);
   renderMan->initGraphics();

   ResourceManager* resMan = new ResourceManager(logWorker);
   Resource* picture = resMan->getResource("assets/Dragon_ground_color.jpg", "");
   if (picture != NULL)
      picture->loadToGraphics(renderMan);

   Resource* object = resMan->getResource("assets/Dragon_2.5.3ds");
   if (object != NULL)
      object->loadToGraphics(renderMan);



   std::vector<std::string> shaders(2, "");
   shaders[0] = "shaders/main.vs";
   shaders[1] = "shaders/main.fs";
   unsigned int pID = LoadShaders(shaders);
   glUseProgram(pID);


   glm::vec3 pos = glm::vec3(0.0f, 0.0f, 20.0f);
   double mouseSpeed = 0.03f, dT = 0.016;
   glm::vec2 angle = glm::vec2(3.14f, 0.0f);

   glViewport(0, 0, 2*1920, 2*1080);
   glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)(1920.0/1080.0), 0.1f, 100.0f);
   glm::mat4 view, model = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
   model = glm::rotate(model, -1.55f, glm::vec3(1.0, 0.0, 0.0));

    do {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear screen
      input->pollEvents(); //Check for key and mouse events

      //update the game

      //renderMan->draw();

      // Draw quad
      if (picture != NULL) {
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, picture->getResource());
      }

      input->update();

      //Determines the angle of the camera based on how far the cursor position has changed
      float* mouseDiff = input->getMousePosDiff();
      angle.x += mouseSpeed * dT * GLfloat(mouseDiff[0]);
      double dTheta = mouseSpeed * dT * GLfloat(mouseDiff[1]);

      //Makes sure that camera can't look up or down too far
      if (angle.y + dTheta <= -0.9 * glm::pi<GLfloat>() / 2.0f)
         angle.y = -0.9 * glm::pi<GLfloat>() / 2.0f;
      else if (angle.y + dTheta > 0.9 * glm::pi<GLfloat>() / 2.0f)
         angle.y = 0.9 * glm::pi<GLfloat>() / 2.0f;
      else
         angle.y += dTheta;

      //Generates a vector for the facing direction from the angles
      glm::vec3 facing = glm::vec3(cos(angle.y) * sin(angle.x),
                             sin(angle.y),
                             cos(angle.y) * cos(angle.x));

      //Creates the movement (2D) vectors (posible should normalise)
      // this is for 2d movement
      // glm::vec3 dir = glm::vec3(facing.x, 0, facing.z);
      glm::vec3 right = glm::vec3(facing.z, 0, -facing.x);

      if (input->isKeyActive(KEY_UP))
         pos += facing * 0.1f;
      if (input->isKeyActive(KEY_DOWN))
         pos -= facing * 0.1f;
      if (input->isKeyActive(KEY_LEFT))
         pos += right * 0.1f;
      if (input->isKeyActive(KEY_RIGHT))
         pos -= right * 0.1f;






      view = glm::lookAt(pos, pos + facing, glm::vec3(0,1,0));
      glm::mat4 mvp = proj * view * model;
      glUniformMatrix4fv(glGetUniformLocation(pID, "MVP"), 1, GL_FALSE, &mvp[0][0]);
      glBindVertexArray(object->getResource());
      glDrawElements(GL_TRIANGLES, 36656*3, GL_UNSIGNED_INT, (void*)0);

      //glBindVertexArray(quadVAO);
      //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      glBindVertexArray(0);

      //Swap second buffer
      input->swapBuffers();

   } while (input->isWindowOpen());


   delete picture;
   delete object;
   delete resMan;
   delete renderMan;

   delete input;

   //this should always be deleted last to ensure all logs are written
   delete logWorker;


}
