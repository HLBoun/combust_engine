#pragma once


#include "cmbst_window.hpp"



namespace cmbst 
{
  class FirstApp
  {
    public:
      static constexpr int WIDTH  = 800;
      static constexpr int HEIGHT = 600;
      
      void run();
    
    private:
      CmbstWindow cmbstWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  };
} // namespace cmbst
