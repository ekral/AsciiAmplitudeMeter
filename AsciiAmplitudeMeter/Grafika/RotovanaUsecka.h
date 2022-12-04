#pragma once
#include "Usecka.h"
#include "Transformace.h"
#include "Platno.h"

enum class Stav
{
	Nahoru,
	Dolu
};

struct RotovanaUsecka
{
	Usecka usecka;
	Bod2d pt1;
	Bod2d pt2;

	RotovanaUsecka(Bod2d p1, Bod2d p2) 
		: usecka(p1, p2), pt1(p1), pt2(p2)
	{
	}

	void ZmenUhel(double uhelStupne)
	{
		pt1 = usecka.P1;
		pt2 = RotaceKolemBodu(usecka.P2, usecka.P1, -uhelStupne);
	}

	void Nakresli(Platno& platno)
	{
		platno.NakresliUsecku(pt1, pt2);
	}
};
