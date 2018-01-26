#include "main/main.hpp"
#include "resource/resourceManager.hpp"
#include "renderer/renderManager.hpp"
#include "resource/texture.hpp"

#include "utils/shader.hpp"

using namespace xxx;

int main() {

   std::cout << "It has begun!" << std::endl;

   RenderManager* renderMan = new RenderManager();
   renderMan->initGraphics();

   ResourceManager* resMan = new ResourceManager();
   Resource* picture = resMan->getResource(0, "assets/test.dds");
   if (picture != NULL)
      picture->loadToGraphics(renderMan);




   std::vector<std::string> lighting(2, "");
   lighting[0] = "shaders/main.vs";
   lighting[1] = "shaders/main.fs";
   unsigned int lightingPID = LoadShaders(lighting);
   glUseProgram(lightingPID);

   // Pass quad to buffers (this could be linked up with the menu quad)
   float quadVertices[] = {
       // positions        // texture Coords
       -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
       -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
   };
   // setup plane VAO
   unsigned int quadVAO;
   glGenVertexArrays(1, &quadVAO);
   unsigned int quadVBO;
   glGenBuffers(1, &quadVBO);
   glBindVertexArray(quadVAO);
   glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

   glViewport(0, 0, 2*1920, 2*1080);


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

      glBindVertexArray(quadVAO);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      glBindVertexArray(0);



      //Swap second buffer
      glfwSwapBuffers(renderMan->getWindow());

   } while (glfwGetKey(renderMan->getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(renderMan->getWindow()) == 0);


   delete picture;
   delete resMan;
   delete renderMan;

}
