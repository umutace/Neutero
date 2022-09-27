#include "imagecontroller.h"
#include <algorithm>

ImageController::ImageController()
{

}

std::vector<Image> ImageController::getParameter()
{
    return ImageController::m_images;
}

//Adds new images on top of the existing ones
bool ImageController::setParameter(std::vector<std::string> imageFilePaths)
{
    for (std::string filePath : imageFilePaths)
    {
        Image currentImage(filePath);
        m_images.push_back(currentImage);
    }
    if(this->m_images.size()>0) {
        this->ready = true;
        InferenceManager::getInstance()->checkIfRunnable();
    }
    return true;
}

void ImageController::resizeAll(int width, int height)
{
    for (Image image : m_images)
    {
        image.resize(width, height);
    }
}

void ImageController::removeImage(std::string imagePath)
{
    for (auto it = m_images.begin(); it != m_images.end(); it++)
    {
        if (it->getFilePath() == imagePath)
        {
            m_images.erase(it);
            break;
        }
    }
    if (this->m_images.size() == 0) {
        this->ready = false;
        InferenceManager::getInstance()->checkIfRunnable();
    }
}





