#include "first_app.hpp"

namespace cmbst
{

  void FirstApp::run()
  {

    while (!cmbstWindow.shouldClose())
    {
      glfwPollEvents();
    }

  }

}
