// https://en.wikipedia.org/wiki/VU_meter

// pridat Winmm.lib do linkeru
#include <Windows.h>
#include <Mmsystem.h>
#include <stdio.h>
#include <chrono>
#include "Grafika/Platno.h"
#include "Grafika/RotovanaUsecka.h"
#include "Grafika/Jehlan.h"
#include "Audio/Wav.h"

// Doporuceny soubor pro testovani:
// https://freewavesamples.com/ensoniq-zr-76-01-dope-77-bpm

#define PATH "C:\\Users\\ekral\\Downloads\\file.wav"

int main()
{
	Wav wav;

	bool ok = wav.LoadWavFile(PATH);

	if (!ok)
	{
		printf("Nepovedlo se nacist soubor.\n");
		return -1;
	}

	if (wav.fmtAudioFormat != 1 && wav.fmtNumChannels != 2 && wav.fmtBitsPerSample != 16)
	{
		printf("Nespravny format wav souboru..\n");
		return -1;
	}

	Platno platno(20, 70, ' ', 'x');

	Usecka usecka(0.0, 0.0, 20.0, 0.0);

	while (true)
	{
		PlaySound(TEXT(PATH), NULL, SND_FILENAME | SND_ASYNC);
		Sleep(200);

		auto start = std::chrono::steady_clock::now();

		int lMax = 0;
		int rMax = 0;

		bool play = false;

		do
		{
			auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

			play = wav.MaxAmplitude(elapsedMilliseconds.count(), 2, lMax, rMax);

			platno.Vymaz();

			int xLeft = (lMax * (platno.pocetSloupcu - 1)) / 32768;
			int xRight = (rMax * (platno.pocetSloupcu - 1)) / 32768;

			platno.NakresliUsecku(Bod2d(0.0, 1.0), Bod2d(xLeft, 1.0));
			platno.NakresliUsecku(Bod2d(0.0, 0.0), Bod2d(xRight, 0.0));

			platno.Zobraz();
			
		} while (play);

		printf("Stiskni klavesu enter pro opakovani.\n");
		int znak = getchar();
	}
}
