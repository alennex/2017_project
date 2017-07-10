// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <imgui.h>
#include "imgui_impl_sdl.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>

// ----------------------------------

CvCapture* g_Capture;  
IplImage* image;  
GLint g_hWindow;  

GLvoid InitGL();  
GLvoid OnDisplay();  
GLvoid OnReshape(GLint w, GLint h);  
GLvoid OnKeyPress (unsigned char key, GLint x, GLint y);

// --------------------------------------------------

using namespace cv;

void combine(Mat src1, Mat src2, Mat& dst);
    
int main(int argc, char** argv)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_Window *window = SDL_CreateWindow("Patch Image Analysist", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    // Setup ImGui binding
    ImGui_ImplSdl_Init(window);
    ImVec4 clear_color = ImColor(128, 128, 128);
    
    /* - open image- */
    Mat src = imread(argv[1],1), tmp;
 
    if(!src.data){
        printf("Image is not open!\n");
        exit(1);
    }

    /* - opencv Mat to opengl GLuint  -*/
    cvtColor(src, src, CV_BGR2RGB); // imgui show used RGB format
    GLuint id;
    glGenTextures(1, &id);                  // Create The Texture
    glBindTexture( GL_TEXTURE_2D, id);               
    glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, src.cols, src.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, src.data);

    Mat gSrc = Mat::zeros(Size(src.cols, src.rows),src.type());
    Mat dst = Mat::zeros(Size(src.cols*2, src.rows), src.type());
    static float f = 3.0, gSize=0.0;
    static bool do_gaussian = false;
        
    // Main loop
    bool done = false;

    while (!done)
    {

        // image inite
        gSrc = Mat::zeros(Size(src.cols, src.rows),src.type());
    
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdl_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }

        ImGui_ImplSdl_NewFrame(window);
 
        if (ImGui::Button("open_gaussian_filter"))
                do_gaussian = true;
        ImGui::SameLine();
        if (ImGui::Button("close_gaussian_filter"))
                do_gaussian = false;

        ImGui::SliderFloat("gaussian size", &f, 0.0f, 7.0f);
        gSize = (int)f*2+1;

        if(do_gaussian)
            GaussianBlur(src, gSrc, Size(gSize,gSize), 0, 0);

        combine(src, gSrc, dst);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, dst.cols, dst.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, dst.data);
        ImGui::Image((ImFontAtlas*)id,ImVec2((float)dst.cols, (float)dst.rows), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
     
        // Rendering
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(window);

    }

    // Cleanup
    ImGui_ImplSdl_Shutdown();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


void combine(Mat src1, Mat src2, Mat& dst){
    Mat left = Mat::zeros(Size(src1.cols, src1.rows),src1.type());
    Mat righ = Mat::zeros(Size(src2.cols, src2.rows),src2.type());

    left = dst(Rect(0,0,src1.cols,src1.rows));
    righ = dst(Rect(src1.cols,0,src2.cols,src2.rows));

    src1.copyTo(left);
    src2.copyTo(righ);
}

