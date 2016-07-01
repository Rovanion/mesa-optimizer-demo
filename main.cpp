/* Derivative of https://gist.github.com/dlivingstone/628320
 * Daniel Livingstone, October 2010.
 *
 * Modified by Rovanion Luckey, 2016
 */

#include <GL/glew.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


GLfloat vertices[] = { -1.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f };
GLfloat colours[] =  {  1.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 1.0f };
GLfloat vertices2[] = { 0.0f, 0.0f, 0.0f,
                        0.0f,-1.0f, 0.0f,
                        1.0f, 0.0f, 0.0f };

// two vertex array objects, one for each object drawn
unsigned int vertexArrayObjID[2];
// three vertex buffer objects in this example
unsigned int vertexBufferObjID[3];

// loadFile - loads text file into char* fname
// allocates memory - so need to delete after use
// size of file returned in fSize
char* loadFile(string fname, GLint &fSize) {
  ifstream::pos_type size;
  char * memblock;
  string text;

  // file read based on example in cplusplus.com tutorial
  ifstream file(fname, ios::in | ios::binary | ios::ate);
  if (file.is_open()) {
    size = file.tellg();
    fSize = (GLuint) size;
    memblock = new char [size];
    file.seekg (0, ios::beg);
    file.read (memblock, size);
    file.close();
    cout << "file " << fname << " loaded" << endl;
    text.assign(memblock);
  }
  else
  {
    cout << "Unable to open file " << fname << endl;
    exit(1);
  }
  return memblock;
}


// printShaderInfoLog
// From OpenGL Shading Language 3rd Edition, p215-216
// Display (hopefully) useful error messages if shader fails to compile
void printShaderInfoLog(GLint shader) {
  int infoLogLen = 0;
  int charsWritten = 0;
  GLchar *infoLog;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

  // should additionally check for OpenGL errors here

  if (infoLogLen > 0) {
    infoLog = new GLchar[infoLogLen];
    // error check for fail to allocate memory omitted
    glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
    cout << "InfoLog:" << endl << infoLog << endl;
    delete [] infoLog;
  }

  // should additionally check for OpenGL errors here
}


void init(void) {
  // Would load objects from file here - but using globals in this example

  // Allocate Vertex Array Objects
  glGenVertexArrays(2, &vertexArrayObjID[0]);
  // Setup first Vertex Array Object
  glBindVertexArray(vertexArrayObjID[0]);
  glGenBuffers(2, vertexBufferObjID);

  // VBO for vertex data
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
  glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // VBO for colour data
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[1]);
  glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colours, GL_STATIC_DRAW);
  glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  // Setup second Vertex Array Object
  glBindVertexArray(vertexArrayObjID[1]);
  glGenBuffers(1, &vertexBufferObjID[2]);

  // VBO for vertex data
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[2]);
  glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
  glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}



void initShaders(void) {
  GLuint p, f, v;

  char *vs, *fs;

  v = glCreateShader(GL_VERTEX_SHADER);
  f = glCreateShader(GL_FRAGMENT_SHADER);

  // load shaders & get length of each
  GLint vlen;
  GLint flen;
  vs = loadFile("minimal.vert", vlen);
  fs = loadFile("minimal.frag", flen);

  const char* vv = vs;
  const char* ff = fs;

  glShaderSource(v, 1, &vv, &vlen);
  glShaderSource(f, 1, &ff, &flen);

  GLint compiled;

  glCompileShader(v);
  glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    cout << "Vertex shader not compiled." << endl;
    printShaderInfoLog(v);
  }

  glCompileShader(f);
  glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    cout << "Fragment shader not compiled." << endl;
    printShaderInfoLog(f);
  }

  p = glCreateProgram();

  glBindAttribLocation(p, 0, "in_Position");
  glBindAttribLocation(p, 1, "in_Color");

  glAttachShader(p, v);
  glAttachShader(p, f);

  glLinkProgram(p);
  glUseProgram(p);

  delete [] vs; // dont forget to free allocated memory
  delete [] fs; // we allocated this in the loadFile function...
}

void display(void) {
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(vertexArrayObjID[0]);   // First VAO
  glDrawArrays(GL_TRIANGLES, 0, 3);   // draw first object

  glBindVertexArray(vertexArrayObjID[1]);     // select second VAO
  glVertexAttrib3f((GLuint)1, 1.0, 0.0, 0.0); // set constant color attribute
  glDrawArrays(GL_TRIANGLES, 0, 3);   // draw second object

  glBindVertexArray(0);

  glutSwapBuffers();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

int main (int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Triangle Test");
  glewInit();
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    cout << "glewInit failed, aborting." << endl;
    exit (1);
  }
  cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
  cout << "OpenGL version " << glGetString(GL_VERSION) << " supported" << endl;

  init();
  initShaders();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0;

}
