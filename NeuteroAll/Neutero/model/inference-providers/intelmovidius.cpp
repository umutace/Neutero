#include "intelmovidius.h"
#include <fstream>

IntelMovidius::IntelMovidius()
{

}

// Use "lsusb" command to get the name of the connected neural compute stick
std::string IntelMovidius::getModelName() const
{
    std::system("lsusb | grep Movidius >> movidius_name.txt");
    std::ifstream ifs("movidius_name.txt");
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    std::string result = content.substr(content.find("Intel"));
    std::remove("movidius_name.txt");
    return result;
}

std::string IntelMovidius::toString() const
{
    return "IntelMovidius";
}
