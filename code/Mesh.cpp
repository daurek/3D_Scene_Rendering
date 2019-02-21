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
#include "Scene.hpp"

namespace example
{
	Mesh::Mesh(const std::string & objFilePath, Translation3f position, Scaling3f scale, Color color, bool isStatic) : translation(position), scaling(scale), meshColor(color), staticMesh(isStatic)
	{
		tinyobj::attrib_t					attributes;
		std::vector< tinyobj::shape_t    >	shapes;
		std::vector< tinyobj::material_t >	materials;
		std::string warn;
		std::string error;

		std::cout << "\n\n	Loading " << objFilePath << " ... \n";

		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &error, objFilePath.c_str()))
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

		original_colors.resize(number_of_colors);
		original_normals.resize(number_of_normals);
		transformed_colors.resize(original_colors.size());

		for (size_t index = 0, color_counter = 0; index < number_of_colors; index++, color_counter += 3)
		{
			original_colors[index] = meshColor;
			original_normals[index] = Vertex({ normals[color_counter + 0],normals[color_counter + 1],normals[color_counter + 2], 0 });


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

	void Mesh::Update(Scene * scene)
	{
		static float angle = 0.f;
		angle += 0.01f;

		//rotation_x.set< Rotation3f::AROUND_THE_X_AXIS >(0.50f);
		rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(angle);

		// Creación de la matriz de transformación unificada:
		transformation = translation * rotation_x * rotation_y * scaling;
		// Se transforman todos los vértices usando la matriz de transformación resultante:
		for (size_t index = 0, number_of_vertices = original_vertices.size(); index < number_of_vertices; index++)
		{
			Vertex transformedVertex = Matrix44f(transformation) * Matrix41f(original_vertices[index]);
			Vertex transformedNormal = Matrix44f(transformation) * Matrix41f(original_normals[index]);
			// Se multiplican todos los vértices originales con la matriz de transformación y
			// se guarda el resultado en otro vertex buffer:

			float normalMagnitude = std::sqrt(std::powf(transformedNormal[0], 2) + std::powf(transformedNormal[1], 2) + std::powf(transformedNormal[2], 2));
			Vertex normalNormalized({ transformedNormal[0] / normalMagnitude,transformedNormal[1] / normalMagnitude,transformedNormal[2] / normalMagnitude });

			float lightMagnitude = std::sqrt(std::powf(scene->rasterizer.light.position[0], 2) + std::powf(scene->rasterizer.light.position[1], 2) + std::powf(scene->rasterizer.light.position[2], 2));
			Vertex lightNormalized({ scene->rasterizer.light.position[0] / lightMagnitude, scene->rasterizer.light.position[1] / lightMagnitude, scene->rasterizer.light.position[2] / lightMagnitude });

			float intensity = (normalNormalized[0] * lightNormalized[0] + normalNormalized[1] * lightNormalized[1] + normalNormalized[2] * lightNormalized[2]) * 1.f;
			intensity = std::max(intensity, 0.2f);

			transformed_colors[index].set_clamped(static_cast<int>(meshColor.data.component.r * intensity), static_cast<int>(meshColor.data.component.g * intensity), static_cast<int>(meshColor.data.component.b * intensity));

			Vertex & vertex = transformed_vertices[index] = Matrix44f(scene->camera->projection) * Matrix41f(transformedVertex);

			// La matriz de proyección en perspectiva hace que el último componente del vector
			// transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:

			float divisor = 1.f / vertex[3];

			vertex[0] *= divisor;
			vertex[1] *= divisor;
			vertex[2] *= divisor;
			vertex[3] = 1.f;
		}
	}

	void Mesh::Render(Scene * scene)
	{
		Matrix44f cameraTransform = Matrix44f(scene->camera->getTransformation());

		for (size_t index = 0, number_of_vertices = transformed_vertices.size(); index < number_of_vertices; index++)
		{
			display_vertices[index] = Point4i(cameraTransform * Matrix41f(transformed_vertices[index]));
		}

		for (int * indices = original_indices.data(), *end = indices + original_indices.size(); indices < end; indices += 3)
		{
			if (IsFrontface(transformed_vertices.data(), indices))
			{
				scene->rasterizer.set_color(transformed_colors[*indices]);
				// Se rellena el polígono:
				scene->rasterizer.fill_convex_polygon_z_buffer(display_vertices.data(), indices, indices + 3);
			}
		}
	}

	bool Mesh::IsFrontface(const Vertex * const projected_vertices, const int * const indices)
	{
		const Vertex & v0 = projected_vertices[indices[0]];
		const Vertex & v1 = projected_vertices[indices[1]];
		const Vertex & v2 = projected_vertices[indices[2]];

		// Se asumen coordenadas proyectadas y polÃ­gonos definidos en sentido horario.
		// Se comprueba a quÃ© lado de la lÃ­nea que pasa por v0 y v1 queda el punto v2:

		return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
	}


}
