// https://en.wikipedia.org/wiki/VU_meter

// pridat Winmm.lib do linkeru
#include <Windows.h>
#include <Mmsystem.h>
#include <stdio.h>
#include <chrono>
#include "Grafika/Platno.h"
#include "Grafika/CtyrbokyHranol.h"
#include "Audio/Wav.h"

// Doporuceny soubor pro testovani:
// https://freewavesamples.com/ensoniq-zr-76-01-dope-77-bpm

#define PATH "C:\\Users\\erik\\source\\repos\\AsciiAmplitudeMeter\\file.wav"

double Rescale(int amplitude, double max)
{
	double k = (double)amplitude / 32768;
	double value = k * max;

	return value;
}

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

	CtyrbokyHranol levyHranol(Bod3d(2.0, 2.0, -8.0), 3, 3);
	CtyrbokyHranol pravyHranol(Bod3d(2.0 + 8, 2.0, -8.0), 3, 3);
	
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

			play = wav.CalculateAmplitude(elapsedMilliseconds.count(), 2, lMax, rMax);

			platno.Vymaz();

			const int max = 5;

			double vyskaLeft = Rescale(lMax, max);
			double vyskaRight = Rescale(rMax, max);

			levyHranol.ZmenVysku(vyskaLeft);
			pravyHranol.ZmenVysku(vyskaLeft);

			levyHranol.Nakresli(platno);
			pravyHranol.Nakresli(platno);

			platno.Zobraz();
			
		} while (play);

		printf("Stiskni klavesu enter pro opakovani.\n");
		int znak = getchar();
	}
}
