#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include <Point.hpp>
#include "Translation.hpp"

namespace example
{
	using toolkit::Translation3f;
	using toolkit::Point4f;

	class Light
	{

	private:

		typedef Point4f               Color;
		typedef Point4f               Vertex;

	public:

		Light() : position({ 1,1,0 }), color({ 255,255,0 }) {};
	
		Vertex				position;
		Color				color;

	};

}

#endif