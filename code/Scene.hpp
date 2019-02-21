#ifndef SCENE_HEADER
#define SCENE_HEADER

// System
#include <vector>
#include <map>

// Libraries
#include "Point.hpp"
#include "rapidxml.hpp"
// Project
#include "Rasterizer.hpp"
#include "Color_Buffer_Rgb565.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"

namespace example
{

    using toolkit::Point4f;

    class Scene
    {
    private:

        typedef Color_Buffer_Rgba8888 Color_Buffer;
        typedef Point4f               Vertex;

        size_t width;
        size_t height;

        Color_Buffer               colorBuffer;

		std::map< std::string, std::shared_ptr< Mesh >> meshes;
		
    public:

        Scene(size_t width, size_t height, const std::string & sceneContentXML);

        void Update ();

        void Render  ();

		void LoadScene(const std::string & file);

		void LoadMeshes(rapidxml::xml_node<> * meshNode);

		std::shared_ptr< Camera > camera;

        Rasterizer< Color_Buffer > rasterizer;
    };

}

#endif
