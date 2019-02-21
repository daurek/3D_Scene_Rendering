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

			Mesh(const std::string & obj_file_path, Translation3f position, Scaling3f scale, Color color, bool isStatic);

			void Update(Scene * scene);

			void Render(Scene * scene);

			bool IsFrontface(const Vertex * const projected_vertices, const int * const indices);

			// Mesh data
			Vertex_Buffer		original_vertices;
			Index_Buffer		original_indices;
			Vertex_Colors		original_colors;
			Vertex_Buffer		original_normals;
			Vertex_Buffer		transformed_vertices;
			Vertex_Colors		transformed_colors;
			vector< Point4i >	display_vertices;
			Color				meshColor;
			bool				staticMesh;

			// Transform matrices
			Translation3f		translation;
			Rotation3f			rotation_x;
			Rotation3f			rotation_y;
			Scaling3f			scaling;
			Transformation3f	transformation;
	};

		

}

#endif