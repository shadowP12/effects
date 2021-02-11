#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

unsigned int getGLFormatStride(unsigned int format);

unsigned int getGLFormat(unsigned int format);

unsigned int getGLComponentType(unsigned int format);