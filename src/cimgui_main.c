//
// Created by ilias on 8/9/23.
//
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "cimgui_impl.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>

// My header files
#include "main.h"
#include "conversions.h"
#include "images.h"

extern float my_size;

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

GLFWwindow *window;

int main(int argc, char *argv[])
{


    if (!glfwInit())
        return -1;

    // Decide GL+GLSL versions
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

#if __APPLE__
    // GL 3.2 Core + GLSL 150
  const char *glsl_version = "#version 150";
#else
    // GL 3.2 + GLSL 130
    const char *glsl_version = "#version 150";
#endif

    // just an extra window hint for resize
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // first winow
    window = glfwCreateWindow(1024, 768, "Aviator's Companion", NULL, NULL);
    if (!window)
    {
        printf("Failed to create window! Terminating!\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // enable vsync
    glfwSwapInterval(1);

    // check opengl version sdl uses
    printf("opengl version: %s\n", (char *)glGetString(GL_VERSION));

    // setup imgui
    igCreateContext(NULL);

    // set docking
    ImGuiIO *ioptr = igGetIO();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
#ifdef IMGUI_HAS_DOCK
    ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
#endif

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    igStyleColorsDark(NULL);

    // Custom Font?
//    ImFontAtlas_AddFontFromFileTTF(ioptr->Fonts,"../fonts/SVBasicManual.ttf", 20, NULL, NULL);
    ImFontAtlas_AddFontFromFileTTF(ioptr->Fonts,"/home/ilias/Programming/C/aviators_companion/fonts/Cabin-Regular.otf", 20, NULL, NULL);
    //ImFontAtlas_AddFontFromFileTTF(igGetIO()->Fonts, "fonts/Sono_Regular.ttf", 20, NULL, NULL);


    bool showDemoWindow = false;
    bool showAnotherWindow = false;
    ImVec4 clearColor;
    clearColor.x = 0.12f;
    clearColor.y = 0.12f;
    clearColor.z = 0.12f;
    clearColor.w = 1.0f;

    ImVec4 frame_col;
    frame_col.x = 0.25f;
    frame_col.y = 0.25f;
    frame_col.z = 0.25f;
    frame_col.w = 0.5f;

    // init variables here before the main event loop (while)
    bool quit = false;

    const char * temp_scale[] = { "Celsius to Fahrenheit", "Fahrenheit to Celsius"};
    static const char * curr_temp_scale = "Celsius to Fahrenheit";
    ImVec2 temp_size;
    temp_size.x = 0;
    temp_size.y = 0;
    char celsius[8];
    char fahren[8];
    char hpa[8];
    char inhg[8];

    const char * fuel_types[] = {"JET A-1", "AVGAS"};
    static const char * curr_fuel_type = "JET A-1";
    const char * select_fuel_unit[] = {"Volume (USGAL)", "Volume (LTR)","Weight (KG)", "Weight (LBS)"};
    static const char * curr_select_fuel_unit = "Volume (USGAL)";

    const char * image = "/home/ilias/Programming/C/aviators_companion/images/background.png";




    ImVec2 back_size;
    back_size.x = 1024;
    back_size.y = 768;

    // main event loop
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        // start imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        igNewFrame();

        if (showDemoWindow)
            igShowDemoWindow(&showDemoWindow);

        // show a simple window that we created ourselves.
        {

            // Temp Conv
            igPushStyleVar_Float(ImGuiStyleVar_WindowRounding,10.0f);
            igPushStyleVar_Float(ImGuiStyleVar_FrameRounding,5.0f);
            igPushStyleColor_Vec4(ImGuiCol_WindowBg, frame_col);

            const char * temps = "temps";
            igBegin(temps, NULL,  ImGuiWindowFlags_NoResize |ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
            igText("TEMPERATURE CONVERSION");
            if (igBeginTabBar("tabs", ImGuiTabBarFlags_None))
            {
                if (igBeginTabItem("Celsius to F", NULL, ImGuiTabItemFlags_None))
                {
                    igNewLine();
                    igText("Enter Temp in Celsius:");
                    igSameLine(0, 30);
                    igPushItemWidth(65);
                    igInputText("##c_temp", celsius, sizeof(celsius), ImGuiInputTextFlags_None, 0,0);
                    igPopItemWidth();
                    float c_out = temp_c_to_f(atof(celsius));
                    igText("Fahrenheit: ");
                    if (strcmp(celsius, "") != 0)   // show output value only if user has enter some value
                    {
                        igSameLine(0, 105);
                        igText("%.1f", c_out);
                    }
                    igEndTabItem();
                }

                if (igBeginTabItem("Fahreneit to C", NULL, ImGuiTabItemFlags_None))
                {
                    igNewLine();
                    igText("Enter Temp in Fahrenheit:");
                    igSameLine(0, 10);
                    igPushItemWidth(65);
                    igInputText("##f_temp", fahren, sizeof(fahren), ImGuiInputTextFlags_None, 0,0);
                    igPopItemWidth();
                    float f_out = temp_f_to_c(atof(fahren));
                    igText("Celsius: ");
                    if (strcmp(fahren, "") != 0)    // show output value only if user has enter some value
                    {
                        igSameLine(0, 137);
                        igText("%.1f", f_out);
                    }

                    igEndTabItem();
                }
            }

            igEndTabBar();
            igPopStyleVar(2);
            igPopStyleColor(1);
            igEnd();


            // Baro pressure window
            igPushStyleVar_Float(ImGuiStyleVar_WindowRounding,10.0f);
            igPushStyleVar_Float(ImGuiStyleVar_FrameRounding,5.0f);
            igPushStyleColor_Vec4(ImGuiCol_WindowBg, frame_col);

            igBegin("baroPress", NULL,  ImGuiWindowFlags_NoResize |ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
            igText("BAROMETRIC PRESSURE CONVERSION");

            if (igBeginTabBar("tabs", ImGuiTabBarFlags_None))
            {
                // hpa -> inhg
                if (igBeginTabItem("hPa to inHg", NULL, ImGuiTabItemFlags_None))
                {
                    igText("Enter barometric pressure in hPa:");
                    igSameLine(300, 0);
                    igPushItemWidth(65);
                    igInputText("##hpa_pr", hpa, sizeof(hpa), ImGuiInputTextFlags_None, 0,0);
                    igPopItemWidth();
                    float press_out = hpa_to_inHg(atof(hpa));
                    igText("Barometric Pressure inHg: ");
                    if (strcmp(hpa, "") != 0)
                    {
                        igSameLine(305, 0);
                        igText("%.2f", press_out);
                    }
                    igEndTabItem();
                }


                // inhg -> hpa
                if (igBeginTabItem("inHg to hPa", NULL, ImGuiTabItemFlags_None))
                {
                    igText("Enter barometric pressure in inHg:");
                    igSameLine(300, 0);
                    igPushItemWidth(65);
                    igInputText("##inhg_pr", inhg, sizeof(inhg), ImGuiInputTextFlags_None, 0,0);
                    igPopItemWidth();
                    float press_out2 = inHg_to_hpa(atof(inhg));
                    igText("Barometric Pressure hPa: ");
                    if (strcmp(inhg, "") != 0)
                    {
                        igSameLine(305, 0);
                        igText("%.1f", press_out2);
                    }
                    igEndTabItem();
                }

            }
            igEndTabBar();
            igPopStyleVar(2);
            igPopStyleColor(1);
            igEnd();

            // fuel window
            igPushStyleVar_Float(ImGuiStyleVar_WindowRounding,10.0f);
            igPushStyleVar_Float(ImGuiStyleVar_FrameRounding,5.0f);
            igPushStyleColor_Vec4(ImGuiCol_WindowBg, frame_col);
            igBegin("Fuel Conversions", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

            igText("FUEL - WEIGHTS and VOLUMES");
            igNewLine();
            igText("Select Fuel Type:");
            igSameLine(200, 0);
            igPushItemWidth(200);
            if (igBeginCombo("Fuel Type", curr_fuel_type, 0))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool is_selected = (curr_fuel_type == fuel_types[i]);
                    if (igSelectable_Bool(fuel_types[i], is_selected, ImGuiSelectableFlags_None, temp_size))   // using here temp_size again since we want ot pass the same numbers
                        curr_fuel_type = fuel_types[i];
                    if (is_selected)
                        igSetItemDefaultFocus();
                }

                igEndCombo();
            }
            igPopItemWidth();

            igText("Select input:");
            igSameLine(200, 0);
            igPushItemWidth(200);
            if (igBeginCombo("Unit", curr_select_fuel_unit, ImGuiComboFlags_HeightSmall))
            {
                for (int j = 0; j < 4; j++)
                {
                    bool is_selected_fu = (curr_select_fuel_unit == select_fuel_unit[j]);
                    if (igSelectable_Bool(select_fuel_unit[j], is_selected_fu, ImGuiSelectableFlags_None, temp_size))
                        curr_select_fuel_unit = select_fuel_unit[j];
                    if (is_selected_fu)
                        igSetItemDefaultFocus();
                }
                igEndCombo();
            }
            igPopItemWidth();
            igPopStyleVar(2);
            igPopStyleColor(1);
            igEnd();


            // weather
            igPushStyleVar_Float(ImGuiStyleVar_WindowRounding,10.0f);
            igPushStyleVar_Float(ImGuiStyleVar_FrameRounding,5.0f);
            igPushStyleColor_Vec4(ImGuiCol_WindowBg, frame_col);
            igBegin("weather", NULL,  ImGuiWindowFlags_NoResize |ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
            igText("WEATHER");
            igPopStyleVar(2);
            igPopStyleColor(1);
            igEnd();





            // demo window
            static float f = 0.0f;
            static int counter = 0;

            igPushStyleVar_Float(ImGuiStyleVar_WindowRounding,10.0f);
            igBegin("Demo", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            igPushStyleVar_Float(ImGuiStyleVar_FrameRounding,5.0f);
            igText("DEMO");
            igSliderFloat("Float", &my_size, 0.0f, 1.0f, "%.3f", 0);
            igColorEdit3("clear color", (float *)&clearColor, 0);

            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            if (igButton("Button", buttonSize))
                counter++;
            igSameLine(0.0f, -1.0f);
            igText("counter = %d", counter);

            igText("Application average %.3f ms/frame (%.1f FPS)",
                   1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
            igPopStyleVar(2);
            igEnd();
        }

        if (showAnotherWindow)
        {
            igBegin("imgui Another Window", &showAnotherWindow, 0);
            igText("Hello from imgui");
            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            if (igButton("Close me", buttonSize)) {
                showAnotherWindow = false;
            }
            igEnd();

        }

        // render
        igRender();
        glfwMakeContextCurrent(window);
        glViewport(0, 0, (int)ioptr->DisplaySize.x, (int)ioptr->DisplaySize.y);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
#ifdef IMGUI_HAS_DOCK
        if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_window = glfwGetCurrentContext();
            igUpdatePlatformWindows();
            igRenderPlatformWindowsDefault(NULL, NULL);
            glfwMakeContextCurrent(backup_current_window);
        }
#endif
        glfwSwapBuffers(window);
    }

    // clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);

    glfwDestroyWindow(window);
    glfwTerminate();



    return 0;

}





