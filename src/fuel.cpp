//
// Created by ilias on 8/12/23.
//

#include <stdio.h>
#include <string.h>

#include "fuel.h"

#define JET_A1_density 0.785    // kg/l @ 15C.
#define AVGAS_density 0.72      // kg/l @ 15C. @ -40C 0.743. Decreases by 0.5% every 5C in temp.


float fuel_density(float temp, char * type)
{
    float density = 0;
    float temp_diff = temp - 15;
    float factor = 1 - 0.005 * (temp_diff / 5);


    if (strcmp(type, "AVGAS") == 0)
    {
        density = AVGAS_density * factor;
    }

    return density;
}