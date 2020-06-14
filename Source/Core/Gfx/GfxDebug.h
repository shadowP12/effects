#pragma once
#include "../Base.h"
#include "Gfx.h"
#include <map>
#include <string>
EFFECTS_NAMESPACE_BEGIN

#define MAX_GL_POINTS 2048
#define MAX_GL_LINES 2048

    class DebugPoints
    {
    public:
        DebugPoints();
        ~DebugPoints();
        void draw();
        void addPoint(float* point_xyz, float* colour_rgba);
    private:
        GLuint m_vao, m_vbo;
        uint32_t m_points_count = 0;//当前需要绘制点的数量
    };

    class DebugLines
    {
    public:
        DebugLines();
        ~DebugLines();
        void draw();
        void addLine(float* start_xyz, float* end_xyz, float* colour_rgba);
    private:
        GLuint m_vao, m_vbo;
        uint32_t m_lines_count = 0;//当前需要绘制线的数量
    };

EFFECTS_NAMESPACE_END
