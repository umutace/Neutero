#include "diagnosticdata.h"

std::string DiagnosticData::providerData() const
{
    return m_providerData;
}

void DiagnosticData::setProviderData(std::string providerData)
{
    m_providerData = providerData;
}

DiagnosticData::DiagnosticData()
{

}

void DiagnosticData::setHardwareData(std::string hardwareData)
{
    m_hardwareData = hardwareData;
}

void DiagnosticData::setNumberOfImages(int numberOfImages)
{
    m_numberOfImages = numberOfImages;
}

double DiagnosticData::elapsedTime() const
{
    return m_elapsedTime;
}

void DiagnosticData::setElapsedTime(double elapsedTime)
{
    m_elapsedTime = elapsedTime;
}

int DiagnosticData::numberOfImages() const
{
    return m_numberOfImages;
}

std::string DiagnosticData::hardwareData() const
{
    return m_hardwareData;
}

// Return all information as a single string for viewing
std::string DiagnosticData::toString() const
{
    std::string result = "";
    result += "Processed number of images: " + std::to_string(m_numberOfImages) + "\n";
    result += "Hardware data: " + m_hardwareData + "\n";
    result += "Total elapsed time for execution: " + std::to_string(m_elapsedTime) + " seconds" + "\n";
    return result;
}
