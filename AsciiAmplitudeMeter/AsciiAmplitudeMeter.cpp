// https://en.wikipedia.org/wiki/VU_meter

// pridat Winmm.lib do linkeru
#include <Windows.h>
#include <conio.h>
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

	Kamera kamera(Bod3d(0.0, 0.0, 0.0), Bod3d(0.0, 0.0, 0.0), 10.0);

	Platno platno(20, 70, '-', 'o');
	
	double n = 20.0;
	double d = 10.0;
	double y = -8;
	double z = -n - 10;

	CtyrbokyHranol levyHranol(Bod3d(-d - n, y, z), n, 1.0);
	CtyrbokyHranol pravyHranol(Bod3d(d, y, z), n, 1.0);
	
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

			const int max = 50;

			double vyskaLeft = Rescale(lMax, max);
			double vyskaRight = Rescale(rMax, max);

			levyHranol.ZmenVysku(vyskaLeft);
			pravyHranol.ZmenVysku(vyskaLeft);

			levyHranol.Nakresli(kamera, platno);
			pravyHranol.Nakresli(kamera, platno);

			platno.NakresliBod(0, 0);

			platno.Zobraz();

			if (_kbhit())
			{
				int klavesa = _getch();

				switch (klavesa)
				{
				case 77:
					++kamera.pozice.x;
					break;
				case 75:
					--kamera.pozice.x;
					break;
				case 72:
					++kamera.pozice.y;
					break;
				case 80:
					--kamera.pozice.y;
					break;
				case 's':
					++kamera.pozice.z;
					break;
				case 'w':
					--kamera.pozice.z;
					break;
				}
			}
			
		} while (play);
	}
}
