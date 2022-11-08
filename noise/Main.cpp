#include "Noise.h"
#include "olcPixelGameEngine.h"

class Test : public olc::PixelGameEngine
{
	Perlin noise;
	bool change = false;
	int o = 8;
	float b = 2;
public:
	Test() : PixelGameEngine()
	{
		sAppName = "Noise";
	}
private:
	bool OnUserCreate() override
	{
		noise = { (unsigned int)ScreenWidth(), (unsigned int)ScreenHeight(),nullptr,o,b };
		return true;
	}
	bool OnUserUpdate(float ts) override
	{
		if (GetKey(olc::W).bReleased) { o++; change = true; }
		if (GetKey(olc::S).bReleased) { o--; change = true; }
		if (GetKey(olc::A).bReleased) { b++; change = true; }
		if (GetKey(olc::D).bReleased) { b--; change = true; }
		if (change)
		{
			change = false;
			noise = { (unsigned int)ScreenWidth(), (unsigned int)ScreenHeight(), nullptr,o,b };
			Clear(olc::BLACK);
			float v;
			for (unsigned int x = 0; x < ScreenWidth(); x++)
			{
				for (unsigned int y = 0; y < ScreenWidth(); y++)
				{
					v = (noise.Get(x, y) + 1.0f) * 255 * 0.5f;
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