#include "Noise.h"

Noise::Noise(float f, float a, float l) {
    frequency_ = f;
    amplitude_ = a;
    lacunarity_ = l;
    persistence_ = 1 / l;
    octaves_ = 1;
	srand(time(NULL));
	newPermutation();
}

Noise::~Noise() {

}

float Noise::perlin(float x, float y, float z) {
    int xi = (int)floor(x) & 255;
    int yi = (int)floor(y) & 255;
    int zi = (int)floor(z) & 255;

    float xf = x - floor(x);
    float yf = y - floor(y);
    float zf = z - floor(z);

    float u = fade(xf);
    float v = fade(yf);
    float w = fade(zf);

    int i0 = permutation_[permutation_[xi] + yi] + zi;
    int i1 = permutation_[permutation_[xi + 1] + yi] + zi;
    int i2 = permutation_[permutation_[xi] + yi + 1] + zi;
    int i3 = permutation_[permutation_[xi + 1] + yi + 1] + zi;

    float g000 = grad(permutation_[i0], xf, yf, zf);
    float g001 = grad(permutation_[i0 + 1], xf, yf, zf - 1);
    float g100 = grad(permutation_[i1], xf - 1, yf, zf);
    float g101 = grad(permutation_[i1 + 1], xf - 1, yf, zf - 1);
    float g010 = grad(permutation_[i2], xf, yf - 1, zf);
    float g011 = grad(permutation_[i2 + 1], xf, yf - 1, zf - 1);
    float g110 = grad(permutation_[i3], xf - 1, yf - 1, zf);
    float g111 = grad(permutation_[i3 + 1], xf - 1, yf - 1, zf - 1);

    float l0 = linearInterp(u, g000, g100);
    float l1 = linearInterp(u, g010, g110);
    float l2 = linearInterp(u, g001, g101);
    float l3 = linearInterp(u, g011, g111);
    float l4 = linearInterp(v, l0, l1);
    float l5 = linearInterp(v, l2, l3);

    return linearInterp(w, l4, l5);
}

float Noise::recursivePerlin(float x, float y, float z) {
    float total = 0.0f;
    float denom = 0.0f;
    float f = frequency_;
    float a = amplitude_;

    for (int i = 0; i < octaves_; ++i) {
        total += a * perlin(x * f, y * f, z * f);
        denom += a;

        f *= lacunarity_;
        a *= persistence_;
    }

    return amplitude_ * total / denom;
}

void Noise::newPermutation() {
	// initialize permutation
	for (int i = 0; i < 256; ++i) {
		permutation_[i] = i;
        permutation_[i + 256] = i;
	}
	// randomize permutation
	for (int i = 0; i < 256; ++i) {
		int temp = permutation_[i];
		int j = rand() % 256;
		permutation_[i] = permutation_[j];
        permutation_[i + 256] = permutation_[j + 256];
		permutation_[j] = temp;
        permutation_[j + 256] = temp;
	}
}

float Noise::linearInterp(float t, float a, float b) {
    return a * (1 - t) + b * t;
}

float Noise::cosineInterp(float t, float a, float b) {
    float f = (1 - cos(t * M_PI)) / 2;
    return a * (1 - f) + b * f;
}

float Noise::fade(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float Noise::grad(int h, float x, float y) {
	int i = h & 0x3F;
	float u = i < 4 ? x : y;
	float v = i < 4 ? y : x;
	return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

float Noise::grad(int h, float x, float y, float z) {
    int i = h & 15;
    float u = i < 8 ? x : y;
    float v = i < 4 ? y : i == 12 || i == 14 ? x : z;
    return ((i & 1) == 0 ? u : -u) + ((i & 2) == 0 ? v : -v);
}

void Noise::setFrequency(float f) {
    frequency_ = f;
}

void Noise::setAmplitude(float a) {
    amplitude_ = a;
}

void Noise::setLacunarity(float l) {
    lacunarity_ = l;
    persistence_ = 1 / l;
}

void Noise::setOctaves(int o) {
    octaves_ = o;
}