#pragma once


#include "cmbst_window.hpp"
#include "cmbst_pipeline.hpp"
#include "cmbst_device.hpp"
#include "cmbst_swap_chain.hpp"

// std
#include <memory>
#include <vector>


namespace cmbst 
{
  class FirstApp
  {
    public:
      static constexpr int WIDTH  = 800;
      static constexpr int HEIGHT = 600;
      
      FirstApp();
      ~FirstApp();

      FirstApp(const CmbstWindow &) = delete;
      FirstApp &operator=(const CmbstWindow &) = delete;

      void run();
    
    private:
      void createPipelineLayout();
      void createPipeline();
      void createCommandBuffers();
      void drawFrame();

      CmbstWindow cmbstWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
      CmbstDevice cmbstDevice {cmbstWindow};
      CmbstSwapChain cmbstSwapChain{cmbstDevice, cmbstWindow.getExtent()};
      std::unique_ptr<CmbstPipeline> cmbstPipeline;
      VkPipelineLayout pipelineLayout;
      std::vector<VkCommandBuffer> commandBuffers;
  };

} // namespace cmbst
