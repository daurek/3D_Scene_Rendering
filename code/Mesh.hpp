#ifndef MESH_HEADER
#define MESH_HEADER
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

// System
#include <string>

// Libraries
#include "Point.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include "Translation.hpp"
#include "Rotation.hpp"
#include "Scaling.hpp"
#include "Rasterizer.hpp"

#include "Camera.hpp" 

namespace example
{
	using std::vector;
	using toolkit::Point4i;
	using toolkit::Point4f;
	using toolkit::Matrix44f;
	using toolkit::Matrix41f;
	using toolkit::Translation3f;
	using toolkit::Rotation3f;
	using toolkit::Scaling3f;
	using toolkit::Transformation3f;

	class Scene;

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

			Mesh(){}

			/// Loads the given obj file path into the given position and scale with the given color
			Mesh(const std::string & objFilePath, Translation3f position, Scaling3f scale, Color color);

			/// Updates the Mesh calculating every vertex position and light with the given projection
			void Update(Scene * scene, Transformation3f parentTransform = Transformation3f());

			/// Renders the Mesh with the given rasterizer
			void Render(Scene * scene, Transformation3f parentTransform = Transformation3f());

			/// Checks if a face is in the front
			bool IsFrontface(const Vertex * const projected_vertices, const int * const indices);

			// Mesh Data
			Vertex_Buffer		originalVertices;
			Index_Buffer		originalIndices;
			Vertex_Colors		originalColors;
			Vertex_Buffer		originalNormals;
			Vertex_Buffer		transformedVertices;
			Vertex_Colors		transformedColors;
			vector< Point4i >	displayVertices;
			Color				meshColor;

			// Transform matrices
			Translation3f		translation;
			Rotation3f			rotation_x;
			Rotation3f			rotation_y;
			Scaling3f			scaling;
			Transformation3f	transformation;

			// Hierarchy Data	
			Transformation3f	meshParent;
			vector< std::shared_ptr<Mesh> >		meshesChildren;
	};

		

}

#endif