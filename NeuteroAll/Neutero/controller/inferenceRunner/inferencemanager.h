#ifndef INFERENCEMANAGER_H
#define INFERENCEMANAGER_H

#include "subject.h"
#include <controller/inferencecreator.h>
#include "inferencestate.h"
#include <iostream>
#include <vector>
#include <model/inference.h>

class InferenceManager : public ISubject
{
private:
    inline static InferenceManager* m_singletonInstance = nullptr;
    InferenceManager();
    std::vector<IObserver*> observers;
    std::vector<std::shared_ptr<Inference>> currentInferences;
    InferenceState state;
    std::shared_ptr<IInferenceCreator> creator;
    std::string labelFilePath;

public:
    static InferenceManager* getInstance();
    InferenceState getState();
    void setState(InferenceState state);
    void setCreator(std::shared_ptr<IInferenceCreator> creator);
    void registerObserver(IObserver* observer) override;
    void removeObserver(IObserver* observer) override;
    void notify() override;
    bool runInference();
    bool abortInference();
    bool createInferences();
    std::vector<std::pair<std::shared_ptr<InferenceResult>,std::shared_ptr<DiagnosticData>>> getAllResultsAndDiagnostics();
    void setLabelFilePath(std::string labelFilePath);
    void checkIfRunnable();
};

#endif // INFERENCEMANAGER_H
