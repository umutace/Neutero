#pragma once
#include <string>
#include <vector>
#include "singleimageinferenceresult.h"
#include <memory>

class InferenceResult 
{
    private:
        std::vector<SingleImageInferenceResult> m_results;
        double m_elapsedTime;
    public: 
        InferenceResult();
        std::string toString(bool sortedByConfidence=true) const;
        std::vector<SingleImageInferenceResult> results() const;
        void setResults(std::vector<SingleImageInferenceResult> results);
        double elapsedTime() const;
        void setElapsedTime(double elapsedTime);
        void setLabels(std::string filePath);
};
