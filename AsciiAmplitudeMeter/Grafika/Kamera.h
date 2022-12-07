#pragma once
#include "Bod3d.h"
#include "Bod2d.h"

struct Kamera
{
	Bod3d pozice;
	Bod3d rotace;
	double f;

	Kamera(Bod3d pozice, Bod3d rotace, double f) : pozice(pozice), rotace(rotace), f(f)
	{
	}

	Bod2d Projekce(Bod3d bod)
	{
		bod -= pozice;

		double xp = -(f * bod.x / bod.z);
		double yp = -(f * bod.y / bod.z);

		return Bod2d(xp, yp);
	}
};