#ifndef CAMERA_HEADER
#define CAMERA_HEADER

// System


// Libraries
#include "Projection.hpp"
#include "Scaling.hpp"
#include "Translation.hpp"
#include "Rotation.hpp"
#include "Transformation.hpp"
// Project


namespace example
{

	using toolkit::Projection3f;
	using toolkit::Scaling3f;
	using toolkit::Translation3f;
	using toolkit::Transformation3f;

	class Camera
	{
	public:

		Camera(const float & near_z, const float & far_z, const float & fov_in_degrees, const float & aspect_ratio, Translation3f position, Scaling3f scale) :
			projection(near_z, far_z, fov_in_degrees, aspect_ratio),
			translation(position), 
			scaling(scale)
		{}

		Projection3f		projection;
		Scaling3f			scaling;
		Translation3f		translation;
		

		const Transformation3f & getTransformation()
		{
			transformation = translation * scaling;
			return transformation;
		}

	private:

		Transformation3f	transformation;
	};

	

	

}

#endif
