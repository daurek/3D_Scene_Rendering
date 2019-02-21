// Header
#include "Scene.hpp"

// System
#include <iostream>
#include <string>
// Libraries
#include "Projection.hpp"
#include "rapidxml_utils.hpp"
// Project
#include "Light.hpp"

namespace example
{
	using namespace toolkit;

	Scene::Scene(size_t width, size_t height, const std::string & scene_content_xml) : 
		width(width), height(height), 
		colorBuffer(width, height), 
		rasterizer(colorBuffer, Light{}), 
		camera(new Camera(5, 15, 20, float(width) / float(height), { float(width / 2), float(height / 2), 0.f }, { float(width / 2), float(height / 2), 100000000.f }))
    {
		LoadScene(scene_content_xml);
    }

    void Scene::Update ()
    {
		// Update every mesh
		for (auto const& mesh : meshes)
			mesh.second->Update(this);
    }

    void Scene::Render ()
    {
        // Clear color buffer
		rasterizer.clear();

		// Render every mesh
		for (auto const& mesh : meshes)
			mesh.second->Render(this);

        // Copy hidden color buffer to the window color buffer
        rasterizer.get_color_buffer ().gl_draw_pixels (0, 0);
    }

	void Scene::LoadScene(const std::string & file)
	{
		std::cout << "\n________Loading Scene________\n";
		// Load xml
		rapidxml::file<> xml_file(file.c_str());
		
		// Parse xml
		rapidxml::xml_document<> doc;
		doc.parse<0>(xml_file.data());
		
		// Load Meshes
		LoadMeshes(doc.first_node()->first_node());

		std::cout << "\n\n________Scene  Loaded________";
	}

	void Scene::LoadMeshes(rapidxml::xml_node<>* meshes_node)
	{
		std::cout << "\n	________Loading Meshes________";
		// Loop through every mesh
		for (rapidxml::xml_node<>* mesh_node = meshes_node->first_node(); mesh_node; mesh_node = mesh_node->next_sibling())
		{
			// Get Name
			std::string entity_id = mesh_node->first_attribute()->value();
			// Get Path
			std::string path = mesh_node->first_node()->value();

			// Check validity
			if (entity_id.empty() || path.empty())
			{
				std::cout << "Empty ID or path";
				continue;
			}

			// Get position
			rapidxml::xml_node<> * position_node = mesh_node->first_node("position");
			// Position string values
			std::string x_pos_str = position_node->first_node("x")->value();
			std::string y_pos_str = position_node->first_node("y")->value();
			std::string z_pos_str = position_node->first_node("z")->value();
			// Position float values
			float x_pos = std::stof(x_pos_str);
			float y_pos = std::stof(y_pos_str);
			float z_pos = std::stof(z_pos_str);

			// Get scale
			rapidxml::xml_node<> * scale_node = mesh_node->first_node("scale");
			// Scale string values
			std::string x_scale_str = scale_node->first_node("x")->value();
			std::string y_scale_str = scale_node->first_node("y")->value();
			std::string z_scale_str = scale_node->first_node("z")->value();
			// Scale float values
			float x_rot = std::stof(x_scale_str);
			float y_rot = std::stof(y_scale_str);
			float z_rot = std::stof(z_scale_str);

			// Get color
			rapidxml::xml_node<> * color_node = mesh_node->first_node("color");
			// Color string values
			std::string red_str = color_node->first_node("red")->value();
			std::string green_str = color_node->first_node("green")->value();
			std::string blue_str = color_node->first_node("blue")->value();
			// Color float values
			uint8_t red = static_cast<uint8_t>		(std::stof(red_str));
			uint8_t green = static_cast<uint8_t>	(std::stof(green_str));
			uint8_t blue = static_cast<uint8_t>		(std::stof(blue_str));

			// Create mesh
			std::shared_ptr< Mesh > mesh(new Mesh
			(
				"../../assets/meshes/" + path, 
				{ x_pos, y_pos, z_pos	},
				{ x_rot, y_rot, z_rot	},
				{ red, green, blue		},
				false
			));
			
			if (mesh)
			{
				std::cout << "\n	Position:	x = " + x_pos_str +		"| y = " + y_pos_str +		"| z = " + z_pos_str;
				std::cout << "\n	Scale:		x = " + x_scale_str +	"| y = " + y_scale_str +	"| z = " + z_scale_str;
				std::cout << "\n	Color:		red = " + red_str +		"| green = " + green_str +	"| blue = " + blue_str;

				meshes[entity_id] = mesh;
			}
			else
			{
				std::cout << "\n Mesh has not been loaded";
			}
		}
	}

}
