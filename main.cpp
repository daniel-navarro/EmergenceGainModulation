//
//  main.cpp
//  exp2
//
//  Created by Dan the Destroyer on 02/05/2017.
//  Copyright © 2017 OFTNAI. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "Population.hpp"
#include "engine.h"
#include <math.h>
#include <iomanip>
#define  BUFSIZE 256

template <typename T>
std::vector<T> linspace(T begin, T end, size_t N);

void testNeuronResponseAgainstEyePositionsInItsPreferredRetinalLocation(int neuron_index, std::vector<float> testing_eye_positions, Neuron *chosen_neuron);

int main(int argc, const char * argv[]) {
    /******************/
    float r1 = -10, r2 =  10;
    float e1 = -35, e2 =  35;
    float sparseness  =  1;
    
    // create linearly spaced retinal locations [r1,r2] and eye positions intervals [e1.e2]
    std::vector<float> preferred_retinal_locations = linspace(r1, r2, (r2-r1+1));
    
    std::vector<float> preferred_eye_positions = linspace(e1, e2, (e2-e1+1));
    
    // || \\ || // || \\ || // || \\ || // || \\ || // || \\ || // || \\ // || \\ || // || \\ || // || \\ // || \\ || // || \\ || // ||
    // Print all retinal locations
    // for (int i = 0; i < preferred_retinal_locations.size(); i++) {
    //    if (preferred_retinal_locations.at(i) == 0) std::cout << std::endl << "there you go: " << i << std::endl << std::endl;
    //    std::cout << preferred_retinal_locations.at(i) << ", ";
    //}
    //std::cout << std::endl << std::endl;
    // Print all retinal locations
    // || \\ || // || \\ || // || \\ || // || \\ || // || \\ || // || \\ // || \\ || // || \\ || // || \\ // || \\ || // || \\ || // ||
    
    
    // (i) creates our population of neurons
    Population outputPopulation = *new Population(preferred_retinal_locations, preferred_eye_positions, sparseness);
    // END OF (i)
    
    // (ii) creates testing intervals (same as preferred for the moment)
    e1=  -35; e2= 35;
    std::vector<float> testing_eye_positions      = linspace(e1, e2, (e2-e1+1)); //(r2-r1+1));
    
    r1 = -10; r2 = 10;
    std::vector<float> testing_retinal_locations  = linspace(r1, r2, (r2-r1+1));
    // END OF (ii)
    
    // save lists of firing rates that are exported as csv files
    std::vector<float> firing_rate_list(71);
    
    // (iii) calculates internal activation for all output neurons (optionally prints data for neuron at index 'neuron_index'
    int neuron_index = 1489;
    Neuron *chosen_neuron = &outputPopulation.neurons.at(neuron_index);
    float preferred_retinal_location = -5; //chosen_neuron->GetPreferredRetinalLocation();
    
    std::vector<float>::iterator r;
    std::vector<float>::iterator e;
    
    std::vector<Neuron>::iterator neuron;
    std::vector<Neuron>::iterator neuron_i;
    std::vector<Neuron>::iterator neuron_j;
    
    float sigmoidGain = .48;
    float alpha       = 5.125;
    
    
    
    // (neuron_index, testing_eye_positions, chosen_neuron);
    //std::cout << "Hey Dan: " << outputPopulation.getNeuronAtIndex(neuron_index).GetInternalActivation() << std::endl;
    
    
    std::cout << "\t\t\t\t\t\t\t\t\t\t\t.:: Neuron #" << outputPopulation.neurons.at(neuron_index).GetNeuronID() << " e=" << outputPopulation.neurons.at(neuron_index).GetPreferredEyePosition() << " r=" << outputPopulation.neurons.at(neuron_index).GetPreferredRetinalLocation() << " ::." << std::endl; // print
    
    for (neuron_i = outputPopulation.neurons.begin(); neuron_i != outputPopulation.neurons.end(); neuron_i++) {
        for (e = testing_eye_positions.begin(); e != testing_eye_positions.end(); e++) {
            neuron_i->ComputeInternalActivation(*e, neuron_i->GetPreferredRetinalLocation());
            
            neuron_i->SetFiringRate((1 / (1 + exp(-sigmoidGain*(neuron_i->GetInternalActivation() + - alpha)))));
            
            //if (neuron_i->GetNeuronID() == neuron_index) std::cout << "Firing rate of neuron #" << neuron_i->GetNeuronID() << " for eye-position " << *e << " and visual target location " << neuron_i->GetPreferredRetinalLocation() << ":   " << neuron_i->GetFiringRate() << std::endl;
            
            std::cout << neuron_i->GetFiringRate() << ' ';
        }
        std::cout << ";" << std::endl;
        
    }
    std::cout << std::endl << "Hello Dan" << std::endl;
    
    // code from 'exp2' in this same directory. See README for details.
    /*for (e = testing_eye_positions.begin(); e != testing_eye_positions.end(); e++) {
        //std::cout << ";" << std::endl;
        //std::cout << *e << ' ';
        
        // (i) internal activation of all output neurons (depends only on preferred and current eye-positions/retinal locations)
        for (neuron = outputPopulation.neurons.begin(); neuron != outputPopulation.neurons.end(); neuron++) {
            neuron->ComputeInternalActivation(*e, neuron->GetPreferredRetinalLocation());
        }
        
        // (ii) firing rate of all output neurons
        float sum_Wij_times_rj = 0;
        int initial_condition  = 0; // until sets on a stable state (currently doing it twice)
        
        for (int i=initial_condition; i<5; i++) {
            for (neuron_i = outputPopulation.neurons.begin(); neuron_i != outputPopulation.neurons.end(); neuron_i++) {
                for (neuron_j = outputPopulation.neurons.begin(); neuron_j != outputPopulation.neurons.end(); neuron_j++) {
                    // multiply each individual dist_Wij by correspondent rj (based on neuronID == index)  <===l;'
                    sum_Wij_times_rj += (neuron_i->GetWijArrayAtIndex(neuron_j->GetNeuronID())*neuron_j->GetFiringRate());
                }
                neuron_i->SetFiringRate((1 / (1 + exp(-sigmoidGain*(neuron_i->GetInternalActivation() + 0*sum_Wij_times_rj - alpha)))));
                
                // prints firing rates of chosen_neuron for all testing_eye_positions
                if (i == 4 && neuron_i->GetNeuronID() == neuron_index) {
                    //std::cout << "Firing rate for e =  " << *e << ": " << neuron_i->GetFiringRate() << std::endl;
                    std::cout << neuron_i->GetFiringRate() << ' ';
                    //firing_rate_list.push_back(neuron_i->GetFiringRate());
                }
                
                / * prints firing rates for preferred eye-position and r == preferred_retinal_location
                if (i == 4 && neuron_i->GetPreferredRetinalLocation() == preferred_retinal_location && neuron_i->GetPreferredEyePosition() == (int) *e) {
                    std::cout << "Neuron ID " << neuron_i->GetNeuronID() << " with preferred e = " << neuron_i->GetPreferredEyePosition() << " and r = " << neuron_i->GetPreferredRetinalLocation() << " has firing rate of " << neuron_i->GetFiringRate() << std::endl;
                    //std::cout << neuron_i->GetFiringRate() << ' ';
                } * /
            }
        }
        
        
        //std::cout << "r = " << preferred_retinal_location << " and e = " << *e << " firing rate = " << chosen_neuron->GetFiringRate() << std::endl;
        //std::cout << chosen_neuron->GetFiringRate() << ' ';
    } */
    
    
    
    
    // uncomment for (e = testing_eye_positions.begin(); e != testing_eye_positions.end(); e++) {
    // uncomment     for (r = testing_retinal_locations.begin(); r != testing_retinal_locations.end(); r++) {
            // (i) internal activation of chosen output neuron
            //chosen_neuron->ComputeInternalActivation(*e, *r);
            
            // (ii) internal activation of all output neurons
            // uncomment for (neuron = outputPopulation.neurons.begin(); neuron != outputPopulation.neurons.end(); neuron++) {
                // uncomment neuron->ComputeInternalActivation(*e, *r);
            // uncomment }
            
            // (iii) firing rate of all output neurons
            // uncomment float sum_Wij_times_rj = 0;
            // uncomment int initial_condition = 4;
            
            // uncomment for (int i=initial_condition; i<5; i++) {
                
                // uncomment for (neuron_i = outputPopulation.neurons.begin(); neuron_i != outputPopulation.neurons.end(); neuron_i++) {
                    
                    // uncomment for (neuron_j = outputPopulation.neurons.begin(); neuron_j != outputPopulation.neurons.end(); neuron_j++) {
                        // multiply each individual dist_Wij by correspondent rj (based on neuronID == index)  <===l;'
                        // uncomment sum_Wij_times_rj += (neuron_i->GetWijArrayAtIndex(neuron_j->GetNeuronID())*neuron_j->GetFiringRate());
                    // uncomment }
                    //std::cout << "1 + " << (neuron_i->GetInternalActivation() + sum_Wij_times_rj - alpha) << std::endl;
                    // uncomment neuron_i->SetFiringRate((1 / (1 + exp(-sigmoidGain*(neuron_i->GetInternalActivation() + sum_Wij_times_rj - alpha)))));
                    
                    //std::cout << std::endl << std::endl << " here --> " << sum_Wij_times_rj << " <-- " << std::endl << std::endl << std::endl;
                    
                    //std::cout  << i << " -> firing rate of neuron #" << neuron_i->GetNeuronID() << "=(" << neuron_i->GetFiringRate() << ") and Wij*rj=(" << sum_Wij_times_rj << ") " << std::endl;
                // uncomment }
            // uncomment }
            
            // (iv) firing rate of chosen ouput neuron for current (r,e)   <====  HERE
//            sum_Wij_times_rj = 0;
//            //for (neuron_i = outputPopulation.neurons.begin(); neuron_i != outputPopulation.neurons.end(); neuron_i++) {
//                for (neuron_j = outputPopulation.neurons.begin(); neuron_j != outputPopulation.neurons.end(); neuron_j++) {
//                    // multiply each individual dist_Wij by correspondent rj (based on neuronID == index)  <===l;'
//                    sum_Wij_times_rj += chosen_neuron->GetWijArrayAtIndex(neuron_j->GetNeuronID())*neuron_j->GetFiringRate();
//                }
//                chosen_neuron->SetFiringRate(1 / (1 + exp(-.002*4*(chosen_neuron->GetInternalActivation() + sum_Wij_times_rj - 0))));
            
                // uncomment std::cout << "\t" << chosen_neuron->GetFiringRate(); // << "(" << *r << "," << *e << ") "; // print
                //std::cout  << " -> firing rate of chosen neuron #" << chosen_neuron->GetNeuronID() << "=(" << chosen_neuron->GetFiringRate() << ") and Wij*rj=(" << sum_Wij_times_rj << ") " << std::endl;
                //std::cout  << i << " -> firing rate of neuron #" << neuron_i->GetNeuronID() << "=(" << neuron_i->GetFiringRate() << ") and Wij*rj=(" << sum_Wij_times_rj << ") " << std::endl;
            //}
        // uncomment }
        // uncomment std::cout << ";" << std::endl;
    // uncomment }
    
    // uncomment std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
    
    
//    for (neuron = outputPopulation.neurons.begin(); neuron != outputPopulation.neurons.end(); neuron++) {
//        //if (neuron->GetNeuronID() == neuron_index) std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t.:: Neuron #" << outputPopulation.neurons.at(neuron_index).GetNeuronID() << " e=" << outputPopulation.neurons.at(neuron_index).GetPreferredEyePosition() << " r=" << outputPopulation.neurons.at(neuron_index).GetPreferredRetinalLocation() << " ::." << std::endl; // print
//        for (n = testing_eye_positions.begin(); n != testing_eye_positions.end(); n++) {
//            
//            
//            //std::cout << "Eye-position: " << *n << std::endl << std::endl << std::endl;
//            for (r = testing_retinal_locations.begin(); r != testing_retinal_locations.end(); r++) {
//                //std::cout << std::endl << std::endl << std::endl << "r=" << *r << ", e=" << *n << std::endl << std::endl << std::endl;
//                
//                neuron->ComputeInternalActivation(*n, *r);
//                
//                /****/
//                float sum_Wij_times_rj = 0;
//                
//                int initial_condition = 4;
//                for (int i=initial_condition; i<5; i++) {
//                    for (neuron_i = outputPopulation.neurons.begin(); neuron_i != outputPopulation.neurons.end(); neuron_i++) {
//    
//                        for (neuron_j = outputPopulation.neurons.begin(); neuron_j != outputPopulation.neurons.end(); neuron_j++) {
//                            // multiply each individual dist_Wij by correspondent rj (based on neuronID == index)  <===l;'
//                            sum_Wij_times_rj += neuron_i->GetWijArrayAtIndex(neuron_j->GetNeuronID())*neuron_j->GetFiringRate();
//                        }
//                        neuron_i->SetFiringRate(1 / (1 + exp(-.002*4*(neuron_i->GetInternalActivation() + sum_Wij_times_rj - 0))));
//                        //std::cout  << i << " -> firing rate of neuron #" << neuron_i->GetNeuronID() << "=(" << neuron_i->GetFiringRate() << ") and Wij*rj=(" << sum_Wij_times_rj << ") " << std::endl;
//                    }
//                }
//                
//                if (neuron->GetNeuronID() == neuron_index) std::cout << "\t\t" << neuron->GetFiringRate();// << "(" << *r << "," << *n << ") "; // print
//                
//                //for (neuron_i = outputPopulation.neurons.begin(); neuron_i != outputPopulation.neurons.end(); neuron_i++) {
//                //    std::cout  << "\t\t" << neuron_i->GetFiringRate();
//                //}
//                /****/
//                
//                
//                //if (neuron->GetNeuronID() == neuron_index) std::cout << "\t\t" << neuron->GetInternalActivation();// << "(" << *r << "," << *n << ") "; // print
//            }
//            if (neuron->GetNeuronID() == neuron_index) std::cout << ";" << std::endl; // print
//        }
//        if (neuron->GetNeuronID() == neuron_index) std::cout << std::endl; // print
//    }
    
    
    
    
    // uncomment std::cout << std::endl; // print
    // END OF (iii)
    
    
    // (iv) calculates firing rate
    //std::vector<Neuron>::iterator neuron_i;
    //std::vector<Neuron>::iterator neuron_j;
    
    
    
    // END OF (iv)
    
    // (v) print final firing rates:
    //std::cout << std::endl << std::endl << "FIRING RATES" << std::endl << "Oooops, not yet! ;) " << std::endl << std::endl << std::endl << std::endl;
    
    // END OF (v)
    
    
    
    
    
    
    
//    std::vector<float> temp = outputPopulation.neurons.at(0).GetArrayWij();
//    
//    for (int i =0; i<temp.size(); i++) {
//        std::cout << temp.at(i) << " ";
//    }
    
    std::cout << std::endl << "Population size: " << outputPopulation.getPopulationSize() << std::endl;
    
    return 0;
    
}


