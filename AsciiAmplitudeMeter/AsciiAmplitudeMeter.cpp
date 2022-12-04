// https://en.wikipedia.org/wiki/VU_meter

// pridat Winmm.lib do linkeru
#include <Windows.h>
#include <Mmsystem.h>
#include <stdio.h>
#include "Grafika/Platno.h"
#include "Grafika/RotovanaUsecka.h"
#include "Grafika/Jehlan.h"
#include "Audio/Wav.h"
#include <chrono>

int main()
{
	// Doporuceny soubor pro testovani:
	// https://freewavesamples.com/ensoniq-zr-76-01-dope-77-bpm

	auto path = "C:\\Users\\ekral\\Downloads\\file.wav";
	Wav wav;

	bool ok = wav.LoadWavFile(path);

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

	Platno platno(20, 70, '-', 'x');

	RotovanaUsecka rotovanaUsecka1(Bod2d(30.0, 0.0), Bod2d(20.0, 10.0));
	RotovanaUsecka rotovanaUsecka2(Bod2d(20.0, 10.0), Bod2d(25.0, 10.0));

	PlaySound(L"C:\\Users\\ekral\\Downloads\\file.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sleep(600);
	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < 20000; i++)
	{
		auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

		Amplitude max = wav.MaxAmplitude(elapsedMilliseconds.count(), 2);
		
		rotovanaUsecka1.ZmenUhel(max.left * 60 );
		//rotovanaUsecka2.ZmenUhel(max.right * 360 / 2.0);
		
		platno.Vymaz();

		rotovanaUsecka1.Nakresli(platno);
		rotovanaUsecka2.Nakresli(platno);

		platno.Zobraz();
		printf("%20lld\n", elapsedMilliseconds.count());
	}

	int znak = getchar();
}
