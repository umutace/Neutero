#pragma once
#include <string>

class DiagnosticData
{
    private:
        std::string m_hardwareData;
        std::string m_providerData;
        int m_numberOfImages;
        double m_elapsedTime;
    public:
        DiagnosticData();
        void setHardwareData(std::string hardwareData);
        void setNumberOfImages(int numberOfImages);
        void setElapsedTime(double elapsedTime);
        double elapsedTime() const;
        int numberOfImages() const;
        std::string hardwareData() const;
        std::string toString() const;
        std::string providerData() const;
        void setProviderData(std::string providerData);
};
