#include "co2amp.h"


M::M(std::string id)
{
    this->id = id;
    this->type = "M";
    this->yaml = id + ".yml";

    Debug(2, "Creating optic type \'" + this->type + "\' from file \'" + this->yaml + "\' ...");

    //std::string value="";
    //YamlGetValue(&value, yaml, "diameter");
    //this->Dr = std::stod(value) / 2 / (x0-1) / 1000; // mm->m
}

void M::InternalDynamics(double clock_time)
{

}


void M::PulseInteraction(int pulse_n)
{

}

