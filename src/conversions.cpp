//
// Created by ilias on 8/2/23.
//

#include <stdio.h>
#include <string.h>

#include "conversions.h"

#define WEIGHT_CONV 2.2046;

float temp_c_to_f(float tempC){

    float tempF;

    tempF = ((tempC * 9)/5) + 32;

    return tempF;
}


float temp_f_to_c(float tempF){

    float tempC;

    tempC = ((tempF - 32) * 5)/ 9; //0.56;

    return tempC;
}


float weight_kg_to_lbs(float kg){

    float lbs;

    lbs = kg * WEIGHT_CONV;

    return lbs;
}


float weight_lbs_to_kg(float lbs){

    float kg;

    kg = lbs / WEIGHT_CONV;

    return kg;
}


double hpa_to_inHg(double hpa){

    double inHg;

    inHg = hpa * 0.0295302;

    return inHg;
}


double inHg_to_hpa(double inHg){

    double hpa;

    hpa = inHg / 0.0295302;

    return hpa;
}



