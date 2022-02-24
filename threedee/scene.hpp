#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <vector>
#include <SDL.h>
#include "image.hpp"
#include "camera.hpp"
#include "objsphere.hpp"
#include "pointlight.hpp"

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
			// the list of objects in the scene (creates pointers to instances of base class of our objects)
			std::vector<std::shared_ptr<RT::objectbase>> m_objectList;
			// the list of point lights in the scene
			std::vector<std::shared_ptr<RT::lightbase>> m_lightList;
	};
}

#endif