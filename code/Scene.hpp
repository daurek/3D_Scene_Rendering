/// ----------------------------------------------------------------------------------------------------------------------
/// 3D SCENE RENDERING
/// \class renderscene::Scene
///
/// \author Ilyass Sofi Hlimi
/// \date 06/03/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

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

namespace renderscene
{
	/// Renders and Updates a list of Mesh
    class Scene
    {
    private:

        typedef Color_Buffer_Rgba8888 Color_Buffer;
		typedef Point4f               Vertex;

        size_t width;
        size_t height;

        Color_Buffer               colorBuffer;

		/// Scene Mesh map
		std::map< string, shared_ptr< Mesh >> meshes;

    public:

		/// Loads the given xml after creating the color buffer
        Scene(size_t width, size_t height, const string & sceneContentXML);

		/// Updates every Mesh
        void Update ();

		/// Renders every Mesh
        void Render  ();

		/// Loads the xml path  
		void LoadScene(const string & file);

		/// Loads the meshes node creating every Mesh
		void LoadMeshes(rapidxml::xml_node<> * meshesNode);

		const std::map< string, shared_ptr< Mesh >> & getMeshesMap() { return meshes; }

		/// Returns the loaded Mesh with the given path
		shared_ptr< Mesh > LoadMesh(rapidxml::xml_node<> * meshNode);

		/// Scene camera
		shared_ptr< Camera > camera;

		/// Rasterizer reference used to render
        Rasterizer< Color_Buffer > rasterizer;
    };
}

#endif
