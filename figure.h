#ifndef _FIGURE_
#define _FIGURE_
#include<iostream>
#include<cmath>
#include <istream>

struct Point{
	double x, y;
	Point(double x=0, double y=0)
	  : x(x),
		y(y)
		{}
	static double dot(const Point &a, const Point &b)
		{return (a.x*b.x + a.y*b.y);}
	static double cross(const Point &a, const Point &b)
		{return (a.x*b.y - a.y*b.x);} 
	Point& operator+=(const Point &p){
		x += p.x;
		y += p.y;
		return *this;
		}
	Point& operator-=(const Point &p){
		x -= p.x;
		y -= p.y;
		return *this;
		}
	Point& operator*=(double v){
		x *= v;
		y *= v;
		return *this;
		}
	Point operator- () const
		{return Point(-x, -y);}
};
inline Point operator* (const Point &a, double b)
	{return Point(a.x*b, a.y*b);}
inline Point operator* (double b, const Point &a)
	{return Point(a.x*b, a.y*b);}
inline Point operator/ (const Point &a, double b)
	{return Point(a.x/b, a.y/b);}
inline Point operator+ (const Point &a, const Point &b)
	{return Point(a.x + b.x, a.y + b.y);}
inline Point operator- (const Point &a, const Point &b)
	{return Point(a.x - b.x, a.y - b.y);}

inline std::ostream& operator<<(std::ostream& outS, const Point& p){
	outS<< '('
		<< p.x
		<< ", "
		<< p.y
		<< ')';
	return outS;
	}
inline std::istream& operator>>(std::istream& inS, Point& p){
	inS >> p.x
		>> p.y;
	return inS;
	}

// 24
// 8 triangle square
class Figure{
protected:
	Figure(int v) : VERT(v)
		{verts = new Point[VERT];}
	Point *verts;
public:
	const int VERT;
	virtual Point center() const{
		Point toRet;
		for(int i=0; i< VERT; ++i)
			toRet += verts[i] / VERT;
		return toRet;
		}

	virtual void printVertices() const{
		for(unsigned char i=0; i<VERT; ++i){
			std::cout
				<< "  "
				<< (char)('A'+ i)
				<< verts[i]
				<< '\n';
			}
		}
	virtual double surface() const = 0;

	virtual ~Figure()
		{delete[] verts;}
};

class Triangle : public Figure{
public:
	Triangle() : Figure(3){}
	friend std::istream& operator>>(std::istream &inS, Triangle &T){
		/* для треугольника 
		 * B ---- c
		 *  \    /
		 *   \  /
		 *    A
		 * вводятся точки A и B
		 * точка C расчитывается как поворот
		 * вектора AB на 60 гр по часовой стрелке
		 */
		inS >> T.verts[0]
			>> T.verts[1];
		Point vec = T.verts[1] - T.verts[0];
		const double r3 = std::sqrt(3);
		Point trVec; // умножение на матрицу поворота 60 гр.
		trVec = (Point(1, -r3) * vec.x) + (Point(r3, 1) * vec.y);
		T.verts[2] = T.verts[0] + (trVec * 0.5);
		return inS;
		}
	virtual void printVertices() const override{
		std::cout << "Triangle:\n";
		Figure::printVertices();
		}
	virtual double surface() const override{
		double sur = Point::cross(
				verts[1] - verts[0],
				verts[2] - verts[0]
				);
		sur = 0.5 * std::abs(sur);
		return sur;
		}
};

class Square : public Figure{
public:
	Square() : Figure(4){}
	friend std::istream& operator>>(std::istream &inS, Square &S){
		/* для квадрата
		 * B ---- C
		 * |      |
		 * |      |
		 * A ---- D
		 * вводятся точки A и C
		 * остальные точки расчитываются относительно центра O
		 * через поворот вектора OC на 90 градусов
		 */
		inS >> S.verts[0]
			>> S.verts[2];
		Point center = (S.verts[2] + S.verts[0]) * 0.5;
		Point vec = S.verts[2] - center;
		vec.x *= -1; // (x, y)  -->  (y, -x) поворот на 90
		std::swap(vec.x, vec.y);
		S.verts[3] = center + vec;
		S.verts[1] = center - vec;
		return inS;
		}
	virtual Point center() const override{
		return 0.5 *(verts[2] + verts[0]);
		}
	virtual void printVertices() const override{
		std::cout << "Square:\n";
		Figure::printVertices();
		}
	virtual double surface() const override{
		Point side = verts[1] - verts[0];
		return Point::dot(side, side);
		}
};

class Octagon : public Figure{
private:
	Point m_center;
public:
	Octagon() : Figure(8){}
	friend std::istream& operator>>(std::istream &inS, Octagon &Oc){
		/* Вводятся две точки:
		 * центр и одна вершина
		 * Остальные точки достраиваются
		 * до получения правильного восмиугольника
		 */
		inS >> Oc.m_center
			>> Oc.verts[0];
		Point vec = Oc.verts[0] - Oc.m_center;
		Oc.verts[4] = Oc.m_center - vec;
		vec.x *= -1;
		std::swap(vec.x, vec.y);
		Oc.verts[2] = Oc.m_center + vec;
		Oc.verts[6] = Oc.m_center - vec;
		const double r2 = 1 / std::sqrt(2);
		Point dia(vec.x + vec.y, vec.y - vec.x);
		dia *= r2;
		Oc.verts[3] = Oc.m_center + dia;
		Oc.verts[7] = Oc.m_center - dia;
		dia.x *= -1;
		std::swap(dia.x, dia.y);
		Oc.verts[5] = Oc.m_center + dia;
		Oc.verts[1] = Oc.m_center - dia;
		return inS;
		}
	virtual Point center() const override{
		return m_center;
		}
	virtual void printVertices() const override{
		std::cout << "Octagon:\n";
		Figure::printVertices();
		}
	virtual double surface() const override{
		double sur = Point::cross(
				verts[0] - m_center,
				verts[1] - m_center);
		sur = 4 * std::abs(sur);
		return sur;
		}
};
#endif
