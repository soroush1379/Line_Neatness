#include <iostream>
#include <cmath>
#include <SDL.h>
#include <vector>
#include <string>

using namespace std;
#undef main

template <class T>
T max(T a, T b)
{
	return ((a >= b) ? a : b);
}

struct Point {
	double x, y;

	Point(double x, double y) :
		x(x), y(y)
	{
	}
};

void Initialize(int n);
void AnalyzePicture();
void DrawBestFit();
void DrawLine(vector<Point> p, int r, int g, int b, int a);
void WaitForKey();
long double StandardDeviation(int num);
Uint32 getPixel(SDL_Surface *surface, int x, int y);

SDL_Window *windowMain;
SDL_Surface *surfaceMain;
SDL_Surface *surfaceImage;
SDL_Renderer *rendererMain;
SDL_Event e;
vector<Point> MainLine, BestFit;
long double m, n;
string address = "Debug/Data/*.bmp";

int main()
{
	cout << "GROUP T015\nPRAXIS\nUOFT ENGSCI 2T2" << endl;
	string s;
	getline(cin, s);
	int x = 1;

	Initialize(x);
	WaitForKey();

	AnalyzePicture();
	WaitForKey();

	DrawBestFit();
	WaitForKey();

	SDL_Quit();

	StandardDeviation(x);

	getline(cin, s);
	return 0;
}

void Initialize(int n)
{
	SDL_Init(SDL_INIT_VIDEO);

	windowMain = SDL_CreateWindow("Cube", 20, 50, 1500, 750, SDL_WINDOW_SHOWN);
	surfaceMain = SDL_GetWindowSurface(windowMain);
	rendererMain = SDL_CreateRenderer(windowMain, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetWindowTitle(windowMain, "Line Neatness Test | Praxis");

	string add = address;
	add.replace(address.find('*'), 1, to_string(n));
	
	surfaceImage = SDL_LoadBMP(add.c_str());

	SDL_BlitSurface(surfaceImage, NULL, surfaceMain, NULL);
	SDL_UpdateWindowSurface(windowMain);
}

void AnalyzePicture()
{
	int count = 0, sum;


	for (int i = 0; i < surfaceImage->w; i++)
	{
		sum = count = 0;
		for (int j = 0; j < surfaceImage->h; j++)
		{
			if (getPixel(surfaceImage, i, j) >= 0x000000 && getPixel(surfaceImage, i, j) <= 0x999999)
			{
				sum += j;
				count++;
			}
		}

		if (count > 0)
			MainLine.push_back(Point(i, sum / count));
	}

	DrawLine(MainLine, 255, 255, 255, 255);
}

void DrawBestFit()
{
	double xmin = MainLine[0].x, xmax = (MainLine.end() - 1)->x;

	int count = 200, actualCount = 0;
	long double a, b, c, d, e, f;
	a = b = c = e = f = 0;

	for (long long i = 0; i < MainLine.size(); i += (MainLine.size() / count == 0) ? 1 : MainLine.size() / count)
	{
		actualCount++;
		a += MainLine[i].x * MainLine[i].x;
		b += MainLine[i].x;
		e += MainLine[i].x * MainLine[i].y;
		f += MainLine[i].y;
	}
	c = b;
	d = actualCount;

	m = (e * d - b * f) / (a * d - b * c);
	n = (-e * c + f * a) / (a * d - b * c);

	for (int i = xmin; i < xmax + 1; i++)
	{
		BestFit.push_back(Point(i, m * i + n));
	}
	DrawLine(BestFit, 0, 255, 0, 150);
	DrawLine(MainLine, 255, 255, 255, 255);
}

void DrawLine(vector<Point> p, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(rendererMain, r, g, b, a);
	for (int i = 0; i < p.size(); i++)
	{
		SDL_RenderDrawPoint(rendererMain, (int)p[i].x, (int)p[i].y);
	}
	SDL_RenderPresent(rendererMain);
}

Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;
	}
}

void WaitForKey()
{
	while (true)
	{
		SDL_PollEvent(&e);
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
			break;
	}
}

long double StandardDeviation(int num)
{
	long double k = 0;
	for (int i = 0; i < MainLine.size(); i++)
	{
		k += pow((m * MainLine[i].x + n - MainLine[i].y), 2) / MainLine.size();
	}

	k = sqrt(k);

	cout.precision(3);
	cout << num << ": The standard deviation for the points of this line is: " << k << endl;
	return k;
}