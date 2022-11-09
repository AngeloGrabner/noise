#include "Noise.h"
#include "olcPixelGameEngine.h"

class Test : public olc::PixelGameEngine
{
	Perlin noise;
	bool change = false;
	int o = 8;
	float b = 1;
	float* seed; // leaking memory LLL
public:
	Test() : PixelGameEngine()
	{
		sAppName = "Noise";
	}
private:
	bool OnUserCreate() override
	{
		auto temp = ScreenHeight() * ScreenWidth();
		seed = new float[temp];
		for (int i = 0; i < temp; i++)
		{
			//seed[i] = (float)rand() / (float)RAND_MAX;
			seed[i] = 2.0f*((float)rand() / (float)RAND_MAX)-1.0f;
		}
		return true;
	}
	bool OnUserUpdate(float ts) override
	{
		if (GetKey(olc::W).bReleased) { o++; change = true; }
		if (GetKey(olc::S).bReleased) { o--; change = true; }
		if (GetKey(olc::A).bReleased) { b+=0.2f; change = true; }
		if (GetKey(olc::D).bReleased) { b-=0.2f; change = true; }
		if (change)
		{
			change = false;
			noise = { (unsigned int)ScreenWidth(), (unsigned int)ScreenHeight(), seed,o,b };
			Clear(olc::BLACK);
			float v;
			for (unsigned int x = 0; x < ScreenWidth(); x++)
			{
				for (unsigned int y = 0; y < ScreenWidth(); y++)
				{
					//std::cout << noise.Get(x, y) << ' ';
					v = noise.Get(x, y) * 255.0f ;
					Draw({ (int)x,(int)y }, olc::Pixel(v, v, v));
				}
			}
			DrawString({ 0,0 }, "bias: " + std::to_string(b) + " octaves: " + std::to_string(o), olc::RED, 2.5f);
		}
			return true;
	}
};


int main()
{
	Test test;
	if (test.Construct(1600, 900, 1, 1))
		test.Start();
}