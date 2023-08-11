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

#include "main.h"
#include "conversions.h"
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

    // main

//    main_func();


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
    const char *glsl_version = "#version 130";
#endif

    // just an extra window hint for resize
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(1024, 768, "AVIATOR'S COMPANION", NULL, NULL);
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
    //ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
#ifdef IMGUI_HAS_DOCK
    ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
#endif

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    igStyleColorsDark(NULL);

    // Custom Font?
//    ImFontAtlas_AddFontFromFileTTF(ioptr->Fonts,"../fonts/SVBasicManual.ttf", 20, NULL, NULL);
    ImFontAtlas_AddFontFromFileTTF(ioptr->Fonts,"../fonts/Cabin-Regular.otf", 20, NULL, NULL);
    //ImFontAtlas_AddFontFromFileTTF(igGetIO()->Fonts, "fonts/Sono_Regular.ttf", 20, NULL, NULL);


    bool showDemoWindow = false;
    bool showAnotherWindow = false;
    ImVec4 clearColor;
    clearColor.x = 0.45f;
    clearColor.y = 0.55f;
    clearColor.z = 0.60f;
    clearColor.w = 1.00f;

    // main event loop | init variables here before the while loop
    bool quit = false;

    const char * temp_scale[] = { "Celsius to Fahrenheit", "Fahrenheit to Celsius"};
    static const char * curr_temp_scale = "Celsius to Fahrenheit";
    ImVec2 temp_size;
    temp_size.x = 0;
    temp_size.y = 0;
    char celsius[8];
    char fahren[8];


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
            static float f = 0.0f;
            static int counter = 0;
            igBegin("Aviator's Companion", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
            igText("Temperature Conversions");

            igPushItemWidth(250);
            if (igBeginCombo("##combo", curr_temp_scale, 0))
            {
                for (int n = 0; n < 2; n++)
                {
                    bool is_selected = (curr_temp_scale == temp_scale[n]);
                    if (igSelectable_Bool(temp_scale[n], is_selected, ImGuiSelectableFlags_None, temp_size))
                        curr_temp_scale = temp_scale[n];
                    if (is_selected)
                        igSetItemDefaultFocus();
                }
                igEndCombo();
            }
            igPopItemWidth();

            igPushItemWidth(65);
            if (strcmp(curr_temp_scale, "Celsius to Fahrenheit") == 0)
            {
                igText("Enter Temp in Celsius:");
                igSameLine(0, 30);
                igInputText("##c_temp", celsius, sizeof(celsius), ImGuiInputTextFlags_None, 0,0);
                float c_out = temp_c_to_f(atof(celsius));
                igText("Fahrenheit: ");
                if (strcmp(celsius, "") != 0)   // show output value only if user has enter some value
                {
                    igSameLine(0, 120);
                    igText("%.1f", c_out);
                }

            }
            else
            {
                igText("Enter Temp in Fahrenheit:");
                igSameLine(0, 10);
                igInputText("##f_temp", fahren, sizeof(fahren), ImGuiInputTextFlags_None, 0,0);
                float f_out = temp_f_to_c(atof(fahren));
                igText("Celsius: ");
                if (strcmp(fahren, "") != 0)    // show output value only if user has enter some value
                {
                    igSameLine(0, 135);
                    igText("%.1f", f_out);
                }

            }
            igPopItemWidth();



            igNewLine();
            igNewLine();
            igNewLine();
            igNewLine();
            igNewLine();
            igNewLine();
            igNewLine();
            igNewLine();
            igNewLine();
            igNewLine();
            igNewLine();
            igText("---------------------- Below here is the demo area!!! ----------------------");
            igNewLine();

//            igCheckbox("Demo window", &showDemoWindow);
//            igCheckbox("Another window", &showAnotherWindow);

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





