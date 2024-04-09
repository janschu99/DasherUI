#pragma once
#include <xstring>


class OSOutput
{
public:
    OSOutput();
    virtual ~OSOutput();

    void deleteCharacter();
    void outputCharacter(const std::string& sText);
};
