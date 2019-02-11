
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on december of 2013                                       *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef VIEW_HEADER
#define VIEW_HEADER

// System
#include <vector>

// Libraries
#include "Point.hpp"

// Project
#include "Rasterizer.hpp"
#include "Color_Buffer_Rgb565.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include "Mesh.hpp"

namespace example
{

    using std::vector;
    using toolkit::Point4f;

    class View
    {
    private:

        typedef Color_Buffer_Rgba8888 Color_Buffer;
        typedef Point4f               Vertex;

    private:

        size_t width;
        size_t height;

        Color_Buffer               Color_buffer;
        Rasterizer< Color_Buffer > rasterizer;

		vector< Mesh >	  meshes;

    public:

        View(size_t width, size_t height);

        void Update ();

        void Render  ();

        static bool is_frontface (const Vertex * const projected_vertices, const int * const indices);

    };

}

#endif
