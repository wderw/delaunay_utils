#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "dll_import.h"

#define CUSTOM_RED 255,46,109
#define CUSTOM_BLACK 15,15,15
#define CUSTOM_GREEN 35,220,35
#define CUSTOM_WHITE_ALPHA 255,255,255,220

#define VERTEX_SNAP_DISTANCE 15.0

int main()
{
	//int cores = std::thread::hardware_concurrency();
	//int recurrenceDepth = log2(cores);
	//printf("recurrenceDepth: %d", recurrenceDepth);

	time_t randomSeed = time(NULL);
	srand(randomSeed);
	printf("seed: %d\n", randomSeed);

	// zaladuj biblioteke
	HINSTANCE hGetProcIDDLL = LoadLibrary("delaunay_utils.dll");

	if (!hGetProcIDDLL)
	{
		std::cout << "could not load the dynamic library" << std::endl;
		return EXIT_FAILURE;
	}

	auto delaunay_dc = (pDelaunayDC)GetProcAddress(hGetProcIDDLL, "delaunay_dc");
	if (!delaunay_dc)
	{
		std::cout << "could not locate the function" << std::endl;
		return EXIT_FAILURE;
	}

	/* ----------------------- test biblioteki ----------------------*/
	const int vertexCount = 112;
	point_t* points = new point_t[vertexCount];

	for (int i = 0; i < vertexCount; ++i)
	{
		double quakex = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double quakey = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double quakez = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		points[i].x = quakex + rand() % 1000;
		points[i].y = quakey + rand() % 1000;
		points[i].z = 1.0;// quakez + rand() % 1000;
	}

	clock_t begin = clock();
	int resultsize = -1;
	double volume = 6.66;
	triangle_t* result = delaunay_dc(points, vertexCount, resultsize, volume);
	std::cout << "resultsize: " << resultsize << std::endl;
	std::cout << "volume: " << volume << std::endl;
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000;
	std::cout << "elapsed time: " << elapsed_secs << std::endl;
	/* -------------------------------------------------------------*/

	// zaladuj fonty
	sf::Font font;
	if (!font.loadFromFile("cour.ttf"))
	{
		MessageBoxA(NULL, "Cannot load font file!", "Error", MB_ICONERROR | MB_OK);
	}

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1440, 900), "SFML Window One", sf::Style::Default, settings);

	// vsync
	window.setVerticalSyncEnabled(true);

	sf::Vector2u windowSize = window.getSize();

	// tekst pozycji kursora
	sf::Text mousePosText;
	mousePosText.setFont(font);
	mousePosText.setString("Mouse world position.");
	mousePosText.setCharacterSize(18);
	mousePosText.setFillColor(sf::Color::White);
	mousePosText.setStyle(sf::Text::Bold | sf::Text::Regular);
	mousePosText.setPosition(static_cast<double>(mousePosText.getCharacterSize()), static_cast<double>(windowSize.y) - mousePosText.getCharacterSize() * 2);

	/* przesun widok w konkretne miejsce:
	sf::View view;
	view.move(5.56804e+06, 7.50466e+06);
	window.setView(view);
	*/
	
	// petla komunikatow i obsluga zdarzen
	double moveValue = 1.0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
				{
					window.close();
					break;
				}
				case sf::Event::KeyPressed:
				{

					if (event.key.code == sf::Keyboard::F)
					{
						moveValue *= 0.5;
					}
					else if (event.key.code == sf::Keyboard::G)
					{
						moveValue *= 2.0;
					}
					else if (event.key.code == sf::Keyboard::F1)
					{
						sf::View view = window.getView();
						view.zoom(0.5f);
						moveValue *= 0.5;
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::F2)
					{
						sf::View view = window.getView();
						view.zoom(2.0);
						moveValue *= 2.0;
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::W)
					{
						sf::View view = window.getView();
						view.move(0, -moveValue);
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::S)
					{
						sf::View view = window.getView();
						view.move(0, moveValue);
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::A)
					{
						sf::View view = window.getView();
						view.move(-moveValue, 0);
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::D)
					{
						sf::View view = window.getView();
						view.move(moveValue,0);
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::Escape)
					{
						window.close();
						return 0;
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}

		// rysowanie
		window.clear(sf::Color(CUSTOM_BLACK));

		// wyswietl informacje o aktualnie podswiedlonych wierzcholkach
		size_t n = std::count(mousePosText.getString().begin(), mousePosText.getString().end(), '\n');
		mousePosText.setPosition(mousePosText.getPosition().x, window.getSize().y - mousePosText.getCharacterSize()*n - mousePosText.getCharacterSize() * 2);

		window.draw(mousePosText);
		window.display();
	}
	return 0;
}