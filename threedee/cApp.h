#pragma once
#ifndef CAPP_H
#define CAPP_H
// for efficiency, makes sure cApp.h is not defined/compiled more than once
#include <SDL.h>
#include "image.hpp"
#include "scene.hpp"
#include "camera.hpp"

class cApp {
	public:
		cApp();
		int onExecute();
		bool onInit();
		void onEvent(SDL_Event* event);
		void onLoop();
		void onRender();
		void onExit();

	private:
		// for debugging, prints the values of the vector to the terminal
		void printVector(const vector<double>& inputVector);
		// an instance of the image class to store the image
		image m_image;
		// an instance of the scene class
		RT::scene m_scene;
		// stuff to make SDL2 work
		bool isRunning;
		SDL_Window* pWindow;
		SDL_Renderer* pRenderer;
};

#endif