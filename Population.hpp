//
//  Population.hpp
//  exp2
//
//  Created by Dan the Destroyer on 02/05/2017.
//  Copyright © 2017 OFTNAI. All rights reserved.
//

#ifndef Population_hpp
#define Population_hpp

#include <stdio.h>
#include "Neuron.hpp"
#include <vector>

class Population {
    public:
        Population(std::vector<float> preferred_retinal_locations, std::vector<float> preferred_eye_positions, float sparseness);
        ~Population();
        void init(std::vector<float> preferred_retinal_locations, std::vector<float> preferred_eye_positions, unsigned long int number_afferent_synapses);
    
    
        Neuron getNeuronAtIndex(int index);
        unsigned long getPopulationSize();
        
    
        std::vector<Neuron> neurons;
    
        void printNeuronalPreferences();
    
    private:
        unsigned long populationSize;
        unsigned long number_afferent_synapses;
    
    

    
};

#endif /* Population_hpp */
