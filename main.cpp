/* Савченко Илья Владимирович
 * М8О-208Б-19
 *
 * вариант 24
 *    - Треугольник
 *    - Квадрат
 *    - Восьмиугольник
 *
 * Разработать классы согласно варианту задания,
 * классы должны наследоваться от базового класса Figure.
 * Фигуры являются фигурами вращения.
 * Все классы должны поддерживать набор общих методов:
 * 1.       Вычисление геометрического центра фигуры;
 * 2.       Вывод в стандартный поток вывода std::cout
 *          координат вершин фигуры;
 * 3.       Вычисление площади фигуры.
 *
 * Создать программу, которая позволяет:
 * -----   Вводить из стандартного ввода std::cin фигуры,
 *         согласно варианту задания.
 * -----   Сохранять созданные фигуры в динамический массив
 *         std::vector<Figure*>.
 * -----   Вызывать для всего массива общие функции (1-3 см. выше).
 *         Т.е. распечатывать для каждой фигуры в массиве
 *         геометрический центр, координаты вершин и площадь.
 * -----   Необходимо уметь вычислять общую площадь фигур в массиве.
 * -----   Удалять из массива фигуру по индексу.
 */



#include<iostream>
#include<cstdio>
#include<vector>
#include"figure.h"

using namespace std;

double SurfaceComb(vector<Figure*> &vec){
	double toRet(0);
	for(Figure *f : vec)
		if(f)
			toRet += f->surface();
	return toRet;
	}

void quickList(vector<Figure*> &vec){
	if(vec.empty()){
		cout << "Empty\n";
		return;
		}
	for(unsigned int i=0; i< vec.size(); ++i){
		printf("%2u: ", i);
		if(!vec[i]){
			cout << "null\n";
			continue;
			}
		switch(vec[i]->VERT){
			case 3:
				cout << "Triangle\n";
				continue;
			case 4:
				cout << "Square\n";
				continue;
			case 8:
				cout << "Octagon\n";
				continue;
			default:
				cout << "?\n";
				continue;
			}
		}
	cout << '\n';
	}

int main(){
	vector<Figure*> myVec;

	while(true){
		char t;
		Figure *f;
		cout << "Next figure (t,s,o + 2 points, x to exit): ";
		cin >> t;
		switch(t){
			case 't':
				f = new Triangle;
				cin >> *static_cast<Triangle*>(f);
				break;
			case 's':
				f = new Square;
				cin >> *static_cast<Square*>(f);
				break;
			case 'o':
				f = new Octagon;
				cin >> *static_cast<Octagon*>(f);
				break;
			case 'x':
				goto FigureInputEnd;
			default:
				cout << "No such option\n";
				f = nullptr;
				break;
			}
		if(f)
			myVec.push_back(f);
		}
FigureInputEnd:

	cout << '\n';
	for(Figure *f : myVec){
		if(!f){
			cout << "null\n\n";
			continue;
			}
		f->printVertices();
		cout <<  "Center = "
		     << f->center()
		     <<"\n S     = "
		     << f->surface()
		     << "\n\n";
		}

	cout << "Surface combined\n  = "
		 << SurfaceComb(myVec) << "\n\n";

	quickList(myVec);
	cout << "^D to exit\nenter <0 to list\n";
	cout << "Delete figure? ";
	int ind;
	while(cin >> ind){
		if(ind < 0){
			quickList(myVec);
			continue;
			}
		if(ind >= myVec.size()){
			cout << "Size is "
				 << myVec.size()
				 << '\n';
			continue;
			}
		delete myVec[ind];
		myVec.erase(myVec.begin() + ind);
		}
	}
