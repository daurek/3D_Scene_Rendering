/// ----------------------------------------------------------------------------------------------------------------------
/// 3D SCENE RENDERING
/// \class renderscene::Light
///
/// \author Ilyass Sofi Hlimi
/// \date 06/03/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include "Point.hpp"
#include "Color_Buffer_Rgba8888.hpp"

namespace renderscene
{
	/// Unifies Light Position and Color
	class Light
	{

	private:

		typedef toolkit::Point4f	  Vertex;
		typedef Color_Buffer_Rgba8888 Color_Buffer;
		typedef Color_Buffer::Color	  Color;

	public:

		/// Creates a Light in the given position
		Light(Vertex _position, Color _color) : position(_position), color(_color) {};
	
		Vertex				position;
		/// Color is not being used currently
		Color				color;

	};

}

#endif