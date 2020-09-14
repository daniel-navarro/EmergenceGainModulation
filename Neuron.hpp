//
//  Neuron.hpp
//  exp2
//
//  Created by Dan the Destroyer on 02/05/2017.
//  Copyright © 2017 OFTNAI. All rights reserved.
//

#ifndef Neuron_hpp
#define Neuron_hpp

#include <stdio.h>
#include <vector>

class Neuron {
    public:
        Neuron(int neuron_id, float preferred_retinal_location, float preferred_eye_position, unsigned long int number_synaptic_connections);
        ~Neuron();
    
    
        int GetNeuronID();
    
    
        float GetPreferredRetinalLocation();
        float GetPreferredEyePosition();
        float GetInternalActivation();
        float GetFiringRate();
        std::vector<float> GetArrayWij();
    
        void  SetWijArrayAtIndex(int index, float wij);
        void  ComputeInternalActivation(float current_eye_position, float current_retinal_location);
        void  ComputeFiringRate();
        void  SetSumWijTimesRj(float weight_Wij);
        float GetWijArrayAtIndex(int index);
        void  SetFiringRate(float firing_rate);
    
    private:
        int   neuron_id;
        float preferred_retinal_location;
        float preferred_eye_position;
        float internal_activation;
        float sum_Wij_times_rj;
        std::vector<float> dist_Wij;
    
    
        float firing_rate;     // For postsynaptic activation computation
        float newFiringRate;   // The reason this value was introduced in the Neuron class is that if the model is run in discrete scheme, then newFiringRate values may need to be used
    
        char * aux;
    

};

#endif /* Neuron_hpp */
