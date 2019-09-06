#include "co2amp.h"


C::C(std::string id)
{
    this->id = id;
    type = "C";
    yaml = id + ".yml";

    Debug(2, "Creating optic type \'" + type + "\' from file \'" + yaml + "\'");

    std::string value="";

    // r_max
    if(!YamlGetValue(&value, yaml, "r_max")){
        configuration_error = true;
        return;
    }
    r_max = std::stod(value);
    Debug(2, "r_max = " + toExpString(r_max) + " m");

    // Chirp
    if(!YamlGetValue(&value, yaml, "chirp")){
        configuration_error = true;
        return;
    }   
    chirp = std::stof(value);
    Debug(2, "chirp = " + toExpString(chirp) + " s/Hz");
}


void C::InternalDynamics(double)
{

}


void C::PulseInteraction(Pulse *pulse, Plane* plane, double time)
{
    if(chirp == 0)
        return;

    Debug(2, "Interaction with chirper, chirp = " + toExpString(chirp) + " s/Hz");
    StatusDisplay(pulse, plane, time, "chirping/de-chirping...");

    double Dv = 1.0/(t_max-t_min); // frequency step, Hz
    double v_min = vc - Dv*n0/2;

    #pragma omp parallel for // mulithread
    for(int x=0; x<x0; x++){
        double delay;
        std::complex<double> *spectrum;
        spectrum = new std::complex<double>[n0];
        FFT(pulse->E[x], spectrum);
        for(int n=0; n<n0; n++){
            delay = (v_min+Dv*(0.5+n)-vc) * chirp; // delay increases with frequency (red chirp) - group delay
            delay *= 0.5; // conversion to phase delay.
            spectrum[n] *= exp(I*2.0*M_PI*(v_min+Dv*(0.5+n)-vc)*(-delay)); // no "-" in the exponent in frequency domain E(omega)
        }
        IFFT(spectrum, pulse->E[x]);
        delete[] spectrum;
    }

}