/**
 * This method does what it says it does. Seriously though:
 * TODO: write a better method description :P
 *
 */
void testNeuronResponseAgainstEyePositionsInItsPreferredRetinalLocation(int neuron_index, std::vector<float> testing_eye_positions, Neuron *chosen_neuron) {
    std::cout << "Hello Dan." << std::endl << "It seems that you chose neuron #" << neuron_index << ". Good choice!" << std::endl;
    std::cout << "Log: Preferred retinal location: " << chosen_neuron->GetPreferredRetinalLocation()  << ". " << std::endl;
    std::cout << "Log: Range of testing eye-positions: [" << testing_eye_positions[0] << ", " << testing_eye_positions.back() << "]." << std::endl;
    
    for (std::vector<float>::iterator it = testing_eye_positions.begin(); it != testing_eye_positions.end(); ++it) {
        chosen_neuron->ComputeInternalActivation(*it, chosen_neuron->GetPreferredRetinalLocation());
    }
    
}















/**
 * My version of MATLAB's 'linspace' :)
 *
 */
template <typename T = float>
std::vector<T> linspace(T begin, T end, size_t N) {
    T h = (end - begin) / static_cast<T>(N-1);
    
    std::vector<T> xs(N);
    
    typename std::vector<T>::iterator x;
    
    T val;
    
    for (x = xs.begin(), val = begin; x != xs.end(); ++x, val +=h)
        *x= val;
    
    return xs;
}



