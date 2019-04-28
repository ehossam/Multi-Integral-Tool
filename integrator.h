#ifndef INTEGRATOR_H
#define INTEGRATOR_H

//strings section
#define WELCOME_MESSAGE "Welcome to The ENGR313 Program\nFor the function option enter 'f' or 'F'\nFor the datapoints option enter 'd' or 'D'\n"
#define FUNCTION_MESSAGE "Enter the internal function without specifying dx or dy or ...etc:\n"
#define UNKOWN_INPUT_MESSAGE "uknown choice\n"
#define FUNCTION 'f'
#define DATAPOINTS 'd'


#include <string>

class Integrator
{
public:
    Integrator();
    std::string getResult(std::string,std::string);
};

#endif // INTEGRATOR_H
