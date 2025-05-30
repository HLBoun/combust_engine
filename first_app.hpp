#pragma once


#include "cmbst_window.hpp"
#include "cmbst_pipeline.hpp"
#include "cmbst_device.hpp"
#include "cmbst_swap_chain.hpp"
#include "cmbst_model.hpp"

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
      void loadModels();
      void createPipelineLayout();
      void createPipeline();
      void createCommandBuffers();
      void freeCommandBuffers();
      void drawFrame();
      void recreateSwapChain();
      void recordCommandBuffer(int imageIndex);

      CmbstWindow cmbstWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
      CmbstDevice cmbstDevice {cmbstWindow};
      std::unique_ptr<CmbstSwapChain> cmbstSwapChain;
      std::unique_ptr<CmbstPipeline> cmbstPipeline;
      VkPipelineLayout pipelineLayout;
      std::vector<VkCommandBuffer> commandBuffers;
      std::unique_ptr<CmbstModel> cmbstModel;
  };

} // namespace cmbst
