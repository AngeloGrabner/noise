#pragma once
#include <limits>
#include <random>

class Perlin
{
	unsigned int mWidth = 0, mHeight = 0;
	float *mMap = nullptr, *mSeed = nullptr;
	bool ownSeed = false;

public:
	Perlin(unsigned int width, unsigned int height, float* seed, int octaves, float bias)
		: mWidth(width), mHeight(height)
	{
		delete[] mMap;
		mMap = new float[width * height];
		if (seed == nullptr)
		{
			ownSeed = true;
			mSeed = new float[width * height];
			for (int i = 0; i < width * height; i++)
			{
				mSeed[i] = (float)rand() / (float)RAND_MAX;
			}
		}
		else
		{
			mSeed = seed;
		}
		if (bias <= 0.0f)
			bias = 1.0f;
		Generate(mSeed, octaves, bias);
	}
	Perlin() = default;
	Perlin(Perlin&) = delete;
	Perlin(Perlin&&) = delete;
	~Perlin()
	{
		delete[] mMap;
		if (ownSeed)
		{
			delete[] mSeed;
		}
	}
	const Perlin& operator=(const Perlin& other)
	{
		if (this == &other)
			return *this;
		mWidth = other.mWidth;
		mHeight = other.mHeight;

		if (other.mMap)
		{
			mMap = new float[other.mWidth * other.mHeight];
			for (int i = 0; i < other.mWidth * other.mHeight; i++)
			{
				mMap[i] = other.mMap[i];
			}
		}
		if (other.mSeed)
		{
			ownSeed = true;
			mSeed = new float[other.mWidth * other.mHeight];
			for (int i = 0; i < other.mWidth * other.mHeight; i++)
			{
				mSeed[i] = other.mSeed[i];
			}
		}
		else ownSeed = false;
		return *this;
	}
	inline float Get(unsigned int x, unsigned int y)
	{
		if (x >= mWidth)
			x = mWidth - 1;
		if (y >= mHeight)
			y = mHeight - 1;
		return mMap[x + mWidth * y];
	}
private:
	void Generate(float* seed, int octaves, float bias)
	{
		if (octaves > (int)std::log2(mWidth))
			octaves = (int)std::log2(mWidth);
		if (seed != nullptr)
		for (int x = 0; x < mWidth; x++)
			for (int y = 0; y < mHeight; y++)
			{
				float fNoise = 0.0f;
				float fScaleAcc = 0.0f;
				float fScale = 1.0f;

				for (int o = 0; o < octaves; o++)
				{
					int nPitch = mWidth >> o;
					int nSampleX1 = (x / nPitch) * nPitch; // devision by 0
					int nSampleY1 = (y / nPitch) * nPitch;

					int nSampleX2 = (nSampleX1 + nPitch) % mWidth;
					int nSampleY2 = (nSampleY1 + nPitch) % mHeight;

					float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
					float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

					float fSampleT = (1.0f - fBlendX) * seed[nSampleY1 * mWidth + nSampleX1] + fBlendX * seed[nSampleY1 * mWidth + nSampleX2];
					float fSampleB = (1.0f - fBlendX) * seed[nSampleY2 * mWidth + nSampleX1] + fBlendX * seed[nSampleY2 * mWidth + nSampleX2];

					fScaleAcc += fScale;
					fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
					fScale = fScale / bias;
				}
				mMap[y * mWidth + x] = fNoise / fScaleAcc;
			}
	}
};