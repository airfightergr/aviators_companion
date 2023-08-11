//
// Created by ilias on 8/2/23.
//

// system headers
#include <stdio.h>
#include <string.h>

// my header files
#include "conversions.h"
//#include "cimgui_main.h"

//Defines
#define KGR_TO_LBS = 2.20462;
#define LBS_TO_KGR = 0.453592;
#define F_TO_C = 2.154;
#define C_TO_F = 0.254;

float my_size = .546;

void main_func(void)
{



    // Select the type of conversion you want
    int conv_sel;
    printf("Enter 1 for Temperature conversion, or 2 for Weight conversion: ");
    scanf("%d", &conv_sel);

    if (conv_sel == 1)
    {
        // select which temp conversion you want
        int temp_sel;
        printf("Enter 1 for F to C, or 2 for C to F: ");
        scanf("%d", &temp_sel);

        if (temp_sel == 1)
        {
            float temp_F;
            printf("Enter Temperature in F: ");
            scanf("%f", &temp_F);
            printf("Temperature in C is: %4.2f.\n", temp_f_to_c(temp_F));
        }
        else if (temp_sel == 2)
        {
            float temp_C;
            printf("Enter Temperature in C: ");
            scanf("%f", &temp_C);
            printf("Temperature in F is: %4.2f.\n", temp_c_to_f(temp_C));
        }
        else
        {
            printf("Wrong selection! Try again!\n");
        }
    }
    else if (conv_sel == 2)
    {
        // Select
        int wgt_sel;
        printf("Enter 1 for KGS to LBS, or 2 for LBS to KGS: ");
        scanf("%d", &wgt_sel);

        if (wgt_sel == 1)
        {
            float wgt_kg;
            printf("Enter Weight in kilograms: ");
            scanf("%f", &wgt_kg);
            printf("The weight in pounds is: %4.2f\n", weight_kg_to_lbs(wgt_kg));
        }
        else if (wgt_sel == 2)
        {

            float wgt_lb;
            printf("Enter Weight in pounds: ");
            scanf("%f", &wgt_lb);
            printf("The weight in kilograms is: %4.2f\n", weight_lbs_to_kg(wgt_lb));
        }
        else
        {
            printf("Wrong selection! Try again!\n");
        }
    }
    else
    {
        printf("Wrong selection! Try again!\n");
    }

//    return 0;
}
