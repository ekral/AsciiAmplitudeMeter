#pragma once
#include "Bod3d.h"
#include "Bod2d.h"

struct Kamera
{
	Bod3d pozice;
	double f;

	Kamera(Bod3d pozice, double f) : pozice(pozice), f(f)
	{
	}

	Bod2d Projekce(Bod3d bod, double f)
	{
		bod -= pozice;

		double x = f * -bod.x / bod.z;
		double y = f * -bod.y / bod.z;

		return Bod2d(x, y);
	}
};