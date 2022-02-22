#include "cApp.h"
#include "vector.hpp"

// default constructor
cApp::cApp() {
	isRunning = true;
	pWindow = NULL;
	pRenderer = NULL;
}

// handles initialization of SDL2, sets up window, etc.
bool cApp::onInit() {
	// initialization, if SDL_Init() returns less than 0 do not start
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;
	// create window
	pWindow = SDL_CreateWindow("RayTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	if (pWindow != NULL) {
		// create renderer
		pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
		// initialize the image instance
		m_image.initialize(1280, 720, pRenderer);
		// set the background color to white
		SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
		SDL_RenderClear(pRenderer);
		// render the scene
		m_scene.render(m_image);
		// display the image
		m_image.display();
		// show the result
		SDL_RenderPresent(pRenderer);
	}
	else return false;
	return true;
}

// called when the app is initially set up
int cApp::onExecute() {
	SDL_Event event;
	// if initialization failed return -1
	if (onInit() == false) return -1;
	// while window is running
	while (isRunning) {
		while (SDL_PollEvent(&event) != 0) onEvent(&event);
		onLoop();
		onRender();
	}
}

// event handler, only handles quit for now
void cApp::onEvent(SDL_Event* event) {
	if (event->type == SDL_QUIT) isRunning = false;
}

void cApp::onLoop() {

}

// renderer
void cApp::onRender() {
	// set the background color to white
	// SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	// _RenderClear(pRenderer);
	// render the scene
	// cene.render(m_image);
	// display the image
	// m_image.display();
	// render the result
	// SDL_RenderPresent(pRenderer);
}

// handles exiting
void cApp::onExit() {
	// tidy up SDL2 stuff
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	pWindow = NULL;
	SDL_Quit();
}

// debugging
void cApp::printVector(const vector<double> &inputVector) {
	int nRows = inputVector.getNumDims();
	for (int row = 0; row < nRows; row++) {
		std::cout << std::fixed << std::setprecision(3) << inputVector.getElement(row) << std::endl;
	}
}