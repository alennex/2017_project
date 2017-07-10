const int VIEWPORT_WIDTH = 640;  
const int VIEWPORT_HEIGHT = 480;  
const int KEY_ESCAPE = 27;  

CvCapture* g_Capture;  
IplImage* image;  
GLint g_hWindow;  

GLvoid InitGL();  
GLvoid OnDisplay();  
GLvoid OnReshape(GLint w, GLint h);  
GLvoid OnKeyPress (unsigned char key, GLint x, GLint y);  

int main(int argc, char* argv[])  
{  
    glutInit(&argc, argv);  
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);  
    glutInitWindowPosition(100, 100);  
    glutInitWindowSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);   

    g_hWindow = glutCreateWindow("Image");   

    image = cvLoadImage("average.jpg", 1);  
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);   

    InitGL();   

    glutMainLoop();   

    return 0;   
}   

GLvoid InitGL()  
{  
    glClearColor (0.0, 0.0, 0.0, 0.0);    

    glutDisplayFunc(OnDisplay);    
    glutReshapeFunc(OnReshape);    
    glutKeyboardFunc(OnKeyPress);    
}    

GLvoid OnDisplay(void)    
{    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    glEnable(GL_TEXTURE_2D);  

    glPushMatrix();  
        glTranslatef(0.0f, 0.0f, -2.5f);  

        glBegin(GL_QUADS);  

            glTexCoord2f(0.0f, 0.0f);  
            glVertex3f(-1.0f,-1.0f, 1.0f);    

            glTexCoord2f(1.0f, 0.0f);  
            glVertex3f( 1.0f,-1.0f, 1.0f);  

            glTexCoord2f(1.0f, 1.0f);  
            glVertex3f( 1.0f, 1.0f, 1.0f);  

            glTexCoord2f(0.0f, 1.0f);  
            glVertex3f(-1.0f, 1.0f, 1.0f);  

        glEnd();  
    glPopMatrix();  

    glutSwapBuffers();  
}  

GLvoid OnReshape(GLint width, GLint height)  
{  
    if (height==0)  
        height=1;  

    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();  

    glViewport(0, 0, width, height);  
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 1.0f, 10.0f);  

    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity();  
    gluLookAt( 0.0, 0.0, 0.0, 0.0, 0.0, -6.0, 0.0f, 1.0f, 0.0f);  
}  

GLvoid OnKeyPress(unsigned char key, int x, int y)  
{  
    switch (key) {  
        case KEY_ESCAPE:  
            cvReleaseImage(&image);   
            glutDestroyWindow(g_hWindow);  
            exit(0);  
            break;  
    }   
    glutPostRedisplay();  
}  