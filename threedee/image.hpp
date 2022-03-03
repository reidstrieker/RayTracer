#pragma once
#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <vector>
#include <SDL.h>

class image {
public:
	// constructor
	image();
	// destructor
	~image();
	// function to initialize
	void initialize(const int xSize, const int ySize, SDL_Renderer* pRenderer);
	// function to set the color of a pixel
	void setPixel(const int x, const int y, const double red, const double green, const double blue);
	// function to handle the image for display
	void display();
	// functions to return the dimensions of the image
	int getXSize();
	int getYSize();

private:
	// function that accepts RGB and returns Uint32 to represent that in color space for SDL2
	Uint32 convertColor(const double red, const double green, const double blue);
	// SDL2 handling
	void initTexture();
	void computeMaxValues();
	// arrays to store image data
	std::vector<std::vector<double>> m_rChannel; // red channel data
	std::vector<std::vector<double>> m_gChannel; // greeen channel data
	std::vector<std::vector<double>> m_bChannel; // blue channel data
	// store the dimensions of the image
	int m_xSize, m_ySize;
	// store the maximum values
	double m_maxRed, m_maxGreen, m_maxBlue, m_overallMax;
	// SDL2 stuff
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture;
};

#endif