/* Backup line 77 to 123 (?)
 for (e = testing_eye_positions.begin(); e != testing_eye_positions.end(); e++) {
 for (r = testing_retinal_locations.begin(); r != testing_retinal_locations.end(); r++) {
 // (i) internal activation of chosen output neuron
 //chosen_neuron->ComputeInternalActivation(*e, *r);
 
 // (ii) internal activation of all output neurons
 for (neuron = outputPopulation.neurons.begin(); neuron != outputPopulation.neurons.end(); neuron++) {
 neuron->ComputeInternalActivation(*e, *r);
 }
 
 // (iii) firing rate of all output neurons
 float sum_Wij_times_rj = 0;
 int initial_condition = 4;
 
 for (int i=initial_condition; i<5; i++) {
 
 for (neuron_i = outputPopulation.neurons.begin(); neuron_i != outputPopulation.neurons.end(); neuron_i++) {
 
 for (neuron_j = outputPopulation.neurons.begin(); neuron_j != outputPopulation.neurons.end(); neuron_j++) {
 // multiply each individual dist_Wij by correspondent rj (based on neuronID == index)  <===l;'
 sum_Wij_times_rj += (neuron_i->GetWijArrayAtIndex(neuron_j->GetNeuronID())*neuron_j->GetFiringRate());
 }
 //std::cout << "1 + " << (neuron_i->GetInternalActivation() + sum_Wij_times_rj - alpha) << std::endl;
 neuron_i->SetFiringRate((1 / (1 + exp(-sigmoidGain*(neuron_i->GetInternalActivation() + sum_Wij_times_rj - alpha)))));
 
 //std::cout << std::endl << std::endl << " here --> " << sum_Wij_times_rj << " <-- " << std::endl << std::endl << std::endl;
 
 //std::cout  << i << " -> firing rate of neuron #" << neuron_i->GetNeuronID() << "=(" << neuron_i->GetFiringRate() << ") and Wij*rj=(" << sum_Wij_times_rj << ") " << std::endl;
 }
 }
 
 // (iv) firing rate of chosen ouput neuron for current (r,e)   <====  HERE
 //            sum_Wij_times_rj = 0;
 //            //for (neuron_i = outputPopulation.neurons.begin(); neuron_i != outputPopulation.neurons.end(); neuron_i++) {
 //                for (neuron_j = outputPopulation.neurons.begin(); neuron_j != outputPopulation.neurons.end(); neuron_j++) {
 //                    // multiply each individual dist_Wij by correspondent rj (based on neuronID == index)  <===l;'
 //                    sum_Wij_times_rj += chosen_neuron->GetWijArrayAtIndex(neuron_j->GetNeuronID())*neuron_j->GetFiringRate();
 //                }
 //                chosen_neuron->SetFiringRate(1 / (1 + exp(-.002*4*(chosen_neuron->GetInternalActivation() + sum_Wij_times_rj - 0))));
 
 std::cout << "\t" << chosen_neuron->GetFiringRate(); // << "(" << *r << "," << *e << ") "; // print
 //std::cout  << " -> firing rate of chosen neuron #" << chosen_neuron->GetNeuronID() << "=(" << chosen_neuron->GetFiringRate() << ") and Wij*rj=(" << sum_Wij_times_rj << ") " << std::endl;
 //std::cout  << i << " -> firing rate of neuron #" << neuron_i->GetNeuronID() << "=(" << neuron_i->GetFiringRate() << ") and Wij*rj=(" << sum_Wij_times_rj << ") " << std::endl;
 //}
 }
 std::cout << ";" << std::endl;
 }
 
 */
