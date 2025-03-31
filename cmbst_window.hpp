#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <string>
namespace cmbst
{
  class CmbstWindow
  {
    public:
      CmbstWindow(int w, int h, std::string name);
      ~CmbstWindow();
    private:

      void initWindow();

      const int width;
      const int height;

      std::string windowName;
      GLFWwindow *window;
  };
} // namespace cmbst
