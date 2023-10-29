//
// Created by ilias on 8/9/23.
//


#include "imgui_main.h"

extern float my_size;

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int , char **)
{

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return -1;


#if __APPLE__
    // GL 3.2 Core + GLSL 150
  const char *glsl_version = "#version 150";
#else
    // GL 3.2 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // first winow
    GLFWwindow *window = glfwCreateWindow(1024, 768, "Aviator's Companion", nullptr, nullptr);
    if (window== nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    // enable vsync
    glfwSwapInterval(1);



    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io =  ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Custom Font?
//    ImFontAtlas_AddFontFromFileTTF(ioptr->Fonts,"../fonts/SVBasicManual.ttf", 20, NULL, NULL);
    io.Fonts->AddFontFromFileTTF("/home/ilias/Programming/C/aviators_companion/fonts/Cabin-Regular.otf", 20);
    //ImFontAtlas_AddFontFromFileTTF(igGetIO()->Fonts, "fonts/Sono_Regular.ttf", 20, NULL, NULL);


    bool showDemoWindow = false;
    bool showAnotherWindow = false;

    ImVec4 clear_color = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
//    ImGui::ColorEdit3("clear color", (float*) &clear_color);

    ImVec4 frame_col = ImVec4(0.15f, 0.15f, 0.15f, 0.5f);
//    ImGui::ColorEdit3("frame color", (float*) &frame_col);

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
    char arpt[5];

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
        ImGui::NewFrame();

        if (showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);

        // show a simple window that we created ourselves.
        {

            // Temp Conv
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,10.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.0f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, frame_col);

            const char * temps = "TEMPERATURE CONVERSION";
            ImGui::Begin(temps, NULL,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
//            ImGui::Text("TEMPERATURE CONVERSION");
            if (ImGui::BeginTabBar("tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Celsius to Fahreneit", NULL, ImGuiTabItemFlags_None))
                {
                    ImGui::NewLine();
                    ImGui::Text("Enter Temp in Celsius:");
                    ImGui::SameLine(0, 30);
                    ImGui::PushItemWidth(65);
                    ImGui::InputText("##c_temp", celsius, sizeof(celsius), ImGuiInputTextFlags_None, 0,0);
                    ImGui::PopItemWidth();

                    float c_out = temp_c_to_f(atof(celsius));
                    ImGui::Text("Fahrenheit: ");
                    if (strcmp(celsius, "") != 0)   // show output value only if user has enter some value
                    {
                        ImGui::SameLine(0, 105);
                        ImGui::Text("%.1f", c_out);
                    }
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Fahreneit to Celsius", NULL, ImGuiTabItemFlags_None))
                {
                    ImGui::NewLine();
                    ImGui::Text("Enter Temp in Fahrenheit:");
                    ImGui::SameLine(0, 10);
                    ImGui::PushItemWidth(65);
                    ImGui::InputText("##f_temp", fahren, sizeof(fahren), ImGuiInputTextFlags_None, 0,0);
                    ImGui::PopItemWidth();
                    float f_out = temp_f_to_c(atof(fahren));
                    ImGui::Text("Celsius: ");
                    if (strcmp(fahren, "") != 0)    // show output value only if user has enter some value
                    {
                        ImGui::SameLine(0, 137);
                        ImGui::Text("%.1f", f_out);
                    }

                    ImGui::EndTabItem();
                }
            }

            ImGui::EndTabBar();
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(1);
            ImGui::End();


            // Baro pressure window
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,10.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.0f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, frame_col);

            ImGui::Begin("BAROMETRIC PRESSURE CONVERSION", NULL,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
//            ImGui::Text("BAROMETRIC PRESSURE CONVERSION");

            if (ImGui::BeginTabBar("tabs", ImGuiTabBarFlags_None))
            {
                // hpa -> inhg
                if (ImGui::BeginTabItem("hPa to inHg", NULL, ImGuiTabItemFlags_None))
                {
                    ImGui::Text("Enter barometric pressure in hPa:");
                    ImGui::SameLine(300, 0);
                    ImGui::PushItemWidth(65);
                    ImGui::InputText("##hpa_pr", hpa, sizeof(hpa), ImGuiInputTextFlags_None, 0,0);
                    ImGui::PopItemWidth();
                    float press_out = hpa_to_inHg(atof(hpa));
                    ImGui::Text("Barometric Pressure inHg: ");
                    if (strcmp(hpa, "") != 0)
                    {
                        ImGui::SameLine(305, 0);
                        ImGui::Text("%.2f", press_out);
                    }
                    ImGui::EndTabItem();
                }


                // inhg -> hpa
                if (ImGui::BeginTabItem("inHg to hPa", NULL, ImGuiTabItemFlags_None))
                {
                    ImGui::Text("Enter barometric pressure in inHg:");
                    ImGui::SameLine(300, 0);
                    ImGui::PushItemWidth(65);
                    ImGui::InputText("##inhg_pr", inhg, sizeof(inhg), ImGuiInputTextFlags_None, 0,0);
                    ImGui::PopItemWidth();
                    float press_out2 = inHg_to_hpa(atof(inhg));
                    ImGui::Text("Barometric Pressure hPa: ");
                    if (strcmp(inhg, "") != 0)
                    {
                        ImGui::SameLine(305, 0);
                        ImGui::Text("%.1f", press_out2);
                    }
                    ImGui::EndTabItem();
                }

            }
            ImGui::EndTabBar();
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(1);
            ImGui::End();

            // fuel window
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,10.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.0f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, frame_col);
            ImGui::Begin("FUEL - WEIGHTS and VOLUMES", NULL, ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_NoMove );

//            ImGui::Text("FUEL - WEIGHTS and VOLUMES");
//            ImGui::NewLine();
//            ImGui::Text("Select Fuel Type:");
//            ImGui::SameLine(200, 0);
            ImGui::PushItemWidth(200);
            if (ImGui::BeginCombo("Select Fuel Type", curr_fuel_type, 0))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool is_selected = (curr_fuel_type == fuel_types[i]);
                    if (ImGui::Selectable(fuel_types[i], is_selected, ImGuiSelectableFlags_None, temp_size))   // using here temp_size again since we want ot pass the same numbers
                        curr_fuel_type = fuel_types[i];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();

//            ImGui::Text("Select input:");
//            ImGui::SameLine(200, 0);
            ImGui::PushItemWidth(200);
            if (ImGui::BeginCombo("Select Unit", curr_select_fuel_unit, ImGuiComboFlags_HeightSmall))
            {
                for (int j = 0; j < 4; j++)
                {
                    bool is_selected_fu = (curr_select_fuel_unit == select_fuel_unit[j]);
                    if (ImGui::Selectable(select_fuel_unit[j], is_selected_fu, ImGuiSelectableFlags_None, temp_size))
                        curr_select_fuel_unit = select_fuel_unit[j];
                    if (is_selected_fu)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(1);
            ImGui::End();


            // weather
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,10.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.0f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, frame_col);
            ImGui::Begin("WEATHER", NULL,  ImGuiWindowFlags_NoResize / ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

            ImGui::Text("Enter Airport ICAO code:");
            ImGui::SameLine();
            ImGui::PushItemWidth(80);
            ImGui::InputText("##arpt", arpt, sizeof(arpt), ImGuiInputTextFlags_CharsUppercase, 0,0);
            ImGui::PopItemWidth();

            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(1);
            ImGui::End();





            // demo window
            static float f = 0.0f;
            static int counter = 0;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,10.0f);
            ImGui::Begin("Demo", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.0f);
            ImGui::Text("DEMO");
            ImGui::SliderFloat("Float", &my_size, 0.0f, 1.0f, "%.3f", 0);
            ImGui::ColorEdit3("clear color", (float *)&clear_color, 0);

            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            if (ImGui::Button("Button", buttonSize))
                counter++;
            ImGui::SameLine(0.0f, -1.0f);
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                   1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::PopStyleVar(2);
            ImGui::End();
        }

        if (showAnotherWindow)
        {
            ImGui::Begin("imgui Another Window", &showAnotherWindow, 0);
            ImGui::Text("Hello from imgui");
            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            if (ImGui::Button("Close me", buttonSize)) {
                showAnotherWindow = false;
            }
            ImGui::End();

        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//#ifdef IMGUI_HAS_DOCK
//        if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//        {
//            GLFWwindow *backup_current_window = glfwGetCurrentContext();
//            igUpdatePlatformWindows();
//            igRenderPlatformWindowsDefault(NULL, NULL);
//            glfwMakeContextCurrent(backup_current_window);
//        }
//#endif
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();



    return 0;

}





