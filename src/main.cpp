#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#include <GL/glew.h>     
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <algorithm>

SDL_Window* window;
SDL_GLContext gl_context;

void Init();

void Close();





std::string exec(std::string command) {
   char buffer[128];
   std::string result = "";

   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }

   // read till end of process:
   while (!feof(pipe)) {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
   return result;
}



int main() {
    Init();

    int startTime = 0;
    int endTime = 0;

    bool ifInput = false;
    bool ifOutput = false;


    std::string inputFile;
    std::string outputFile;
    std::string outputSentence;

    int clipLength;



    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        {

            ImGui::Begin("Light Video Editor");                      

            ImGui::Text("Please input a file");        
        

            if (ImGui::Button("Select Input File")) {          
                ifInput = true;              
                inputFile = exec("zenity --file-selection");
                std::string temp = std::string(" 2>&1 | grep ") +  
                        std::string(" \"Duration\"| cut -d ") +
                        std::string(" ' ' -f 4 | sed s/,// | sed 's@\\..*@@g' ") +
                        std::string(" | awk '{ split($1, A, \":\"); split(A[3], ") +
                        std::string(" B, \".\"); print 3600*A[1] + 60*A[2] + B[1] }'");
                std::cout << std::string("ffmpeg -i ") + inputFile + temp << std::endl;
                

                std::string length = exec(std::string("ffmpeg -i ") + inputFile + temp);
                std::cout << length;
                clipLength = std::stoi(length);
                
                
            }

            if (ifInput) {
                if (ImGui::Button("Select Output directory")) {    
                    ifOutput = true;                     
                    outputFile = exec("zenity --file-selection --directory");
                    outputFile += "/";
                    outputFile += exec("zenity --entry --text \"specify output name\"");
                }

                std::string intext = "Start, Video Length: " + std::to_string(clipLength);
                std::string outtext = "End, Video Length: " + std::to_string(clipLength);
                ImGui::SliderInt(intext.c_str(), &startTime, 0, clipLength);
                ImGui::SliderInt(outtext.c_str(), &endTime, 0, clipLength);
            }

            

            if (ImGui::Button("Confirm")) {
                if (ifOutput && ifOutput)
                {
                    std::string to_peg = std::string("ffmpeg -i ") + inputFile +
                        " -ss " + std::to_string(startTime) + std::string(" -to ") + 
                        std::to_string(endTime) + std::string(" ") + outputFile;
                    to_peg += " | zenity --progress --pulsate --auto-close";
                    //popen(ffmpeg_open, "r");
                    exec(to_peg);
                    std::string out = "ffplay -autoexit " + outputFile;
                    exec(out);
                    done = true;
                } else {
                    exec("zenity --error --text \"Not Output File\"");
                }
        
            }

            ImGui::End();
        }

        

        // Rendering
        ImGui::Render();
        

        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
        SDL_Delay(30);
    }

    Close();


    return 0;
}

void Init() {
    // initialize sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    
    // Create window with graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    window = SDL_CreateWindow("ffmpeg_gui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2000, 1080, window_flags);
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    glewInit();

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    const char* glsl_version = "#version 150";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void Close() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}