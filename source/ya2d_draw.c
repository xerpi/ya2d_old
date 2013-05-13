#include "ya2d_draw.h"


    void ya2d_drawRect(int x, int y, int w, int h, u32 color)
    {
		sceGuDisable(GU_TEXTURE_2D);
        ya2d_FastVertex *vertices = (ya2d_FastVertex *)sceGuGetMemory(5 * sizeof(ya2d_FastVertex));

        vertices[0] = (ya2d_FastVertex){color, x, y};
        vertices[1] = (ya2d_FastVertex){color, x+w, y};
        vertices[2] = (ya2d_FastVertex){color, x+w, y+h};
        vertices[3] = (ya2d_FastVertex){color, x, y+h};
        vertices[4] = vertices[0];

        sceGuDrawArray(GU_LINE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 5, 0, vertices);

        sceKernelDcacheWritebackRange(vertices, 5 * sizeof(ya2d_FastVertex));
    }

    void ya2d_drawFillRect(int x, int y, int w, int h, u32 color)
    {
		sceGuDisable(GU_TEXTURE_2D);
        ya2d_FastVertex *vertices = (ya2d_FastVertex *)sceGuGetMemory(4 * sizeof(ya2d_FastVertex));

        vertices[0] = (ya2d_FastVertex){color, x, y};
        vertices[1] = (ya2d_FastVertex){color, x, y+h};
        vertices[2] = (ya2d_FastVertex){color, x+w, y};
        vertices[3] = (ya2d_FastVertex){color, x+w, y+h};

        sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 4, 0, vertices);

        sceKernelDcacheWritebackRange(vertices, 4 * sizeof(ya2d_FastVertex));
    }
