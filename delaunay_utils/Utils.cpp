#include <string>
#include <iostream>
#include <fstream>

#include "common.h"

#include "Utils.h"
#include "Vertex.h"
#include "Edge.h"
#include "Triangle.h"

int Utils::recursiveDepth = log2(std::thread::hardware_concurrency());

inline bool Utils::VertexComparatorX(Vertex * A, Vertex * B)
{
	if (A->position.x < B->position.x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline bool Utils::VertexComparatorY(Vertex * A, Vertex * B)
{
	if (A->position.y < B->position.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline void Utils::SortByX(std::vector<Vertex*>& pointset)
{
	std::sort(pointset.begin(), pointset.end(), VertexComparatorX);
	
}

inline void Utils::SortByY(std::vector<Vertex*>& pointset)
{
	std::sort(pointset.begin(), pointset.end(), VertexComparatorY);
}
// funkcja sprawdza czy punkty p oraz cp leza po tej samej stronie linii
// wyznaczanej przez wektor f
// przypadek w ktorym punkt cp lub p leza na linii(f) moze powodowac problemy
// todo: funkcja powinna zwracac inna wartosc niz false/true np. (1,0,-1) w zaleznosci od znaku iloczynu iloczynow skalarnych.
inline int Utils::SameHalfspaceTest(Edge * f, Vertex * p, Vector &cp)
{
	Vector v(f->v2->position.x - f->v1->position.x, f->v2->position.y - f->v1->position.y);
	Vector w(v.y, -v.x);

	double resultP = w.DotProduct((p->position - f->v1->position));
	double resultCP = w.DotProduct((cp - f->v1->position));

	double result = resultP * resultCP;
	//return result;
	
	if (result < 0) // rozne strony linii
	{
		return -1;
	}
	else if (result > 0) // ta sama strona linii
	{
		return 1;
	}
	else // na linii
	{
		return 0;
	}
}


inline Vector Utils::CenterOfMass(Vertex * a, Vertex * b, Vertex * c)
{
	return Vector((a->position.x + b->position.x + c->position.x) / 3,
		(a->position.y + b->position.y + c->position.y) / 3);
}

// patrz wikipedia: algorytm jarvisa
std::vector<Edge*> Utils::ConvexHull(std::vector<Vertex*> &pointset)
{
	if (pointset.size() < 3)
	{
		return std::vector<Edge*>();
	}

	std::vector<Edge*> result;

	if (pointset.size() == 0)
	{
		return result;
	}

	// find vertex with minimal y
	double ymin = DBL_MAX;

	Vertex* current = pointset[0];
	Vertex* min = nullptr;
	ymin = current->position.y;

	for (int i = 0; i < pointset.size(); ++i)
	{
		current = pointset[i];
		if (current->position.y < ymin)
		{
			ymin = current->position.y;
			min = current;
		}
	}

	current = min;
	Vertex* candidate = nullptr;
	Vertex* best = nullptr;
	Vector u(1, 0); // wektor jednostkowy poczatkowy (tylko dla najwyzszego wierzcholka)
	double minAngle = DBL_MAX; // fltmax jest w tym przypadku bezpieczny i szybszy niz inicjalizacja
	Vector previousVector;

	// pierwsza krawedz	
	for (int i = 0; i < pointset.size(); ++i)
	{
		candidate = pointset[i];
		if (candidate == current) continue;

		Vector v(candidate->position.x - current->position.x, candidate->position.y - current->position.y);


		double angle = Vector::AngleBetween(u, v);
		if (angle < minAngle)
		{
			minAngle = angle;
			best = candidate;
			previousVector = v;
		}
	}
	Edge* e = new Edge(current, best);
	result.push_back(e);
	current = best;
	minAngle = DBL_MAX;

	Vector resultVector = previousVector;

	// wrap
	while (best != min)
	{
		for (int i = 0; i < pointset.size(); ++i)
		{
			candidate = pointset[i];
			if (candidate == current) continue;

			Vector v(candidate->position.x - current->position.x, candidate->position.y - current->position.y);

			double angle = Vector::AngleBetween(resultVector, v);
			if (angle < minAngle)
			{
				minAngle = angle;
				best = candidate;
				previousVector = v;
			}
		}

		e = new Edge(current, best);
		result.push_back(e);
		current = best;
		minAngle = DBL_MAX;
		resultVector = previousVector;
	}
	return result;
}

inline Triangle* Utils::MakeSimplex(Edge * f, std::vector<Vertex*>& pointset, double alfa, bool(*IsIntersected)(Vertex*, Vertex*, double))
{
	double minDD = DBL_MAX;
	Vertex* best = nullptr;

	for (register int i = 0; i < pointset.size(); ++i)
	{
		Vertex* p = pointset[i];

		if (p == f->v1 || p == f->v2)
		{
			continue;
		}

		// pomin te ktore leza po zlej stronie f lub na f
		if (Utils::SameHalfspaceTest(f, p, f->simplex_origin) >= 0)
		{
			continue;
		}

		double currentDD = Utils::DelaunayDistance(f, p);

		if ((currentDD < minDD))
		{
			minDD = currentDD;
			best = p;
		}
	}

	if (best != nullptr)
	{
		// buduj tylko sciane
		if (!IsIntersected(f->v1, best, alfa) && !IsIntersected(f->v2, best, alfa))
		{
			return nullptr;
		}

		Edge *e1, *e2;
		e1 = new Edge(f->v1, best);
		e2 = new Edge(f->v2, best);

		Vector origin(Utils::CenterOfMass(f->v1, f->v2, best));
		e1->setOrigin(origin);
		e2->setOrigin(origin);

		Triangle* t = new Triangle(e1, e2, f);

		return t;
	}
	else
	{
		return nullptr;
	}
}

struct special_compare : public std::unary_function<Edge*, bool>
{
	explicit special_compare(Edge* baseline) : baseline(baseline) {}

	bool operator() (Edge* arg)
	{
		return Utils::SameEdge(arg, baseline);
	}
	Edge* baseline;
};

// bezpieczna po wskazniku edge* e
inline void Utils::UpdateAFL(std::list<Edge*>& AFL, Edge* e)
{
	std::list<Edge*>::iterator it = std::find_if(AFL.begin(), AFL.end(), special_compare(e));
	if (it == AFL.end())
	{
		AFL.push_back(e);
	}
	else
	{
		AFL.remove(*it);
	}
}

inline void Utils::UpdateAFLBySide(std::list<Edge*>& AFL, std::list<Edge*>& AFL1, std::list<Edge*>& AFL2, Edge* f, double alfa, int (*WhichSideOfAlpha)(Edge*, double) )
{
	int side = WhichSideOfAlpha(f, alfa);
	if (side == 1)
	{
		Utils::UpdateAFL(AFL1, f);
	}
	else if (side == -1)
	{
		Utils::UpdateAFL(AFL2, f);
	}
	else
	{
		Utils::UpdateAFL(AFL, f);
	}
}

inline void Utils::PointsetPartitionX(std::vector<Vertex*>& pointset, double alfa, std::vector<Vertex*>& P1, std::vector<Vertex*>& P2)
{
	for each (Vertex* point in pointset)
	{
		if (point->position.x < alfa)
		{
			P1.push_back(point);
		}
		else if (point->position.x > alfa)
		{
			P2.push_back(point);
		}
	}
}

inline void Utils::PointsetPartitionY(std::vector<Vertex*>& pointset, double alfa, std::vector<Vertex*>& P1, std::vector<Vertex*>& P2)
{
	for each (Vertex* point in pointset)
	{
		if (point->position.y < alfa)
		{
			P1.push_back(point);
		}
		else if (point->position.y > alfa)
		{
			P2.push_back(point);
		}
	}
}

void Utils::dt_dewall(std::vector<Vertex*>& pointset, std::list<Edge*>& AFL, int recurrentCounter)
{
	double alfa;
	std::list<Edge*> AFLa;
	std::list<Edge*> AFL1;
	std::list<Edge*> AFL2;
	Triangle* t;

	std::vector<Vertex*> P1;
	std::vector<Vertex*> P2;

	int(*SideEval) (Edge*, double);

	bool(*IsIntersected)(Vertex*, Vertex*, double);

	if (recurrentCounter % 2 == 0)
	{
		Utils::SortByX(pointset);
		alfa = pointset.front()->position.x + (pointset.back()->position.x - pointset.front()->position.x) / 2;
		Vector pos1(alfa, 0);
		Vector pos2(alfa, 900);

		Utils::PointsetPartitionX(pointset, alfa, P1, P2);

		SideEval = Utils::WhichSideOfAlpha;
		IsIntersected = Utils::IsIntersected;
	}
	else
	{
		Utils::SortByY(pointset);
		alfa = pointset.front()->position.y + (pointset.back()->position.y - pointset.front()->position.y) / 2;
		Vector pos1(0, alfa);
		Vector pos2(1440, alfa);
	
		Utils::PointsetPartitionY(pointset, alfa, P1, P2);

		SideEval = Utils::WhichSideOfAlphaY;
		IsIntersected = Utils::IsIntersectedY;
	}


	if (AFL.empty())
	{
		t = Utils::MakeFirstSimplex(pointset, alfa);

		UpdateAFL(AFL, t->e0);
		UpdateAFL(AFL, t->e1);
		UpdateAFL(AFL, t->e2);
	}

	for each (Edge* e in AFL) 
	{
		UpdateAFLBySide(AFLa, AFL1, AFL2, e, alfa, SideEval);
	}
	
	while (!(AFLa.empty()))
	{
		Edge* e = AFLa.back();
		AFLa.pop_back();

		Triangle* t = MakeSimplex(e, pointset,alfa,IsIntersected);

		if (t != nullptr)
		{
			UpdateAFLBySide(AFLa, AFL1, AFL2, t->e0, alfa, SideEval);
			UpdateAFLBySide(AFLa, AFL1, AFL2, t->e1, alfa, SideEval);					
		}
	}
		
	if (recurrentCounter < Utils::recursiveDepth)
	{
		recurrentCounter += 1;

		std::vector<std::thread> threads;


		if (!AFL1.empty()) threads.push_back(std::thread(Utils::dt_dewall, P1, AFL1, recurrentCounter));
		if (!AFL2.empty()) threads.push_back(std::thread(Utils::dt_dewall, P2, AFL2, recurrentCounter));

		for (auto& th : threads)
			th.join();
	}
	else
	{
		recurrentCounter += 1;
		if (!AFL1.empty()) Utils::dt_dewall(P1, AFL1, recurrentCounter);
		if (!AFL2.empty()) Utils::dt_dewall(P2, AFL2, recurrentCounter);
	}	
}

inline Triangle* Utils::MakeFirstSimplex(std::vector<Vertex*>& pointset, double alfa)
{
	double minDist1 = DBL_MAX;
	double currentDistance;
	Vertex* currentVertex = nullptr, *p1 = nullptr;
	// MakeFirstSimplex:
	for (register int i = 0; i < pointset.size(); ++i)
	{
		currentVertex = pointset[i];
		currentDistance = fabs(currentVertex->position.x - alfa);

		if (currentDistance < minDist1)
		{
			minDist1 = currentDistance;
			p1 = currentVertex;
		}
	}

	bool rightSide;
	if (p1->position.x > alfa)
	{
		rightSide = true;
	}
	else
	{
		rightSide = false;
	}


	// znajdz p2 po drugiej stronie
	minDist1 = DBL_MAX;
	currentDistance = 0;
	currentVertex = nullptr;
	Vertex* p2 = nullptr;

	for (register int i = 0; i < pointset.size(); ++i)
	{
		currentVertex = pointset[i];
		//currentDistance = Utils::VectorMagnitude(&(currentVertex->position - p1->position));
		currentDistance = currentVertex->position.DistanceTo(p1->position);

		if (currentVertex->position.x - alfa < 0 && rightSide == false)
		{
			continue;
		}
		if (currentVertex->position.x - alfa >= 0 && rightSide == true)
		{
			continue;
		}

		if (currentDistance < minDist1)
		{
			minDist1 = currentDistance;
			p2 = currentVertex;
		}
	}

	// znajdz p3
	double minRadius = DBL_MAX;
	double currentRadius;
	currentVertex = nullptr;
	currentDistance = 0;
	Vertex* p3 = nullptr;
	Vector* circumcenter;

	for (register int i = 0; i < pointset.size(); ++i)
	{
		currentVertex = pointset[i];
		circumcenter = Utils::CircumCenter(p1->position, p2->position, currentVertex->position);
		if (circumcenter != nullptr)
		{
			//currentRadius = Utils::VectorMagnitude(&Vector(circumcenter->x - p1->position.x, circumcenter->y - p1->position.y));
			currentRadius = circumcenter->DistanceTo(p1->position);

			if (currentRadius < minRadius)
			{
				minRadius = currentRadius;
				p3 = currentVertex;
			}
		}
	}

	Edge* f = new Edge(p1, p3);
	Edge* f2 = new Edge(p2, p3);
	Edge* f3 = new Edge(p1, p2);

	Vector centerOfMass = Utils::CenterOfMass(p1, p2, p3);

	f->setOrigin(centerOfMass);
	f2->setOrigin(centerOfMass);
	f3->setOrigin(centerOfMass);

	return new Triangle(f, f2, f3);
}
// funkcja delaunay distance (dd) zdefiniowana w publikacji algorytmu DeWall
inline double Utils::DelaunayDistance(Edge * f, Vertex * p)
{
	Vector* circumcenter = Utils::CircumCenter(f->v1->position, f->v2->position, p->position);

	if (circumcenter != nullptr)
	{
		double radius = circumcenter->DistanceTo(p->position);

		if (SameHalfspaceTest(f, p, *circumcenter) >= 0)
		{
			return radius;
		}
		else
		{
			return -radius;
		}
	}
	else return DBL_MAX;
}



inline bool Utils::ContainsEdge(std::vector<Edge*>& edges, Edge* f)
{
	for (int i = 0; i < edges.size(); ++i)
	{
		if ((edges[i]->v1 == f->v1 && edges[i]->v2 == f->v2) || (edges[i]->v2 == f->v1 && edges[i]->v1 == f->v2))
		{
			return true;
		}
	}
	return false;
}

inline bool Utils::SameEdge(Edge* A, Edge* B)
{
	if (A == B) return true;

	if ((A->v1 == B->v1 && A->v2 == B->v2) || (A->v1 == B->v2 && A->v2 == B->v1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline bool Utils::IsIntersected(Vertex* v1, Vertex* v2, double alfa)
{
	if (v1->position.x < alfa && v2->position.x >= alfa)
	{
		return true;
	}
	else if (v2->position.x < alfa && v1->position.x >= alfa)
	{
		return true;
	}
	else
	{
		return false;
	}
}
inline bool Utils::IsIntersectedY(Vertex* v1, Vertex* v2, double alfa)
{
	if (v1->position.y < alfa && v2->position.y >= alfa)
	{
		return true;
	}
	else if (v2->position.y < alfa && v1->position.y >= alfa)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline int Utils::WhichSideOfAlpha(Edge* f, double alfa)
{
	Vertex* v1 = f->v1;
	Vertex* v2 = f->v2;
	if (v1->position.x < alfa && v2->position.x < alfa)
	{
		return 1;
	}
	else if (v2->position.x > alfa && v1->position.x > alfa)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

inline int Utils::WhichSideOfAlphaY(Edge* f, double alfa)
{
	Vertex* v1 = f->v1;
	Vertex* v2 = f->v2;
	if (v1->position.y < alfa && v2->position.y < alfa)
	{
		return 1;
	}
	else if (v2->position.y > alfa && v1->position.y > alfa)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

void  Utils::EvaluateTriangle(int i, int j, int k, std::vector<Vertex*>& pointset)
{
	// Indeks 'w' oznacza aktualnie sprawdzany wierzchołek.
	// Sprawdzamy czy znajduje się on wewnątrz koła opisanego na i,j,k.
	for (int w = 0; w < pointset.size(); w++)
	{
		// jeżeli aktualnie sprawdzany wierzchołek jest jednym z wierzchołków trójkąta
		// wtedy szukamy dalej.
		if (w == i) continue;
		if (w == j) continue;
		if (w == k) continue;

		// Funkcja Circumcenter
		// zwraca środek okręgu opisanego na trójkącie ijk i wrzuca rezultat do zmiennej
		// circumcenter.
		Vector* circumcenter = Utils::CircumCenter(pointset[i]->position, pointset[j]->position, pointset[k]->position);
		if (circumcenter == nullptr) return;

		// policz promień koła opisanego na trójkącie ijk.
		double diffX = circumcenter->x - pointset[i]->position.x;
		double diffY = circumcenter->y - pointset[i]->position.y;
		double distance = Vector(diffX, diffY).Magnitude();

		// policz odległość wierzchołka sprawdzanego 'w' do środka okręgu.
		double wDiffX = circumcenter->x - pointset[w]->position.x;
		double wDiffY = circumcenter->y - pointset[w]->position.y;
		double wDistance = Vector(wDiffX, wDiffY).Magnitude();

		// dodaj kółko do listy w razie gdybyśmy chcieli je obejrzeć na ekranie. (Nieistotne)
		// circumcircle* c = new circumcircle(circumcenter, distance);

		// jeżeli odległość punktu sprawdzanego 'w' od środka koła opisanego jest mniejsza niż
		// promień koła, to odrzuć trójkąt.
		if (wDistance < distance)
		{
			// odrzuć trójkąt
			return;
		}
	}
	Edge* e0 = new Edge(pointset[i], pointset[j]);
	Edge* e1 = new Edge(pointset[j], pointset[k]);
	Edge* e2 = new Edge(pointset[k], pointset[i]);
}


std::vector<Edge*> Utils::dt_bruteforce(std::vector<Vertex*>& pointset)
{
	for (int i = 0; i < pointset.size() - 2; ++i)
	{
		for (int j = i + 1; j < pointset.size() - 1; ++j)
		{
			for (int k = j + 1; k < pointset.size(); ++k)
			{
				Utils::EvaluateTriangle(i, j, k, pointset);
			}
		}
	}
	return std::vector<Edge*>();
}

// srodek kola opisanego na trojkacie abc
inline Vector* Utils::CircumCenter(Vector& a, Vector& b, Vector& c)
{
	double A, B, C, D, E, F, G;

	A = b.x - a.x;
	B = b.y - a.y;
	C = c.x - a.x;
	D = c.y - a.y;
	E = A * (a.x + b.x) + B * (a.y + b.y);
	F = C * (a.x + c.x) + D * (a.y + c.y);
	G = 2.0 * (A * (c.y - b.y) - B * (c.x - b.x));
	if (G == 0)
	{
		//return Vector(0, 0);
		return nullptr;
	}
	else
	{
		double Px = (D * E - B * F) / G;
		double Py = (A * F - C * E) / G;

		// tutaj zwraca wartość jako punkt.
		return new Vector(Px, Py);
	}
}