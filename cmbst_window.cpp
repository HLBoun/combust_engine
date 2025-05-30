#include "cmbst_window.hpp"

#include <stdexcept>
namespace cmbst
{
  
  // Constructor
  CmbstWindow::CmbstWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
  {
    initWindow();   
  }


  // Destructor
  CmbstWindow::~CmbstWindow()
  {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  // Initializer
  void CmbstWindow::initWindow()
  {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

  }
  
  void CmbstWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
  {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create window surface");
    }
  }

  void CmbstWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height)
  {
    auto cmbstWindow = reinterpret_cast<CmbstWindow *>(glfwGetWindowUserPointer(window));
    cmbstWindow->framebufferResized = true;
    cmbstWindow->width = width;
    cmbstWindow->height = height;
  }

} // namespace cmbst
