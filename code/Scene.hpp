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

		/// Scene Meshes map
		std::map< std::string, std::shared_ptr< Mesh >> meshes;

		Mesh * parent;
		std::vector<Mesh> children;

    public:

		/// Loads the given xml after creating the color buffer
        Scene(size_t width, size_t height, const std::string & sceneContentXML);

		/// Updates every mesh
        void Update ();

		/// Renders every mesh
        void Render  ();

		/// Loads the xml path  
		void LoadScene(const std::string & file);

		/// Loads the meshes node creating every Mesh
		void LoadMeshes(rapidxml::xml_node<> * meshesNode);

		/// Loads the given mesh
		std::shared_ptr< Mesh > LoadMesh(rapidxml::xml_node<> * meshNode);

		/// Scene camera
		std::shared_ptr< Camera > camera;

        Rasterizer< Color_Buffer > rasterizer;
    };
}

#endif
