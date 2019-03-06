/// ----------------------------------------------------------------------------------------------------------------------
/// 3D SCENE RENDERING
/// \class renderscene::Mesh
///
/// \author Ilyass Sofi Hlimi
/// \date 06/03/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef MESH_HEADER
#define MESH_HEADER
#define TINYOBJLOADER_IMPLEMENTATION

// System
#include <string>

// Libraries
#include "Point.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include "Translation.hpp"
#include "Rotation.hpp"
#include "Scaling.hpp"
// Project
#include "Rasterizer.hpp"
#include "Camera.hpp" 

namespace renderscene
{
	using std::vector;
	using std::shared_ptr;
	using std::string;
	
	using toolkit::Point4i;
	using toolkit::Point4f;
	using toolkit::Matrix44f;
	using toolkit::Matrix41f;
	using toolkit::Translation3f;
	using toolkit::Rotation3f;
	using toolkit::Scaling3f;
	using toolkit::Transformation3f;

	class Scene;

	/// Vertex data container that is able to render and transform in a Scene
	class Mesh
	{
		private:

			typedef Color_Buffer_Rgba8888 Color_Buffer;
			typedef Color_Buffer::Color   Color;
			typedef Point4f               Vertex;
			typedef vector< Vertex >      Vertex_Buffer;
			typedef vector< int    >      Index_Buffer;
			typedef vector< Color  >      Vertex_Colors;

		public:

			/// Loads the given obj file path into the given position and scale with the given color
			Mesh(const std::string & objFilePath, Translation3f position, Scaling3f scale, Color color);

			/// Updates the Mesh calculating every vertex position and light with the given projection
			void Update(Scene * scene, Transformation3f parentTransform = Transformation3f());

			/// Renders the Mesh with the given rasterizer
			void Render(Scene * scene);

			/// Checks if a face is in the front
			bool IsFrontface(const Vertex * const projected_vertices, const int * const indices);

			/// Adds a child to the children list and sets it's parent to this object
			void AddChild(shared_ptr<Mesh> newChild)
			{
				newChild->meshParent = this;
				meshesChildren.push_back(newChild);
			}

			// ____________________________________ Transform Matrices
			Translation3f		translation;
			Rotation3f			rotation_x;
			Rotation3f			rotation_y;
			Scaling3f			scaling;
			Transformation3f	transformation;

	private:

			// ____________________________________ Mesh Data
			/// List of vertex positions
			Vertex_Buffer		originalVertices;
			/// List of indices of each triangle
			Index_Buffer		originalIndices;
			/// List of vertex colors
			Vertex_Colors		originalColors;
			/// List of vertex normals
			Vertex_Buffer		originalNormals;
			/// List of transformed vertex positions [AFTER TRANSFORMING]
			Vertex_Buffer		transformedVertices;
			/// List of transformed vertex colors	 [AFTER LIGHTING]
			Vertex_Colors		transformedColors;
			/// List of transformed by camera vertex positions
			vector< Point4i >	displayVertices;
			/// Caching the color of the mesh :)
			Color				meshColor;

			// ____________________________________ Hierarchy Data	
			Mesh *								meshParent;
			vector< shared_ptr<Mesh> >		meshesChildren;

	};

		

}

#endif