#include "main/main.hpp"
#include "resource/resourceManager.hpp"
#include "renderer/renderManager.hpp"
#include "resource/texture.hpp"
#include "input/input.hpp"
#include "utils/logger.hpp"

#include "utils/shader.hpp"

using namespace xxx;

int main() {

   LogWorker* logWorker = new LogWorker();
   logWorker->init();


   std::cout << "It has begun!" << std::endl;

   RenderManager* renderMan = new RenderManager(logWorker);
   renderMan->initGraphics();

   ResourceManager* resMan = new ResourceManager(logWorker);
   Resource* picture = resMan->getResource("assets/statue.dds", "");
   if (picture != NULL)
      picture->loadToGraphics(renderMan);

   Resource* object = resMan->getResource("assets/Dragon.3ds");
   if (object != NULL)
      object->loadToGraphics(renderMan);

   Input* input = new GLFWInput(renderMan->getWindow(), "config/keyMap.cfg", logWorker);
   input->setKey(KEY_LEFT, 100);




   std::vector<std::string> shaders(2, "");
   shaders[0] = "shaders/main.vs";
   shaders[1] = "shaders/main.fs";
   unsigned int pID = LoadShaders(shaders);
   glUseProgram(pID);


   glViewport(0, 0, 2*1920, 2*1080);
   glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)(1920.0/1080.0), 0.1f, 100.0f);

    do {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear screen
      glfwPollEvents(); //Check for key and mouse events

      //update the game

      //renderMan->draw();

      // Draw quad
      if (picture != NULL) {
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, picture->getResource());
      }

      glUniformMatrix4fv(glGetUniformLocation(pID, "projection"), 1, GL_FALSE, &proj[0][0]);
      glBindVertexArray(object->getResource());
      glDrawElements(GL_TRIANGLES, 36656*3, GL_UNSIGNED_INT, (void*)0);

      //glBindVertexArray(quadVAO);
      //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      glBindVertexArray(0);



      //Swap second buffer
      glfwSwapBuffers(renderMan->getWindow());

   } while (glfwGetKey(renderMan->getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(renderMan->getWindow()) == 0);


   delete picture;
   delete resMan;
   delete renderMan;

   //this should always be deleted last to ensure all logs are written
   delete logWorker;

}
