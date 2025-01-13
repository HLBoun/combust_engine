#pragma once


// std
#include <string>
#include <vector>

namespace cmbst
{
  class CmbstPipeline
  {
    public:
      CmbstPipeline(const std::string& verFilePath, const std::string& fragFilepath);
      
    private:
      static std::vector<char> readFile(const std::string& filepath);

      void createGraphicsPipeline(const std::string& verFilePath, const std::string& fragFilepath);
  };
}
