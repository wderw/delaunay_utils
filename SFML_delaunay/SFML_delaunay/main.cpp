#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <thread>

#include "dll_import.h"

#define CUSTOM_RED 255,46,109
#define CUSTOM_BLACK 15,15,15
#define CUSTOM_GREEN 35,220,35
#define CUSTOM_WHITE_ALPHA 255,255,255,220

#define VERTEX_SNAP_DISTANCE 15.0

int main()
{
	int cores = std::thread::hardware_concurrency();
	int recurrenceDepth = static_cast<int>(log2(cores));
	printf("recurrenceDepth: %d\n", recurrenceDepth);

	time_t randomSeed = time(NULL);
	srand(randomSeed);
	printf("seed: %I64d\n", randomSeed);	

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
	const int vertexCount = 400;
	point_t* points = new point_t[vertexCount];

	/* wczytaj punkty z pliku 
	std::ifstream file;
	file.open("delaunay.xyz", std::ios::in);
	for (int i = 0; i < vertexCount; i++)
	{
		file >> points[i].x;
		file >> points[i].y;
		file >> points[i].z;
	}
	*/

	/* losuj punkty */
	for (int i = 0; i < vertexCount; ++i)
	{
		double quakex = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double quakey = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double quakez = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		points[i].x = quakex + rand() % 1000;
		points[i].y = quakey + rand() % 1000;
		points[i].z = rand() % 255 + 200;// quakez + rand() % 1000;
	}	

	/* zmierz czas wykonania */	
	int resultsize = -1;
	double volume = 6.66;

	clock_t begin = clock();
	triangle_t* result = delaunay_dc(points, vertexCount, resultsize, volume);
	clock_t end = clock();

	std::cout << "resultsize: " << resultsize << std::endl;
	std::cout << "volume: " << volume << std::endl;
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000;
	std::cout << "elapsed time: " << elapsed_secs << std::endl;


	/* wypisz wyniki do pliku: plik trzeba wczesniej usunac (TODO: tryb overwrite) 
	std::ofstream myfile;
	myfile.open("triangles.txt", std::ios::out);

	for (int i = 0; i < resultsize; i++)
	{
		myfile << std::fixed;
		myfile << result[i].x1 << std::endl;
		myfile << result[i].y1 << std::endl;
		myfile << result[i].z1 << std::endl;
		myfile << result[i].i1 << std::endl;

		myfile << result[i].x2 << std::endl;
		myfile << result[i].y2 << std::endl;
		myfile << result[i].z2 << std::endl;
		myfile << result[i].i2 << std::endl;

		myfile << result[i].x3 << std::endl;
		myfile << result[i].y3 << std::endl;
		myfile << result[i].z3 << std::endl;
		myfile << result[i].i3 << std::endl;
	}
	myfile.close();
	*/

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

	/* przesun widok w konkretne miejsce
	sf::View view;
	view.move(points[0].x, points[0].y);
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

		/* wyswietl trojkaty po wysokosci Z 
		sf::VertexArray triangle(sf::Triangles, 3);

		for (int i = 0; i < resultsize; i++)
		{
			triangle[0].position = sf::Vector2f(result[i].x1, result[i].y1);
			triangle[1].position = sf::Vector2f(result[i].x2, result[i].y2);
			triangle[2].position = sf::Vector2f(result[i].x3, result[i].y3);


			triangle[0].color = sf::Color(result[i].z1, result[i].z1, result[i].z1);
			triangle[1].color = sf::Color(result[i].z2, result[i].z2, result[i].z2);
			triangle[2].color = sf::Color(result[i].z3, result[i].z3, result[i].z3);

			window.draw(triangle);
		}
		*/


		/* wyswietl krawedzie trojkatow po wspolrzednych (V1)
		for (int i = 0; i < resultsize; i++)
		{
			sf::Vertex line0[] =
			{
				sf::Vertex(sf::Vector2f(result[i].x1, result[i].y1),sf::Color::White),
				sf::Vertex(sf::Vector2f(result[i].x2, result[i].y2),sf::Color::White)
			};

			sf::Vertex line1[] =
			{
				sf::Vertex(sf::Vector2f(result[i].x1, result[i].y1),sf::Color::White),
				sf::Vertex(sf::Vector2f(result[i].x3, result[i].y3),sf::Color::White)
			};

			sf::Vertex line2[] =
			{
				sf::Vertex(sf::Vector2f(result[i].x2, result[i].y2),sf::Color::White),
				sf::Vertex(sf::Vector2f(result[i].x3, result[i].y3),sf::Color::White)
			};

			window.draw(line0, 2, sf::Lines);
			window.draw(line1, 2, sf::Lines);
			window.draw(line2, 2, sf::Lines);
		}			
		*/

		/* wyswietl krawedzie trojkatow po indeksach (V2) */
		for (int i = 0; i < resultsize; i++)
		{
			sf::Vertex line0[] =
			{
				sf::Vertex(sf::Vector2f(points[result[i].i1].x, points[result[i].i1].y),sf::Color::White),
				sf::Vertex(sf::Vector2f(points[result[i].i2].x, points[result[i].i2].y),sf::Color::White)
			};

			sf::Vertex line1[] =
			{
				sf::Vertex(sf::Vector2f(points[result[i].i2].x, points[result[i].i2].y),sf::Color::White),
				sf::Vertex(sf::Vector2f(points[result[i].i3].x, points[result[i].i3].y),sf::Color::White)
			};

			sf::Vertex line2[] =
			{
				sf::Vertex(sf::Vector2f(points[result[i].i3].x, points[result[i].i3].y),sf::Color::White),
				sf::Vertex(sf::Vector2f(points[result[i].i1].x, points[result[i].i1].y),sf::Color::White)
			};

			window.draw(line0, 2, sf::Lines);
			window.draw(line1, 2, sf::Lines);
			window.draw(line2, 2, sf::Lines);
		}

		// wyswietl informacje o aktualnie podswiedlonych wierzcholkach
		size_t n = std::count(mousePosText.getString().begin(), mousePosText.getString().end(), '\n');
		mousePosText.setPosition(mousePosText.getPosition().x, window.getSize().y - mousePosText.getCharacterSize()*n - mousePosText.getCharacterSize() * 2);

		window.draw(mousePosText);
		window.display();
	}
	return 0;
}