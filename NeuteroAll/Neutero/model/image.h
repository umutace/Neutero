#pragma once
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


class Image
{
private:
    std::string m_filePath;
    cv::Mat image;

public:
    Image(const std::string filePath);
    void resize(int width, int height);
    std::string getFilePath() const;
    std::pair<int,int> getSize() const;
    cv::Mat getImage();
};
