#pragma once
#include "Bod3d.h"
#include "Platno.h"
#include "Transformace.h"

struct CtyrbokyHranol
{
	Bod3d pa;
	double n;
	double v;

	Bod3d dolni[4];
	Bod3d horni[4];

	CtyrbokyHranol(Bod3d a, double n, double v) : 
		pa(a), n(n), v(v), 
		dolni{ Bod3d(a.x, a.y, a.z), Bod3d(a.x + n, a.y, a.z), Bod3d(a.x + n, a.y, a.z + n), Bod3d(a.x, a.y, a.z + n) },
		horni{ Bod3d(a.x, a.y + v, a.z), Bod3d(a.x + n, a.y + v, a.z), Bod3d(a.x + n, a.y + v, a.z + n), Bod3d(a.x, a.y + v, a.z + n) }
	{
		
	}

	void ZmenVysku(double v)
	{
		horni[0].y = dolni[0].y + v;
		horni[1].y = dolni[1].y + v;
		horni[2].y = dolni[2].y + v;
		horni[3].y = dolni[3].y + v;
	}

	void Nakresli(Platno& platno)
	{
		double f = 20.0;

		Bod2d a0 = Projekce(dolni[0], f);
		Bod2d b0 = Projekce(dolni[1], f);
		Bod2d c0 = Projekce(dolni[2], f);
		Bod2d d0 = Projekce(dolni[3], f);

		Bod2d a1 = Projekce(horni[0], f);
		Bod2d b1 = Projekce(horni[1], f);
		Bod2d c1 = Projekce(horni[2], f);
		Bod2d d1 = Projekce(horni[3], f);

		platno.NakresliUsecku(a0, b0);
		platno.NakresliUsecku(b0, c0);
		platno.NakresliUsecku(c0, d0);
		platno.NakresliUsecku(d0, a0);

		platno.NakresliUsecku(a1, b1);
		platno.NakresliUsecku(b1, c1);
		platno.NakresliUsecku(c1, d1);
		platno.NakresliUsecku(d1, a1);

		platno.NakresliUsecku(a0, a1);
		platno.NakresliUsecku(b0, b1);
		platno.NakresliUsecku(c0, c1);
		platno.NakresliUsecku(d0, d1);
	}
};