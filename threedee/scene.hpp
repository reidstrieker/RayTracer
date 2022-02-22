#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <SDL.h>
#include "image.hpp"
#include "camera.hpp"
#include "objsphere.hpp"

namespace RT {
	class scene {
		public:
			// default constructor
			scene();
			// function to perform the rendering
			bool render(image& outputImage);
		private:
			// the camera that we will use
			RT::camera m_camera;
			// a unit sphere on the origin, for testing
			RT::objsphere m_testSphere;
	};
}

#endif