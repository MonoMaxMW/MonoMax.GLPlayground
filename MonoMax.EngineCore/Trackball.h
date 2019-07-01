///////////////////////////////////////////////////////////////////////////////
// Trackball.h
// ===========
// Trackball class
// This class takes the current mouse cursor position (x,y), and map it to the
// point (x,y,z) on the trackball(sphere) surface. Since the cursor point is in
// screen space, this class depends on the current screen width and height in
// order to compute the vector on the sphere.
//
// There are 2 modes (Arc and Project) to compute the vector on the sphere: Arc
// mode is that the length of the mouse position from the centre of screen
// becomes arc length moving on the sphere, and Project mode is that directly
// projects the mouse position to the sphere.
//
// The default mode is Arc because it allows negative z-value (a point on the
// back of the sphere). On the other hand, Project mode is limited to
// front hemisphere rotation (z-value is always positive).
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2011-12-09
// UPDATED: 2016-03-31
//
// Copyright (C) 2011. Song Ho Ahn
///////////////////////////////////////////////////////////////////////////////

#ifndef TRACKBALL_H
#define TRACKBALL_H

#include "common_math.h"


class Trackball
{
public:
	enum Mode
	{
		ARC = 0,
		PROJECT
	};

	// ctor/dtor
	Trackball();
	Trackball(float radius, int screenWidth, int screenHeight);
	~Trackball();

	void printSelf() const;

	// setters
	void set(float r, int w, int h);
	void setScreenSize(int w, int h);
	void setRadius(float r) { radius = r; }
	void setMode(Trackball::Mode mode) { this->mode = mode; }

	// getters
	int getScreenWidth() const { return screenWidth; }
	int getScreenHeight() const { return screenHeight; }
	float getRadius() const { return radius; }
	Trackball::Mode getMode() const { return mode; }
	glm::vec3 getVector(int x, int y) const;      // return a point on sphere for given mouse (x,y)
	glm::vec3 getUnitVector(int x, int y) const;  // return normalized point for mouse (x, y)

protected:

private:
	// member functions
	glm::vec3 getVectorWithArc(float x, float y) const;
	glm::vec3 getVectorWithProject(float x, float y) const;
	float clampX(float x) const;                // -halfScreenWidth < x < halfScreenWidth
	float clampY(float y) const;                // -halfScreenHeight < y < halfScreenHeight

	// member variables
	float radius;
	int   screenWidth;
	int   screenHeight;
	float halfScreenWidth;
	float halfScreenHeight;
	Trackball::Mode mode;

};

#endif
