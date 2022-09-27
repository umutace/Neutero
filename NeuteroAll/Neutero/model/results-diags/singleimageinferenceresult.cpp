#include "singleimageinferenceresult.h"
#include <string.h>
#include <fstream>
#include <iostream>

SingleImageInferenceResult::SingleImageInferenceResult
(std::shared_ptr<Image> image, std::map<std::string, std::string> confidences)
{
    this->m_image = image;
    this->m_confidences = confidences;
    this->sortByConfidence();
}

std::string SingleImageInferenceResult::toString(bool sortedByConfidence) const
{
    std::string output = "";
    std::string result = "";
    if (sortedByConfidence)
    {
        for (auto pair : m_sortedConfidences)
        {
            output += pair.first + ": " + pair.second + "\n";
        }
    } else
    {
        for (auto it = m_confidences.cbegin(); it != m_confidences.cend(); it++)
        {
            output += (it->first) + ": " + (it->second) + "\n";
        }
    }
    result = output.substr(0, output.size() - 2 );
    return ("Result for" + m_image->getFilePath() + "\n" + result);
}

std::map<std::string, std::string> SingleImageInferenceResult::getConfidences() const
{
    return m_confidences;
}

std::vector<std::pair<std::string, std::string> > SingleImageInferenceResult::getSortedConfidences() const
{
    return m_sortedConfidences;
}

std::shared_ptr<Image> SingleImageInferenceResult::getImage()
{
    return (this->m_image);
}

//set labels according to the txt file given as path argument
void SingleImageInferenceResult::setLabels(std::string filePath)
{
    std::ifstream ins(filePath);
    if (!ins.is_open()) throw std::runtime_error("\nLabels file cannot be opened.");

    std::vector<std::string> labels;
    std::string line;

    while (std::getline(ins, line)) labels.push_back(line);

    ins.close();

    std::map<std::string, std::string> confidences;

    int i = 0;
    for(auto it = m_confidences.cbegin(); it != m_confidences.cend(); it++)
    {
        if (i >= labels.size())
        {
            confidences[it->first] = it->second;
            continue;
        }
        confidences[labels[i]] = it->second;
        i++;
    }
    m_confidences = confidences;
    this->sortByConfidence();
}

void SingleImageInferenceResult::sortByConfidence()
{
    std::vector<std::pair<std::string, std::string>> pairs;
    for (auto itr = m_confidences.begin(); itr != m_confidences.end(); ++itr)
        pairs.push_back(*itr);

    sort(pairs.begin(), pairs.end(), [=](std::pair<std::string, std::string>& a, std::pair<std::string, std::string>& b)
    {
        return a.second > b.second;
    }
    );
    m_sortedConfidences = pairs;
}


