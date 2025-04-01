#pragma once

#include "cmbst_device.hpp"


// std
#include <string>
#include <vector>

namespace cmbst
{
  struct PipelineConfigInfo
  {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
  };

  class CmbstPipeline
  {
    public:
      CmbstPipeline(CmbstDevice &device, const std::string& verFilePath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

      ~CmbstPipeline() {}

      CmbstPipeline(const CmbstPipeline&) = delete;
      void operator=(const CmbstPipeline&) = delete;

      static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
      
    private:
      static std::vector<char> readFile(const std::string& filepath);

      void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

      void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

      CmbstDevice& cmbstDevice;
      VkPipeline graphicsPipeline;
      VkShaderModule vertShaderModule;
      VkShaderModule fragShaderModule;
  };
} // namespsace cmbst
