#ifndef DEFAULTINFERENCECREATOR_H
#define DEFAULTINFERENCECREATOR_H

#include "inferencecreator.h"
#include "controller/ParameterConfigurator/hwcontroller.h"
#include "controller/ParameterConfigurator/imagecontroller.h"
#include "controller/ParameterConfigurator/nncontroller.h"

class DefaultInferenceCreator : public IInferenceCreator
{
private:
    ImageController* m_imageSource;
    HWController* m_hwSource;
    NNController* m_nnSource;
public:
    DefaultInferenceCreator(ImageController* imgsrc,
                            HWController* hwsrc,
                            NNController* nnsrc);
    std::vector<std::shared_ptr<Inference>> buildInferences();
    ImageController *imageSource() const;
    HWController *hwSource() const;
    NNController *nnSource() const;
    bool isReady();
};

#endif // DEFAULTINFERENCECREATOR_H
