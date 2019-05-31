// Алгоритм Грехема далее Алгоритм
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;


struct Point{
     double x;
     double y;
};

struct key_str {
	double ugol;
	double rast;
	key_str (double nugol = 0.0, double nrast = 0){
				ugol = nugol;
				rast = nrast;
	};
	void operator= (key_str k){
		ugol = k.ugol;
		rast = k.rast;
	};
	bool operator< (key_str k){
		return ugol < k.ugol || (k.ugol == ugol  && rast > k.rast);
	};
};



struct NewPoint{

		double x;
		double y;
		key_str key;
		NewPoint (double new_x = 0.0, double new_y= 0.0, key_str nkey = 0.0){
				x = new_x;
				y = new_y;
				key = nkey;

		};
		bool operator< (NewPoint p){
			return (x < p.x || (x == p.x && y < p.y));
		};
		bool operator> (NewPoint p){
			return (x > p.x || (x == p.x && y > p.y));
		};
		void operator= (NewPoint p){
			x = p.x;
			y = p.y;
			key = p.key;

		};
		bool operator!= (NewPoint p) {
			return ((x != p.x) || (y != p.y) || key < p.key || p.key < key);		
		};
};

double polar_angle (NewPoint p,NewPoint p0){
	double pi=3.14;
    double alpha = atan2(p.y-p0.y, p.x-p0.x);
    if (alpha < 0) alpha += 2 * pi;
    return alpha;
};

class Stack {
	NewPoint* mem;
	int size;
	int n;
public:
	Stack(int nsize = 1000);
	void add (NewPoint x);
	NewPoint pop ();
	NewPoint front ();
	NewPoint next_front ();
	void printStack();
	int get_size();
};

Stack::Stack(int nsize) {
	size = nsize;
	mem = new NewPoint[nsize];
	n = 0;
}
void Stack::add (NewPoint x) {
	if (n == size) throw 1;
	mem[n++] = x;
}
NewPoint Stack::pop () {
	if (n == 0) throw 2;
	return mem[--n];
}
NewPoint Stack::front () {
	if (n == 0) throw 2;
	return mem[n - 1];
}
NewPoint Stack::next_front () {
	if (n == 1) throw 2;
	return mem[n - 2];
};

int Stack::get_size() {
	return n;
};

void Stack::printStack()
{

    for (int ix = n - 1; ix >= 0; ix--)
         cout<< mem[ix].x <<' '<<mem[ix].y <<endl;
};


class Binary_heap {
private:
	NewPoint* mem;
	int size;
	int n;
public:
	Binary_heap (int nsize = 1000);
	void add (NewPoint x);
	NewPoint extract ();
	int get_size();
private:
	void up (int c);
	void down (int c);
};
 
Binary_heap::Binary_heap (int nsize) {
	size = nsize;
	mem = new NewPoint[size];
	n = 0;
}
void Binary_heap::add (NewPoint x) {
	if (n == size - 1) throw 3;
	mem[++n] = x;
	up (n);
}

NewPoint Binary_heap::extract () {
	if (n == 0) throw 4;
	NewPoint ans = mem[1];
	mem[1] = mem[n--];
	down(1);
	return ans;
}

int Binary_heap::get_size() {
	return n;
}
void Binary_heap::up (int c) {
	int m = c / 2;
	if (m == 0) return;
	
	while (mem[c].key < mem[m].key){
		NewPoint tmp = mem[m];
		mem[m] = mem[c];
		mem[c] = tmp;
	}
}

void Binary_heap::down (int c) {
	int m = c * 2;
	if (m > n) return;
	if (m + 1 <= n && &mem[m + 1].key < &mem[m].key) m++;

	while(&mem[m].key < &mem[c].key) {

		NewPoint tmp = mem[m];
		mem[m] = mem[c];
		mem[c] = tmp;

	}

};

double vec (NewPoint p1, NewPoint p2, NewPoint p3) {
	double ax = p2.x - p1.x,
	bx = p1.x - p3.x,
	ay = p2.y - p1.y,
	by = p1.y - p3.y;
	return ax * by - bx * ay;		
};

 int _tmain(int argc, _TCHAR* argv[])
{
	try {
	Point p[1000];
	Point tmp;

	ifstream infile ("input.txt");
	const int MAX = 1000;
	int counter=0;
	char bufer[MAX];
	while (!infile.eof()){
		infile.getline(bufer,MAX);
		infile>>p[counter].x>>p[counter].y;
		counter++;
	}

		

//сортировка по у
	for (int i = 0; i < counter; i++) {
        for (int j = 0; j < counter - i - 1; j++) {
            if (p[j].y > p[j + 1].y) {
                // меняем элементы местами
                tmp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = tmp;
            }
        }
    }
//остортированный массив по у, самая нижние точки первые. теперь надо выявять начальную точку, левую по х
	int i = 0;
	while (p[i].y==p[i+1].y)
	{
		for (int i = 0; i < counter; i++) {
			for (int j = 0; j < counter - i - 1; j++) {
				if (p[j].x > p[j + 1].x) {
                // меняем элементы местами
					tmp = p[j];
					p[j] = p[j + 1];
					p[j + 1] = tmp;
				}
			}
		}
		i++;
	}
//теперь p[0] имеет минимальную координату по у и самую левую по х
	//определить полярный угол
	NewPoint new_p[100];
	new_p[0].x=p[0].x;
	new_p[0].y=p[0].y;
	new_p[0].key=key_str(0.0,0.0);

//перезапишем 
	for (int i=1;i<counter;i++){
		new_p[i].x=p[i].x;
		new_p[i].y=p[i].y;
		new_p[i].key = key_str(polar_angle(new_p[i],new_p[0]),sqrt((new_p[i].x-new_p[0].x)*(new_p[i].x-new_p[0].x)+(new_p[i].y-new_p[0].y)*(new_p[i].y-new_p[0].y)));
	};
	
	NewPoint res_point[100];
	res_point[0]=new_p[0];
	//создать результирующий стек
	Stack s(100);
	//положить в стек new_p[0]
	s.add(new_p[0]);
	//Создать бинарную кучу
	Binary_heap kucha(100);

	NewPoint start = new_p[0];

	for (int i = 0; i < counter; i++) {
		if (new_p[i] != start) {
			kucha.add(new_p[i]);
		}
	}
	s.add(kucha.extract());

	NewPoint tmp1;
	for (int i = 0; i < counter - 2; i++) {
		tmp1 = kucha.extract();
		while (vec (s.front(), s.next_front(), tmp1) < 0) {
			s.pop();
		}
		s.add (tmp1);	
	}
	
	s.printStack();
 

	} catch (int x) {
		cout << "Error number: " << x << endl;
	}	
	return 0;
 
}
