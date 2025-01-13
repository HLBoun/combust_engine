#pragma once


#include "cmbst_window.hpp"
#include "cmbst_pipeline.hpp"
#include "cmbst_device.hpp"


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
      CmbstDevice cmbstDevice {cmbstWindow};
      CmbstPipeline cmbstPipeline {cmbstDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", CmbstPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
  };

} // namespace cmbst
