#ifndef QGE_UI_TEST_H
#define QGE_UI_TEST_H


#include "utils/logging/logger.h"

#include <GLFW/glfw3.h>


namespace quasi_game_engine {
   class Logger;
   class LogWorker;
   class Renderer;

   class UiTest {
   /* NOTES --------------------------------------------------------------------

   ---------------------------------------------------------------------------*/
   public:
      UiTest(LogWorker *log_worker, GLFWwindow *window, Renderer* render_manager);
      ~UiTest();

      void Update();


   protected:
      Logger logger_;
      bool show_demo_window_;

      // this is temporary so have not made it shared_ptr
      GLFWwindow *window_;

   };

} // namespace quasi_game_engine


#endif // QGE_UI_TEST_H
