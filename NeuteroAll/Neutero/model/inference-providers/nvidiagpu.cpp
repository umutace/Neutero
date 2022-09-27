#include "nvidiagpu.h"
#include <fstream>

NvidiaGPU::NvidiaGPU()
{

}

// Use "nvidia-smi" command to get the GPU type
std::string NvidiaGPU::getModelName() const
{
    std::system("nvidia-smi --query-gpu=name --format=csv,noheader >> nvidia_name.txt");
    std::ifstream ifs("nvidia_name.txt");
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    std::remove("nvidia_name.txt");
    return content;
}

std::string NvidiaGPU::toString() const
{
    return "NvidiaGPU";
}
