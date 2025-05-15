#include "first_app.hpp"

// std
#include <stdexcept>
#include <array>


namespace cmbst
{
  FirstApp::FirstApp()
  {
    loadModels();
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
  }

  FirstApp::~FirstApp()
  {
    vkDestroyPipelineLayout(cmbstDevice.device(), pipelineLayout, nullptr);
  }

  void FirstApp::loadModels()
  {
    std::vector<CmbstModel::Vertex> vertices
    {
      {{0.0f, -0.5f}},
      {{0.5f, 0.5f}},
      {{-0.5f, 0.5f}}
    };
    cmbstModel = std::make_unique<CmbstModel>(cmbstDevice, vertices);
  }

  void FirstApp::run()
  {

    while (!cmbstWindow.shouldClose())
    {
      glfwPollEvents();
      drawFrame();
    }
  
    vkDeviceWaitIdle(cmbstDevice.device());
  }

  void FirstApp::createPipelineLayout()
  {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(
          cmbstDevice.device(),
          &pipelineLayoutInfo,
          nullptr,
          &pipelineLayout) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create pipeline layout");
    }
  }
  
  void FirstApp::createPipeline()
  {
    auto pipelineConfig = CmbstPipeline::defaultPipelineConfigInfo(
        cmbstSwapChain.width(),
        cmbstSwapChain.height());
    pipelineConfig.renderPass = cmbstSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    cmbstPipeline = std::make_unique<CmbstPipeline>(
        cmbstDevice,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig);
  }

  void FirstApp::createCommandBuffers()
  {
    commandBuffers.resize(cmbstSwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = cmbstDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if(vkAllocateCommandBuffers(cmbstDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to allocate command buffers!");
    }
    for(int i = 0; i < commandBuffers.size(); i++)
    {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
      {
        throw std::runtime_error("failed to begin recording command buffer!");
      }

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = cmbstSwapChain.getRenderPass();
      renderPassInfo.framebuffer = cmbstSwapChain.getFrameBuffer(i);

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = cmbstSwapChain.getSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
      clearValues[1].depthStencil = {1.0f, 0};
      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();
      
      vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      cmbstPipeline->bind(commandBuffers[i]);
      cmbstModel->bind(commandBuffers[i]);
      cmbstModel->draw(commandBuffers[i]);

      vkCmdEndRenderPass(commandBuffers[i]);
      if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
      {
        throw std::runtime_error("failed to record command buffer!");
      }


    }
  }
  void FirstApp::drawFrame()
  {
    uint32_t imageIndex;
    auto result = cmbstSwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
      throw std::runtime_error("failed to acquire swap chain image!");
    }

    result = cmbstSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if(result != VK_SUCCESS)
    {
      throw std::runtime_error("failed to present swap chain image!");
    }
  }

} // namespace cmbst
