/// ----------------------------------------------------------------------------------------------------------------------
/// 3D SCENE RENDERING
/// \class renderscene::Camera
///
/// \author Ilyass Sofi Hlimi
/// \date 06/03/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef CAMERA_HEADER
#define CAMERA_HEADER

// Libraries
#include "Projection.hpp"
#include "Scaling.hpp"
#include "Translation.hpp"
#include "Transformation.hpp"

namespace renderscene
{
	using toolkit::Projection3f;
	using toolkit::Scaling3f;
	using toolkit::Translation3f;
	using toolkit::Transformation3f;

	/// Unifies Camera data with Projection frustum and Transform
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

		inline const Transformation3f & getTransformation() { return transformation = translation * scaling; }

	private:

		Transformation3f	transformation;
	};
}

#endif
