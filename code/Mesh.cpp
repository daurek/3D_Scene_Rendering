// Header
#include "Mesh.hpp"

// System
#include <cassert>
#include <iostream>
//#include <math.h>

// Libraries
#include "tiny_obj_loader.h"
#include "Matrix.hpp"

// Project
#include "View.hpp"

namespace example
{
	Mesh::Mesh(const std::string & obj_file_path, Translation3f position, Scaling3f scale, Color color) : translation(position), scaling(scale)
	{
		tinyobj::attrib_t					attributes;
		std::vector< tinyobj::shape_t    >	shapes;
		std::vector< tinyobj::material_t >	materials;
		std::string warn;
		std::string error;

		std::cout << "\n Loading " << obj_file_path << " ... \n";

		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &error, obj_file_path.c_str()))
		{
			std::cout << "There is no obj"; return;
		}

		if (shapes.size()				== 0)	{ std::cout << "There is no shapes";	return; }
		if (attributes.vertices.size()	== 0)	{ std::cout << "There is no vertices";	return; }
		if (attributes.normals.size()	== 0)	{ std::cout << "There is no normals";	return; }

		// Vertex data
		const std::vector<tinyobj::real_t> vertices = attributes.vertices;

		size_t number_of_vertices = vertices.size() / 3;
		std::cout << "	Verts: " << number_of_vertices << std::endl;

		original_vertices.resize(number_of_vertices);

		for (size_t index = 0, vertex_counter = 0; index < number_of_vertices; index++, vertex_counter += 3)
		{
			
			original_vertices[index] = Vertex({ vertices[vertex_counter],vertices[vertex_counter + 1],vertices[vertex_counter + 2],1 });
		}

		transformed_vertices.resize(number_of_vertices);
		display_vertices.resize(number_of_vertices);

		// Vertex color and normal data   
		const std::vector<tinyobj::real_t> colors = attributes.colors;
		const std::vector<tinyobj::real_t> normals = attributes.normals;

		size_t number_of_colors = colors.size() / 3;
		size_t number_of_normals = normals.size() / 3;
		std::cout << "	Colors: " << number_of_colors << std::endl;
		std::cout << "	Normals: " << number_of_normals << std::endl;

		assert(number_of_colors == number_of_vertices);

		original_colors.resize(number_of_colors * 3);
		original_normals.resize(number_of_normals * 3);

		//Vertex light_dir({std::sqrt(5)});
		Vertex light_dir({ 1,1,0 });
		Vertex light_color({255,0,0});

		for (size_t index = 0, color_counter = 0; index < number_of_colors; index++, color_counter += 3)
		{
			//original_normals[index] = Vertex({ normals[color_counter + 0], normals[color_counter + 1], normals[color_counter + 2] });
			//original_colors[index].set (colors[color_counter], colors[color_counter+1], colors[color_counter+2]);
			float intensity = (normals[color_counter + 0] * light_dir[0] + normals[color_counter + 1] * light_dir[1] + normals[color_counter + 2] * light_dir[2]) * 1.f;
			if (intensity < 0)  intensity = 0;
			
			//riginal_colors[index].set(200 * attributes.normals[color_counter + 0], 185 * attributes.normals[color_counter + 1], 155 * attributes.normals[color_counter + 2] );
			//original_colors[index].set(200 * intensity, 185 * intensity, 155 * intensity);
			//original_colors[index].set (colors[color_counter], colors[color_counter+1], colors[color_counter+2]);
			original_colors[index]= color.data.value * intensity;

		}

		// Triangle data
		const tinyobj::mesh_t triangles = shapes[0].mesh;

		size_t number_of_triangles = triangles.indices.size() / 3;
		std::cout << "	Triangles: " << number_of_triangles << std::endl;

		original_indices.resize(number_of_triangles * 3);

		Index_Buffer::iterator indices_iterator = original_indices.begin();

		for (size_t triangle_index = 0, triangle_counter = 0; triangle_index < number_of_triangles; triangle_index++, triangle_counter += 3)
		{
			*indices_iterator++ = triangles.indices[triangle_counter + 0].vertex_index;
			*indices_iterator++ = triangles.indices[triangle_counter + 1].vertex_index;
			*indices_iterator++ = triangles.indices[triangle_counter + 2].vertex_index;
		}

	}

	void Mesh::Update()
	{
		/*float angle = 0.f;

		angle += 0.025f;*/

		//rotation_x.set< Rotation3f::AROUND_THE_X_AXIS >(0.50f);
		//rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(0.1f);

		// Creación de la matriz de transformación unificada:
		//transformation = projection * translation * rotation_x * rotation_y * scaling;


		// Se transforman todos los vértices usando la matriz de transformación resultante:

		for (size_t index = 0, number_of_vertices = original_vertices.size(); index < number_of_vertices; index++)
		{
			// Se multiplican todos los vértices originales con la matriz de transformación y
			// se guarda el resultado en otro vertex buffer:

			Vertex & vertex = transformed_vertices[index] = Matrix44f(transformation) * Matrix41f(original_vertices[index]);

			// La matriz de proyección en perspectiva hace que el último componente del vector
			// transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:

			float divisor = 1.f / vertex[3];

			vertex[0] *= divisor;
			vertex[1] *= divisor;
			vertex[2] *= divisor;
			vertex[3] = 1.f;
		}
	}

	bool drawn = false;
	void Mesh::Render(Rasterizer< Color_Buffer > & rasterizer)
	{
		for (int * indices = original_indices.data(), *end = indices + original_indices.size(); indices < end; indices += 3)
		{
			if (View::is_frontface(transformed_vertices.data(), indices))
			{
				//auto intensity = original_normals[*indices][0] * rasterizer.light.position[0] + original_normals[*indices][1] * rasterizer.light.position[1] + original_normals[*indices][2] * rasterizer.light.position[2];//+ original_normals[(*indices)].coordinates[1] * rasterizer.light.position.coordinates[1] + original_normals[(*indices)].coordinates[2] * rasterizer.light.position.coordinates[2]) * 0.5f;
				//if (intensity < 0)  intensity = 0;
				// Se establece el color del polígono a partir del color de su primer vértice:
			//	original_colors[*indices].set(155 * intensity, 155 * intensity, 155 * intensity);// *= intensity;
				rasterizer.set_color(original_colors[*indices]);
				// Se rellena el polígono:
				rasterizer.fill_convex_polygon_z_buffer(display_vertices.data(), indices, indices + 3);
				//if(!drawn) std::cout << display_vertices[*indices][1] << std::endl;
			}
		}
		drawn = true;
	}
}
