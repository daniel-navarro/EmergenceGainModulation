//
//  Population.cpp
//  exp2
//
//  Created by Dan the Destroyer on 02/05/2017.
//  Copyright © 2017 OFTNAI. All rights reserved.
//

#include "Population.hpp"
#include <iostream>
#include <math.h>

// TODO set here limits for eye position and retinal location as parameters to set each neuron within the population
float preferred_eye_positions;

Population::Population(std::vector<float> preferred_retinal_locations, std::vector<float> preferred_eye_positions, float sparseness) {
    this->populationSize = preferred_retinal_locations.size()*preferred_eye_positions.size();
    this->number_afferent_synapses = this->populationSize*sparseness;
    
    // TODO: if sparseness is NOT 1, then we need to define the set of j's for each i
    if (sparseness != 1) { std::cerr << "Set of afferent synaptic connections is not defined." << std::endl;  }
    
    init(preferred_retinal_locations, preferred_eye_positions, number_afferent_synapses);
}

Population::~Population(){

}

void Population::init(std::vector<float> preferred_retinal_locations, std::vector<float> preferred_eye_positions, unsigned long int number_afferent_synapses) {
    typename std::vector<float>::iterator r;
    typename std::vector<float>::iterator e;
    typename std::vector<Neuron>::iterator neuron_i;
    typename std::vector<Neuron>::iterator neuron_j;
    
    int id =0;
    for (r = preferred_retinal_locations.begin(); r != preferred_retinal_locations.end(); r++) {
         for (e = preferred_eye_positions.begin(); e != preferred_eye_positions.end(); e++) {
            
            this->neurons.push_back(*new Neuron(id++, *r, *e, number_afferent_synapses));
            //std::cout << id << ": " << *r << " and " << *e << "\n";
        }
    }
    
    
    
    /**
     * % Eq [2.4] (Salinas and Abbott, 1996)
     * % Sets Wij for each output neuron i
     *
     * 'constant_A_E' stands for A_E in Eq. [2.4]
     * 'delta_E_2'      stands for delta_E^2 in Eq. [2.4]
     *
     * std_dev = .5; % std2(preferred_locations)
     * p1 = (x - xi).^2;
     * p2 = 2 * (std_dev.^2);
     *
     * output_gaussian_activation_hiv = h_max * exp(-(p1/p2));
     *
     */
    float constant_A_E = .05;
    float delta_E_2 = 1;
    for (neuron_i = this->neurons.begin(); neuron_i != this->neurons.end(); neuron_i++)
        for (neuron_j = this->neurons.begin(); neuron_j != this->neurons.end(); neuron_j++)
            neuron_i->SetWijArrayAtIndex(neuron_j->GetNeuronID(), constant_A_E * exp(-((neuron_i->GetPreferredRetinalLocation()-neuron_j->GetPreferredRetinalLocation())*(neuron_i->GetPreferredRetinalLocation()-neuron_j->GetPreferredRetinalLocation()))/(2*delta_E_2)));
    
}

Neuron Population::getNeuronAtIndex(int index){
    if (index<0 || index>=getPopulationSize()) { std::cerr << "ERROR: Index out of bounds (" << index << ")\n"; exit(1);}
    return this->neurons.at(index);
}

unsigned long Population::getPopulationSize() {
    if (this->neurons.size() != this->populationSize) { std::cerr << "ERROR: Change in population size (" << this->neurons.size() << " != " << this->populationSize << ")\n"; exit(1);}
    return this->neurons.size();
}

void Population::printNeuronalPreferences() {
    typename std::vector<Neuron>::iterator n;
    
    
    for (n = this->neurons.begin(); n != this->neurons.end(); n++) {
        std::cout << "Neuron #" << n->GetNeuronID() << ": (" << n->GetPreferredEyePosition() << "," << n->GetPreferredRetinalLocation() << ")" << std::endl;
    }
    std::cout << "TOTAL OF " << this->getPopulationSize() << " NEURON(S)." << std::endl;
    std::cout << "Where each pair represents preferred eye position and preferred retinal location." << std::endl << std::endl;
    
}
