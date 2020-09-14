//
//  Neuron.cpp
//  exp2
//
//  Created by Dan the Destroyer on 02/05/2017.
//  Copyright © 2017 OFTNAI. All rights reserved.
//

#include "Neuron.hpp"
#include <math.h>
#include <iostream>

Neuron::Neuron(int neuron_id, float preferred_retinal_location, float preferred_eye_position, unsigned long int number_synaptic_connections) {
    this->neuron_id = neuron_id;
    this->preferred_retinal_location = preferred_retinal_location;
    this->preferred_eye_position = preferred_eye_position;
    this->firing_rate = 0;
    
    this->dist_Wij = *new std::vector<float>(number_synaptic_connections); // reserve enough space in memory: trade-off running time x RAM use
    
    this->sum_Wij_times_rj = 0;
    
    this->newFiringRate = 0; // (?)
    
    
    // log
    if (this->preferred_retinal_location == 0 && this->preferred_eye_position == 0) {
        std::cout << "Neuron #" << this->neuron_id << " has preferred retinal location " << this->preferred_retinal_location << " and preferred eye position " << this->preferred_eye_position << std::endl << std::endl;
    }
}

Neuron::~Neuron() {
    
}

float Neuron::GetFiringRate(){
    return this->firing_rate;
}

float Neuron::GetPreferredEyePosition() {
    return this->preferred_eye_position;
}

float Neuron::GetPreferredRetinalLocation() {
    return this->preferred_retinal_location;
}

float Neuron::GetInternalActivation() {
    return this->internal_activation;
}

int Neuron::GetNeuronID() {
    return this->neuron_id;
}

void Neuron::SetSumWijTimesRj(float weight_Wij) {
    this->sum_Wij_times_rj = weight_Wij;
}

void Neuron::SetWijArrayAtIndex(int index, float wij) {
    this->dist_Wij.at(index) = wij;
}

void Neuron::SetFiringRate(float firing_rate) {
    this->firing_rate = firing_rate;
}

float Neuron::GetWijArrayAtIndex(int index) {
    return this->dist_Wij.at(index);
    
}

std::vector<float> Neuron::GetArrayWij() {
    return this->dist_Wij;
}

/**
 *
 * sigmoidGain = 40;
 * alpha = 5;
 * sigmoidal_firing_rate_output_neurons = 1.0 ./ (1.0 + exp(-sigmoidGain*(hi_plus_sum_Wij_times_hj-alpha)));
 *
 */
/*void Neuron::ComputeFiringRate() {
    float sigmoid_gain = 40;
    float alpha = 0;
    std::cerr << "YOU SHOULD NOT BE USING THIS..." << std::endl << std::endl;
    this->firing_rate = 1.0 / (1.0 + exp(-sigmoid_gain*(this->sum_Wij_times_rj-alpha)));
}*/

void Neuron::ComputeInternalActivation(float current_eye_position, float current_retinal_location) {
    
    //std::cout << 'c = ' << current_eye_position << ' and r = ' << current_eye_position << std::endl;
    /**
     * % Eq [2.2] and replacement of Eq [2.3] (Salinas and Abbott, 1996)
     * % Columns for output neurons
     * h_max = 1.0;
     *
     * std_dev = .5; % std2(preferred_locations)
     * p1 = (x - xi).^2;
     * p2 = 2 * (std_dev.^2);
     *
     * output_gaussian_activation_hiv = h_max * exp(-(p1/p2));
     *
     */
    
    
    /**///////////////////////////|  RETINAL TERM   |///////////////////////////**/
    /**/ float hr_max = 2;                                                     /**/
    /**/ float std_dev_r = 6; //3;                                             /**/
    /**/ float p1r = (current_retinal_location-GetPreferredRetinalLocation()); /**/
    /**/ float p2r = 2*(std_dev_r*std_dev_r);                                  /**/
    /**/                                                                       /**/
    /**/ float gaussian_activation_r = hr_max * exp(- ((p1r*p1r)/p2r));        /**/
    /**/////////////////////////////////////////////////////////////////////////**/ width
    
    /**///////////////////////|  GAZE-DIRECTION TERM   |////////////////////////**/
    /**/ float hg_max = 2;                                                    /**/
    /**/ float std_dev_g = 15; //3.741657386773941; //4.5;                     /**/
    /**/ float p1g = (current_eye_position-GetPreferredEyePosition());         /**/
    /**/ float p2g = 2*(std_dev_g*std_dev_g);                                  /**/
    /**/                                                                       /**/
    /**/ float gaussian_activation_g = hg_max * exp(- ((p1g*p1g)/p2g));        /**/
    /**/////////////////////////////////////////////////////////////////////////**/ ampl
    
    this->internal_activation = gaussian_activation_r + 1*gaussian_activation_g;
    
}
