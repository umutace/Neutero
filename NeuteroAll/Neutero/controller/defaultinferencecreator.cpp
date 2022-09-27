#include "defaultinferencecreator.h"

ImageController *DefaultInferenceCreator::imageSource() const
{
    return m_imageSource;
}

HWController *DefaultInferenceCreator::hwSource() const
{
    return m_hwSource;
}

NNController *DefaultInferenceCreator::nnSource() const
{
    return m_nnSource;
}

DefaultInferenceCreator::DefaultInferenceCreator(ImageController* imgsrc,
                                                 HWController* hwsrc,
                                                 NNController* nnsrc)
{
    this->m_hwSource = hwsrc;
    this->m_imageSource = imgsrc;
    this->m_nnSource = nnsrc;
}

std::vector<std::shared_ptr<Inference>> DefaultInferenceCreator::buildInferences()
{
    std::vector<std::shared_ptr<Inference>> infs;
    if (this->m_hwSource->isReady() && this->m_imageSource->isReady() && this->m_nnSource->isReady()) {
        for (std::pair<Hardware*, IInferenceProvider*> backendPair : m_hwSource->getParameter()) {
            Inference inference = Inference(backendPair.first, backendPair.second,
                                            m_nnSource->getParameter(), m_imageSource->getParameter());
            auto inf = std::make_shared<Inference>(inference);
            infs.push_back(inf);
        }
    }

    return infs;
}

bool DefaultInferenceCreator::isReady()
{
    return (m_hwSource->isReady() && m_imageSource->isReady() && m_nnSource->isReady());
}
