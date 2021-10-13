#pragma once

#define _USE_MATH_DEFINES

#include <time.h>
#include <stdlib.h>
#include <math.h>

class Noise {
public:
	Noise(float, float, float);
	~Noise();

	float perlin(float, float, float);
	float recursivePerlin(float, float, float);

	void newPermutation();

	void setFrequency(float);
	void setAmplitude(float);
	void setLacunarity(float);
	void setOctaves(int);

protected:
private:
	float linearInterp(float, float, float);
	float cosineInterp(float, float, float);

	float fade(float);
	float grad(int, float, float);
	float grad(int, float, float, float);

	int permutation_[512];
	float frequency_, amplitude_, lacunarity_, persistence_;
	int octaves_;
};