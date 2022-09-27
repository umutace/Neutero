#pragma once
#include <string>
#include <map>
#include "../image.h"
#include <memory>

class SingleImageInferenceResult
{
    private:
        std::shared_ptr<Image> m_image;
        std::map<std::string, std::string> m_confidences;
        std::vector<std::pair<std::string, std::string>> m_sortedConfidences;
        void sortByConfidence();
    public: 
        SingleImageInferenceResult(std::shared_ptr<Image> image, std::map<std::string, std::string> confidences);
        std::string toString(bool sortedByConfidence=true) const;
        std::map<std::string, std::string> getConfidences() const;
        std::vector<std::pair<std::string, std::string> > getSortedConfidences() const;
        std::shared_ptr<Image> getImage();
        void setLabels(std::string filePath);        
};
