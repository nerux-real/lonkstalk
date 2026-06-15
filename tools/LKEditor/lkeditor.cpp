#include "deps/imgui.h"
#include "deps/backends/imgui_impl_sdl2.h"
#include "deps/backends/imgui_impl_sdlrenderer2.h"

#include <SDL.h>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "LKEditor",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1920,
        1080,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    // imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // ui scaling
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.5f;
    ImGui::GetStyle().ScaleAllSizes(1.5f);

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    bool running = true;
    SDL_Event event;

    // app loop
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
                running = false;
        }

        // imgui frame init
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // top menu bar
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Project"))
                {
                    // TODO
                }

                if (ImGui::MenuItem("Open"))
                {
                    // TODO
                }

                if (ImGui::MenuItem("Save"))
                {
                    // TODO
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Exit"))
                {
                    running = false;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::MenuItem("Undo");
                ImGui::MenuItem("Redo");
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // main editor window
        ImGui::Begin("Beatmap");

        ImGui::Text("Song: none");
        ImGui::Text("Notes: 0");

        if (ImGui::Button("Play"))
        {
        }

        ImGui::SameLine();

        if (ImGui::Button("Stop"))
        {
        }

        ImGui::End();

        // render
        ImGui::Render();

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        ImGui_ImplSDLRenderer2_RenderDrawData(
            ImGui::GetDrawData(),
            renderer
        );

        SDL_RenderPresent(renderer);
    }

    // shutdown
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
