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

	Bod2d Projekce(Bod3d bod, double f)
	{
		bod -= pozice;

		/*double x = bod.z * sin(-rotace.y) + bod.x * cos(-rotace.y);
		double y = bod.y;
		double z = bod.z * cos(-rotace.y) - bod.x * sin(-rotace.y);*/

		double xp = -(f * bod.x / bod.z);
		double yp = -(f * bod.y / bod.z);

		return Bod2d(xp, yp);
	}
};