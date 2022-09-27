#include "inferencemanager.h"

InferenceManager::InferenceManager() {
    this->labelFilePath = "";
}

InferenceManager* InferenceManager::getInstance() {
    if (m_singletonInstance == nullptr) {
        m_singletonInstance = new InferenceManager();
    }
    return m_singletonInstance;
}
InferenceState InferenceManager::getState()
{
    return state;
}

//sets the state and notifies all observers of the object
void InferenceManager::setState(InferenceState state)
{
    this->state = state;
    notify();
}

//notify means calling the update method of all of the observer with the current state of the object as argument
void InferenceManager::notify()
{
    for(auto observer : this->observers) {
        observer->update(this->state);
    }
}

void InferenceManager::setCreator(std::shared_ptr<IInferenceCreator> creator)
{
    this->creator = creator;
}

void InferenceManager::registerObserver(IObserver *observer)
{
    this->observers.push_back(observer);
}

void InferenceManager::removeObserver(IObserver *observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end()); //not sure if this works correctly
}

//runs all inferences in the currentInferences object
bool InferenceManager::runInference()
{
    if(this->currentInferences.size()>0) {
        this->setState(InferenceState::RUNNING);
        for(auto inference : this->currentInferences) {
            inference->execute();
        }
        this->setState(InferenceState::COMPLETED);
    }
}


bool InferenceManager::abortInference()
{
    this->setState(InferenceState::RUNNABLE);
    //OPTIONAL CRITERIA
}

//returns a vector of pairs of InferenceResult and DiagnosticData corresponding to the Inference at the same index of currentInferences
std::vector<std::pair<std::shared_ptr<InferenceResult>,std::shared_ptr<DiagnosticData>>> InferenceManager::getAllResultsAndDiagnostics()
{
    std::vector<std::pair<std::shared_ptr<InferenceResult>,std::shared_ptr<DiagnosticData>>> resultsAndDiagnostics;
    for (auto inference : this->currentInferences) {
        resultsAndDiagnostics.push_back(std::make_pair(inference->getResult(),inference->getDiagnostics()));
    }
    if(this->labelFilePath != "") {
        for (auto pair : resultsAndDiagnostics) {
            pair.first->setLabels(this->labelFilePath);
        }
    }
    return resultsAndDiagnostics;
}


bool InferenceManager::createInferences()
{
    if (this->state == InferenceState::RUNNABLE ||
            this->state == InferenceState::COMPLETED) {
        this->currentInferences = this->creator->buildInferences();
    }
}

void InferenceManager::setLabelFilePath(std::string labelFilePath)
{
    this->labelFilePath = labelFilePath;
}

//this method checks all ParameterConfigurator and sets the state of the Inference manager as runnable if all parameters are set
void InferenceManager::checkIfRunnable() {
    if (this->creator->isReady()) {
        this->setState(InferenceState::RUNNABLE);
    } else {
        this->setState(InferenceState::NOT_READY);
    }
}





