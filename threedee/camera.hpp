#ifndef CAMERA_H
#define CAMERA_H
#include "vector.hpp"
#include "ray.hpp"

namespace RT {
	class camera {
		public: 
			// default constructor
			camera();
			// functions to set camera parameters
			void setPosition(const vector<double>& newPosition);
			void setLookAt(const vector<double>& newLookAt);
			void setUp(const vector<double>& upVector);
			void setLength(double newLength); // dist between pinhole and screen in camera
			void setHorzSize(double newSize);
			void setAspect(double newAspect);
			// functions to return camera parameters
			vector<double> getPosition();
			vector<double> getLookAt();
			vector<double> getUp();
			vector<double> getU();
			vector<double> getV();
			vector<double> getScreenCenter();
			double getLength();
			double getHorzSize();
			double getAspect();
			// function to generate a ray
			bool generateRay(float proScreenX, float proScreenY, RT::ray &cameraRay);
			// function to update the camera geometry
			void updateCameraGeometry();
		private:
			vector<double> m_cameraPosition{ 3 };
			vector<double> m_cameraLookAt{ 3 };
			vector<double> m_cameraUp{ 3 };
			double m_cameraLength;
			double m_cameraHorzSize;
			double m_cameraAspectRatio;
			vector<double> m_alignmentVector{ 3 }; // principle axes of camera
			vector<double> m_projectionScreenU{ 3 };
			vector<double> m_projectionScreenV{ 3 };
			vector<double> m_projectionScreenCenter{ 3 };
	};
}

#endif