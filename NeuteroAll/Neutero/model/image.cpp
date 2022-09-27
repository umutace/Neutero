#include "image.h"


Image::Image(const std::string filePath)
{
    try {
        this->image = cv::imread(filePath, cv::IMREAD_COLOR);
    }
    catch (const std::exception& e)
    {
        std::string errorMes;
        errorMes = (std::string) e.what() + "\nImage cannot be read.";
        throw std::runtime_error(errorMes);
    }

    this->m_filePath = filePath;
}

//resizes the image to the specified width and height
void Image::resize(int width, int height)
{
    //    int imageWidth = this->image.cols;
    //    int imageHeight = this->image.rows;
    cv::Mat resized;
    cv::resize(this->image,resized, cv::Size(width, height), cv::INTER_LINEAR);

                                                                                // INTER_LINEAR Resizing seemed like the right choice
    this->image = resized;                                                      // but a more advanced implementation can reduce the confidence-loss caused by resizing
}

std::string Image::getFilePath() const 
{
    return m_filePath;
}

std::pair<int,int> Image::getSize() const
{
    std::pair<int,int> imageSize=std::make_pair(this->image.cols, this->image.rows);
    return imageSize;
}

cv::Mat Image::getImage()
{
    return this->image;
}

