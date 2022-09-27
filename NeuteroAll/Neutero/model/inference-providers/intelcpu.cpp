#include "intelcpu.h"
#include <fstream>
#include <regex>

IntelCPU::IntelCPU()
{

}

// Use "lscpu" command to get CPU model
std::string IntelCPU::getModelName() const
{
    std::system("lscpu | grep 'Model name' -m 1 >> intelcpu_name.txt");
    std::ifstream ifs("intelcpu_name.txt");
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    std::remove("intelcpu_name.txt");
    std::regex whitespace("[' ']{2,}");
    std::string withoutWhitepace = std::regex_replace(content, whitespace, " ");
    return std::regex_replace(withoutWhitepace, std::regex("Model name: "), "");
}

std::string IntelCPU::toString() const
{
    return "IntelCPU";
}
