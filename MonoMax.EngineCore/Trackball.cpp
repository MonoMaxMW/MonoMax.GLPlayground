///////////////////////////////////////////////////////////////////////////////
// Trackball.cpp
// =============
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
// UPDATED: 2016-04-01
//
// Copyright (C) 2011. Song Ho Ahn
///////////////////////////////////////////////////////////////////////////////

#include "Trackball.h"
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
// ctor
///////////////////////////////////////////////////////////////////////////////
Trackball::Trackball() : radius(0), screenWidth(0), screenHeight(0),
halfScreenWidth(0), halfScreenHeight(0), mode(Trackball::ARC)
{
}
Trackball::Trackball(float radius, int width, int height) : radius(radius),
screenWidth(width),
screenHeight(height),
mode(Trackball::ARC)
{
	halfScreenWidth = width * 0.5f;
	halfScreenHeight = height * 0.5f;
}



///////////////////////////////////////////////////////////////////////////////
// dtor
///////////////////////////////////////////////////////////////////////////////
Trackball::~Trackball()
{
}



///////////////////////////////////////////////////////////////////////////////
// print itself
///////////////////////////////////////////////////////////////////////////////
void Trackball::printSelf() const
{
	std::cout << "===== Trackball =====\n"
		<< "     Radius: " << radius << "\n"
		<< "Screen Size: (" << screenWidth << ", " << screenHeight << ")\n"
		<< "       Mode: " << (mode == ARC ? "ARC\n" : "PROJECT\n")
		<< std::endl;
}



///////////////////////////////////////////////////////////////////////////////
// setters
///////////////////////////////////////////////////////////////////////////////
void Trackball::set(float r, int w, int h)
{
	radius = r;
	screenWidth = w;
	screenHeight = h;
	halfScreenWidth = w * 0.5f;
	halfScreenHeight = h * 0.5f;
}

void Trackball::setScreenSize(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	halfScreenWidth = w * 0.5f;
	halfScreenHeight = h * 0.5f;
}



///////////////////////////////////////////////////////////////////////////////
// clamp x and y
// the params of getVector() must be inside the window: -half < (x,y) < +half
///////////////////////////////////////////////////////////////////////////////
float Trackball::clampX(float x) const
{
	if (x <= -halfScreenWidth)
		x = -halfScreenWidth + 1;
	else if (x >= halfScreenWidth)
		x = halfScreenWidth - 1;
	return x;
}

float Trackball::clampY(float y) const
{
	if (y <= -halfScreenHeight)
		y = -halfScreenHeight + 1;
	else if (y >= halfScreenHeight)
		y = halfScreenHeight - 1;
	return y;
}



///////////////////////////////////////////////////////////////////////////////
// return the point corrds on the sphere
///////////////////////////////////////////////////////////////////////////////
glm::vec3 Trackball::getVector(int x, int y) const
{
	if (radius == 0 || screenWidth == 0 || screenHeight == 0)
		return glm::vec3(0, 0, 0);

	// compute mouse position from the centre of screen (-half ~ +half)
	float mx = x - halfScreenWidth;
	float my = halfScreenHeight - y;    // OpenGL uses bottom to up orientation
	//float mx = clampX(x - halfScreenWidth);
	//float my = clampY(halfScreenHeight - y);    // OpenGL uses bottom to up orientation

	if (mode == Trackball::PROJECT)
		return getVectorWithProject(mx, my);
	else
		return getVectorWithArc(mx, my); // default mode
}



///////////////////////////////////////////////////////////////////////////////
// return the point on the sphere as a unit vector
///////////////////////////////////////////////////////////////////////////////
glm::vec3 Trackball::getUnitVector(int x, int y) const
{
	glm::vec3 vec = getVector(x, y);
	vec = glm::normalize(vec);
	return vec;
}



///////////////////////////////////////////////////////////////////////////////
// use the mouse distance from the centre of screen as arc length on the sphere
// x = R * sin(a) * cos(b)
// y = R * sin(a) * sin(b)
// z = R * cos(a)
// where a = angle on x-z plane, b = angle on x-y plane
//
// NOTE: the calculation of arc length is an estimation using linear distance
// from screen center (0,0) to the cursor position
///////////////////////////////////////////////////////////////////////////////
glm::vec3 Trackball::getVectorWithArc(float x, float y) const
{
	float arc = sqrtf(x * x + y * y);   // legnth between cursor and screen center
	float a = arc / radius;         // arc = r * a
	float b = atan2f(y, x);         // angle on x-y plane
	float x2 = radius * sinf(a);    // x rotated by "a" on x-z plane

	glm::vec3 vec;
	vec.x = x2 * cosf(b);
	vec.y = x2 * sinf(b);
	vec.z = radius * cosf(a);

	return vec;
}



///////////////////////////////////////////////////////////////////////////////
// project the mouse coords to the sphere to find the point coord
// return the point on the sphere using hyperbola where x^2 + y^2 > r^2/2
///////////////////////////////////////////////////////////////////////////////
glm::vec3 Trackball::getVectorWithProject(float x, float y) const
{
	/*
	Vector3 vec = Vector3(x, y, 0);
	float d = x*x + y*y;
	float rr = radius * radius;
	if(d <= rr)
	{
		vec.z = sqrtf(rr - d);
	}
	else
	{
		vec.z = 0;
		// put (x,y) on the sphere radius
		vec.normalize();
		vec *= radius;
	}

	return vec;
	*/

	glm::vec3 vec = glm::vec3(x, y, 0);
	float d = x * x + y * y;
	float rr = radius * radius;

	// use sphere if d<=0.5*r^2:  z = sqrt(r^2 - (x^2 + y^2))
	if (d <= (0.5f * rr))
	{
		vec.z = sqrtf(rr - d);
	}
	// use hyperbolic sheet if d>0.5*r^2:  z = (r^2 / 2) / sqrt(x^2 + y^2)
	// referenced from trackball.c by Gavin Bell at SGI
	else
	{
		// compute z first using hyperbola
		vec.z = 0.5f * rr / sqrtf(d);

		// scale x and y down, so the vector can be on the sphere
		// y = ax => x^2 + (ax)^2 + z^2 = r^2 => (1 + a^2)*x^2 = r^2 - z^2
		// => x = sqrt((r^2 - z^2) / (1 - a^2)
		float x2, y2, a;
		if (x == 0.0f) // avoid dividing by 0
		{
			x2 = 0.0f;
			y2 = sqrtf(rr - vec.z * vec.z);
			if (y < 0)       // correct sign
				y2 = -y2;
		}
		else
		{
			a = y / x;
			x2 = sqrtf((rr - vec.z * vec.z) / (1 + a * a));
			if (x < 0)   // correct sign
				x2 = -x2;
			y2 = a * x2;
		}

		vec.x = x2;
		vec.y = y2;
	}

	return vec;
}
