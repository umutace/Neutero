#include "inferenceresult.h"

InferenceResult::InferenceResult()
{

}

// Return all information as a single string for viewing
std::string InferenceResult::toString(bool sortedByConfidence) const
{
    std::string result = "";
    for (auto singleResult : m_results)
    {
        result += singleResult.toString(sortedByConfidence) + "\n--------------------------------\n";
    }
    result += "Total elapsed time for inference: " + std::to_string(m_elapsedTime) + " seconds";
    return result;
}

std::vector<SingleImageInferenceResult> InferenceResult::results() const
{
    return m_results;
}

void InferenceResult::setResults(std::vector<SingleImageInferenceResult> results)
{
    m_results = results;
}

double InferenceResult::elapsedTime() const
{
    return m_elapsedTime;
}

void InferenceResult::setElapsedTime(double elapsedTime)
{
    m_elapsedTime = elapsedTime;
}

//this method traverses all singleImageInferenceResul objects and set labels of their label-confidence maps according to the txt file given as a path parameter
void InferenceResult::setLabels(std::string filePath)
{
    std::vector<SingleImageInferenceResult> results;
    for (auto singleResult : m_results)
    {
        singleResult.setLabels(filePath);
        results.push_back(singleResult);
    }
    this->m_results = results;
}
