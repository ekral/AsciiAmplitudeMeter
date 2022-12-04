#pragma once

#include <cstdint>
#include <fstream>
#include <cmath>

// https://blog.demofox.org/2015/04/14/decibels-db-and-amplitude/
static double ValueToDb(double amplitude)
{
	//if (amplitude <= 0.1) return -20;

	double dB = 20 * std::log10(amplitude);

	return dB;
}

static double SampleToAmplitude(int16_t sample)
{
	double amplitude = ((double)(sample) / -INT16_MIN) + 1.0;
	return amplitude;
}

struct Amplitude
{
	double left;
	double right;
	
	Amplitude() : left(0.0), right(0.0)
	{

	}
};

struct Wav
{
	Wav(Wav&) = delete;

	Wav() : data(nullptr)
	{
	}

	uint8_t riffChunkId[4]{};
	uint32_t riffChunkSize{};
	uint8_t riffFormat[4]{};

	uint8_t fmtSubchunk1Id[4]{};
	uint32_t fmtSubchunk1Size{};
	uint16_t fmtAudioFormat{};
	uint16_t fmtNumChannels{};
	uint32_t fmtSampleRate{};
	uint32_t fmtByteRate{};
	uint16_t fmtBlockAlign{};
	uint16_t fmtBitsPerSample{};

	uint8_t id[4]{};
	uint32_t size{};
	uint8_t* data;
	uint32_t framesCount{};

	Amplitude MaxAmplitude(size_t timeMilliseconds, size_t widthMilliseconds)
	{
		Amplitude amplitude;
		amplitude.left = 0;
		amplitude.right = 0;

		size_t offset = (timeMilliseconds * fmtSampleRate) / 1000;
		
		if (offset >= framesCount)
		{
			return amplitude;
		}
		
		size_t width = (widthMilliseconds * fmtSampleRate) / 1000;
		size_t end = (offset + width);

		if (end >= framesCount)
		{
			end = framesCount;
		}

		int16_t* const ptBegin = reinterpret_cast<int16_t*>(data);
		int16_t* const ptStart = ptBegin + offset;
		int16_t* const ptEnd = ptBegin + end;

		for (int16_t* p = ptStart; p < ptEnd;)
		{
			int16_t leftSample = *p;
			++p;

			int16_t rightSample = *p;
			++p;

			double left = SampleToAmplitude(leftSample);
			double right = SampleToAmplitude(rightSample);

			amplitude.left = std::max<double>(amplitude.left, left);
			amplitude.right = std::max<double>(amplitude.right, right);
		}

		return amplitude;
	}

	bool LoadWavFile(const char* filePath)
	{
		FILE* fp;

		errno_t err = fopen_s(&fp, filePath, "rb");

		if (fp == nullptr)
		{
			return false;
		}

		size_t count = 0;

		count = fread(&riffChunkId[0], sizeof(riffChunkId), 1, fp);
		if (count != 1) goto exit;

		count = fread(&riffChunkSize, sizeof(riffChunkSize), 1, fp);
		if (count != 1) goto exit;

		count = fread(&riffFormat[0], sizeof(riffFormat), 1, fp);
		if (count != 1) goto exit;

		count = fread(&fmtSubchunk1Id[0], sizeof(fmtSubchunk1Id), 1, fp);
		if (count != 1) goto exit;

		count = fread(&fmtSubchunk1Size, sizeof(fmtSubchunk1Size), 1, fp);
		if (count != 1) goto exit;

		count = fread(&fmtAudioFormat, sizeof(fmtAudioFormat), 1, fp);
		if (count != 1) goto exit;

		count = fread(&fmtNumChannels, sizeof(fmtNumChannels), 1, fp);
		if (count != 1) goto exit;

		count = fread(&fmtSampleRate, sizeof(fmtSampleRate), 1, fp);
		if (count != 1) goto exit;

		count = fread(&fmtByteRate, sizeof(fmtByteRate), 1, fp);
		if (count != 1) goto exit;

		count = fread(&fmtBlockAlign, sizeof(fmtBlockAlign), 1, fp);
		if (count != 1) goto exit;

		count = fread(&fmtBitsPerSample, sizeof(fmtBitsPerSample), 1, fp);
		if (count != 1) goto exit;

		do
		{
			count = fread(&id[0], sizeof(id), 1, fp);
			if (count != 1) goto exit;

			count = fread(&size, sizeof(size), 1, fp);
			if (count != 1) goto exit;

			if (id[0] == 'd' && id[1] == 'a' && id[2] == 't' && id[3] == 'a')
			{
				int n = size;
				data = new uint8_t[n];

				count = fread(data, sizeof(uint8_t), n, fp);
				if (count != size) goto exit;
			}
			else
			{
				int count = fseek(fp, size, SEEK_CUR);
				if (count != size) return false;
			}

		} while (data == nullptr);

	exit:
		fclose(fp);

		if (data != nullptr)
		{
			framesCount = size / fmtBlockAlign;
			return true;
		}
		
		return false;
	}

	~Wav()
	{
		if (data != nullptr)
		{
			if (data != nullptr) delete[] data;
		}
	}
};

