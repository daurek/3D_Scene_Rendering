/// ----------------------------------------------------------------------------------------------------------------------
/// 3D SCENE RENDERING
/// \class renderscene::Mesh
///
/// \author Ilyass Sofi Hlimi
/// \date 06/03/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

// Header
#include "Mesh.hpp"

// System
#include <cassert>
#include <iostream>

// Libraries
#include "tiny_obj_loader.h"
#include "Matrix.hpp"
// Project
#include "Scene.hpp"

namespace renderscene
{

	using std::cout;

	Mesh::Mesh(const string & objFilePath, Translation3f position, Scaling3f scale, Color color) : translation(position), scaling(scale), meshColor(color)
	{
		// __________________________________________________________________ Mesh Data
		tinyobj::attrib_t					attributes;
		vector< tinyobj::shape_t    >	shapes;
		vector< tinyobj::material_t >	materials;
		string warn;
		string error;

		cout << "\n\n	Loading " << objFilePath << " ... \n";

		// __________________________________________________________________ Check validity
		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &error, objFilePath.c_str()))
		{
			cout << error << std::endl;
			return;
		}

		if (shapes.size()				== 0)	{ std::cout << "There is no shapes";	return; }
		if (attributes.vertices.size()	== 0)	{ std::cout << "There is no vertices";	return; }
		if (attributes.normals.size()	== 0)	{ std::cout << "There is no normals";	return; }

		// __________________________________________________________________ Vertex position data

		const vector<tinyobj::real_t> vertices = attributes.vertices;

		// Resizing lists to determined size
		size_t number_of_vertices = vertices.size() / 3;
		cout << "	Verts: " << number_of_vertices << std::endl;
		originalVertices.resize(number_of_vertices);
		transformedVertices.resize(number_of_vertices);
		displayVertices.resize(number_of_vertices);

		// Storing vertex position data into mesh 
		for (size_t index = 0, vertex_counter = 0; index < number_of_vertices; index++, vertex_counter += 3)
			originalVertices[index] = Vertex({ vertices[vertex_counter],vertices[vertex_counter + 1],vertices[vertex_counter + 2],1 });

		// __________________________________________________________________ Vertex color and normal data   

		const vector<tinyobj::real_t> colors = attributes.colors;
		const vector<tinyobj::real_t> normals = attributes.normals;

		// Resizing lists to determined size
		size_t number_of_colors = colors.size() / 3;
		size_t number_of_normals = normals.size() / 3;
		cout << "	Colors: " << number_of_colors << std::endl;
		cout << "	Normals: " << number_of_normals << std::endl;
		assert(number_of_colors == number_of_vertices);
		originalColors.resize(number_of_colors);
		originalNormals.resize(number_of_normals);
		transformedColors.resize(originalColors.size());

		// Storing vertex color and normal data into mesh 
		for (size_t index = 0, color_counter = 0; index < number_of_colors; index++, color_counter += 3)
		{
			originalColors[index] = meshColor;
			originalNormals[index] = Vertex({ normals[color_counter + 0],normals[color_counter + 1],normals[color_counter + 2], 0 });
		}

		// __________________________________________________________________ Indices data

		const tinyobj::mesh_t triangles = shapes[0].mesh;

		// Resizing lists to determined size
		size_t number_of_triangles = triangles.indices.size() / 3;
		cout << "	Indices: " << number_of_triangles << std::endl;
		originalIndices.resize(number_of_triangles * 3);

		// Storing mesh indices data into mesh 
		Index_Buffer::iterator indices_iterator = originalIndices.begin();
		for (size_t triangle_index = 0, triangle_counter = 0; triangle_index < number_of_triangles; triangle_index++, triangle_counter += 3)
		{
			*indices_iterator++ = triangles.indices[triangle_counter + 0].vertex_index;
			*indices_iterator++ = triangles.indices[triangle_counter + 1].vertex_index;
			*indices_iterator++ = triangles.indices[triangle_counter + 2].vertex_index;
		}
	}

	void Mesh::Update(Scene * scene, Transformation3f parentTransform)
	{
		// TO REMOVE ______
		static float angle = 0.f;
		angle += 0.01f;

		//rotation_x.set< Rotation3f::AROUND_THE_X_AXIS >(0.50f);
		rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(angle);
		// TO REMOVE ______


		// Unified transform matrix
		transformation = parentTransform * translation * rotation_x * rotation_y * scaling;

		// Transform every vertex using the unified transfrom
		for (size_t index = 0, number_of_vertices = originalVertices.size(); index < number_of_vertices; index++)
		{
			// Transform vertex and normal
			Vertex transformedVertex = Matrix44f(transformation) * Matrix41f(originalVertices[index]);
			Vertex transformedNormal = Matrix44f(transformation) * Matrix41f(originalNormals[index]);

			// ______________________________________________________ Lambert Shading ______________________________________________________
			// Normalize vertex normal
			float normalMagnitude = std::sqrt(std::powf(transformedNormal[0], 2) + std::powf(transformedNormal[1], 2) + std::powf(transformedNormal[2], 2));
			Vertex normalNormalized({ transformedNormal[0] / normalMagnitude,transformedNormal[1] / normalMagnitude,transformedNormal[2] / normalMagnitude });
			// Normalize light vector
			float lightMagnitude = std::sqrt(std::powf(scene->rasterizer.light.position[0], 2) + std::powf(scene->rasterizer.light.position[1], 2) + std::powf(scene->rasterizer.light.position[2], 2));
			Vertex lightNormalized({ scene->rasterizer.light.position[0] / lightMagnitude, scene->rasterizer.light.position[1] / lightMagnitude, scene->rasterizer.light.position[2] / lightMagnitude });
			// Dot product between both normalized vectors, light and normal
			float intensity = (normalNormalized[0] * lightNormalized[0] + normalNormalized[1] * lightNormalized[1] + normalNormalized[2] * lightNormalized[2]) * 1.f;
			intensity = std::max(intensity, 0.f);
			// Clamp and set colors 
			transformedColors[index].set_clamped(static_cast<int>(meshColor.data.component.r * intensity), static_cast<int>(meshColor.data.component.g * intensity), static_cast<int>(meshColor.data.component.b * intensity));
			// _____________________________________________________________________________________________________________________________


			// Transform the vertex with the projection of the camera
			Vertex & vertex = transformedVertices[index] = Matrix44f(scene->camera->projection) * Matrix41f(transformedVertex);

			// Normalize
			float divisor = 1.f / vertex[3];
			vertex[0] *= divisor;
			vertex[1] *= divisor;
			vertex[2] *= divisor;
			vertex[3] = 1.f;
		}

		// Update every child with their parent transform (this one)
		for (auto& meshChild : meshesChildren)
			meshChild->Update(scene, transformation);
	}

	void Mesh::Render(Scene * scene)
	{
		// Get camera transform in order to get the display vertex
		Matrix44f cameraTransform = Matrix44f(scene->camera->getTransformation());

		// Calculate vertex positions with the camera transform
		for (size_t index = 0, number_of_vertices = transformedVertices.size(); index < number_of_vertices; index++)
			displayVertices[index] = Point4i(cameraTransform * Matrix41f(transformedVertices[index]));

		// Render every triangle 
		for (int * indices = originalIndices.data(), *end = indices + originalIndices.size(); indices < end; indices += 3)
		{
			// Only if its a front face
			if (IsFrontface(transformedVertices.data(), indices))
			{
				scene->rasterizer.set_color(transformedColors[*indices]);
				scene->rasterizer.fill_convex_polygon_z_buffer(displayVertices.data(), indices, indices + 3);
			}
		}

		// Render every child
		for (auto& meshChild : meshesChildren)
			meshChild->Render(scene);
	}

	bool Mesh::IsFrontface(const Vertex * const projected_vertices, const int * const indices)
	{
		const Vertex & v0 = projected_vertices[indices[0]];
		const Vertex & v1 = projected_vertices[indices[1]];
		const Vertex & v2 = projected_vertices[indices[2]];

		return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
	}
}
