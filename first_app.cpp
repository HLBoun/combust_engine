#include "first_app.hpp"

// std
#include <stdexcept>
#include <cassert>
#include <array>


namespace cmbst
{
  FirstApp::FirstApp()
  {
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
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
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
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

  void FirstApp::recreateSwapChain()
  {
      auto extent = cmbstWindow.getExtent();
      while (extent.width == 0 || extent.height == 0)
      {
          extent = cmbstWindow.getExtent();
          glfwWaitEvents();
      }
      vkDeviceWaitIdle(cmbstDevice.device());

      if (cmbstSwapChain == nullptr)
      {
          cmbstSwapChain = std::make_unique<CmbstSwapChain>(cmbstDevice, extent);
      }
      else
      {
          cmbstSwapChain = std::make_unique<CmbstSwapChain>(cmbstDevice, extent, std::move(cmbstSwapChain));

          if (cmbstSwapChain->imageCount() != commandBuffers.size())
          {
              freeCommandBuffers();
              createCommandBuffers();
          }
      }
      createPipeline();
  }
  
  void FirstApp::createPipeline()
  {
      assert(cmbstSwapChain != nullptr && "Cannot Create pipeline before swap chain");
      assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

      PipelineConfigInfo pipelineConfig{};
      CmbstPipeline::defaultPipelineConfigInfo(pipelineConfig);
      pipelineConfig.renderPass = cmbstSwapChain->getRenderPass();
      pipelineConfig.pipelineLayout = pipelineLayout;
      cmbstPipeline = std::make_unique<CmbstPipeline>
          (
              cmbstDevice,
              "shaders/simple_shader.vert.spv",
              "shaders/simple_shader.frag.spv",
              pipelineConfig
          );
  }


  void FirstApp::createCommandBuffers()
  {
    commandBuffers.resize(cmbstSwapChain->imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = cmbstDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if(vkAllocateCommandBuffers(cmbstDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to allocate command buffers!");
    }
  }

  void FirstApp::freeCommandBuffers()
  {
      vkFreeCommandBuffers
      (
          cmbstDevice.device(),
          cmbstDevice.getCommandPool(),
          static_cast<uint32_t>(commandBuffers.size()),
          commandBuffers.data()
      );
      commandBuffers.clear();
  }

  void FirstApp::recordCommandBuffer(int imageIndex)
  {
    VkCommandBufferBeginInfo beginInfo{};

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = cmbstSwapChain->getRenderPass();
    renderPassInfo.framebuffer = cmbstSwapChain->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = cmbstSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    
    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(cmbstSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(cmbstSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{ {0, 0}, cmbstSwapChain->getSwapChainExtent() };
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

    cmbstPipeline->bind(commandBuffers[imageIndex]);
    cmbstModel->bind(commandBuffers[imageIndex]);
    cmbstModel->draw(commandBuffers[imageIndex]);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to record command buffer!");
    }
  }

  void FirstApp::drawFrame()
  {
    uint32_t imageIndex;
    auto result = cmbstSwapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
      recreateSwapChain();
      return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
      throw std::runtime_error("failed to acquire swap chain image!");
    }
    
    recordCommandBuffer(imageIndex);
    result = cmbstSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || cmbstWindow.wasWindowResized())
    {
      cmbstWindow.resetWindowResizedFlag();
      recreateSwapChain();
      return;
    }
    if(result != VK_SUCCESS)
    {
      throw std::runtime_error("failed to present swap chain image!");
    }
  }

} // namespace cmbst
