#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include <map>
#include <vector>
#include "parameterconfigurator.h"
#include "model/image.h"

class ImageController : public IParameterConfigurator
{
    private:
        inline static std::vector<Image> m_images;

    public:
        ImageController();
        std::vector<Image> getParameter();
        bool setParameter(std::vector<std::string> imageFilePaths);
        void resizeAll(int width, int height);
        void removeImage(std::string imagePath);
};

#endif // IMAGECONTROLLER_H
