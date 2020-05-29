#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <Windows.h>

using namespace std;

#define INF 10000

struct WSpace {
	double max_X;
	double min_X;
	double max_Y;
	double min_Y;
};
struct Point
{
	double x;
	double y;
};

const int W = 80;
const int H = W / 2;

double max_el(Point p, Point r);
double min_el(Point p, Point r);
bool ontheSide(Point p, Point q, Point r);
int orientation(Point p, Point q, Point r);
bool Intersection(Point p, Point pn, Point pr, Point q2);
bool isInside(Point* pt, int n, Point ranD);
double dRand(double min, double max);
Point getPositionByAngle(float angle, Point* pole);
void inputrandPolygon(Point* pt, Point* pole, int n);
void workSize(Point* pt, WSpace* worksp, int n);
void inputPolygon(Point* pt, int n);
void inputPole(Point* pole);
void setColor(int a, int b);
void OutPutBuf(int buf[H][W]);
void buffer(int buf[H][W], Point rand, WSpace* worksp);
void NumofPoints(Point* pt, int buf[H][W], WSpace* worksp, int n, Point* pole, double& N, double M);
double Area(double N, Point* pole, double M);
void Polygon(Point* pole);
void inCircle(Point* pole, int buf[H][W], WSpace* worksp, double& N, Point circ, double R, double M);
void Circle(Point* pole);
void inLemniscate(Point* pole, int buf[H][W], WSpace* worksp, int& N, int M, double a);
void Lemniscate(Point* pole);

int main()
{
	srand(time(NULL));
	int command;
	Point pole[2];
	printf("Welcome to square finder\n\nPlease,choose the type of figure:\n1) - lemniscate\n2) - circle\n3) - Polygon\n\n");
	printf("Your choice is : ");
	scanf("%i", &command);
	printf("\n");
	switch (command) {
        case 1: {

            Lemniscate(pole);

            break;
        }
        case 2: {

            Circle(pole);

            break;
        }
        case 3: {

            Polygon(pole);

            break;
        }
        default: {

            printf("Wrong command!!!");

            break;
        }
	}
	return 0;
}

double max_el(Point p, Point r) {
	double max = r.x;
	if (p.x > r.x)max = p.x;
	return max;
}
double min_el(Point p, Point r) {
	double min = r.x;
	if (p.x < r.x)min = p.x;
	return min;
}
bool ontheSide(Point p, Point q, Point r)
{
	if (q.x <= max_el(p, r) && q.x >= min_el(p, r) && q.y <= max_el(p, r) && q.y >= min_el(p, r))
		return true;
	return false;
}
int orientation(Point p, Point q, Point r)
{
	double val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}
bool Intersection(Point p, Point pn, Point pr, Point q2)
{
	int o1 = orientation(p, pn, pr);
	int o2 = orientation(p, pn, q2);
	int o3 = orientation(pr, q2, p);
	int o4 = orientation(pr, q2, pn);
	if (o1 != o2 && o3 != o4)
		return true;
	if (o1 == 0 && ontheSide(p, pr, pn)) return true;
	if (o2 == 0 && ontheSide(p, q2, pn)) return true;
	if (o3 == 0 && ontheSide(pr, p, q2)) return true;
	if (o4 == 0 && ontheSide(pr, pn, q2)) return true;

	return false;
}
bool isInside(Point* pt, int n, Point ranD)
{
	if (n < 3)  return false;


	Point extreme = { INF, ranD.y };


	int count = 0, i = 0;
	do
	{
		int next = (i + 1) % n;


		if (Intersection(pt[i], pt[next], ranD, extreme))
		{
			if (orientation(pt[i], ranD, pt[next]) == 0)
				return ontheSide(pt[i], ranD, pt[next]);

			count++;
		}
		i = next;
	} while (i != 0);
	return count & 1;
}
double dRand(double min, double max) {
	return min + ((double)rand() / RAND_MAX) * (max - min);
}
Point getPositionByAngle(float angle, Point* pole) {

	float pi = 3.1415;
	angle = angle * pi / 180.0f;
	Point position;
	double  er_x = 0, er_y = 0;
	er_x = (rand() * 1.0 / RAND_MAX) * (pole[1].x - 2 - pole[0].x) + 1 + pole[0].x;
	er_y = (rand() * 1.0 / RAND_MAX) * (pole[1].y - 2 - pole[0].y) + 1 + pole[0].x;
	position.x = cos(angle) + er_x;
	position.y = sin(angle) + er_y;

	return position;
}
void inputrandPolygon(Point* pt, Point* pole, int n) {
	float angleStep = 360.0f / n;
	for (int i = 0; i < n; i++) {
		pt[i] = getPositionByAngle((i * angleStep - dRand(-angleStep / 2.1f, angleStep / 2.1f)), pole);
		printf("Enter point %i", i + 1);
		printf(" : %lf", pt[i].x);
		printf(" %lf\n", pt[i].y);

	}
}
void workSize(Point* pt, WSpace* worksp, int n) {
	worksp->max_X = worksp->min_X = pt[0].x;
	worksp->max_Y = worksp->min_Y = pt[0].y;
	for (int i = 1; i < n; i++) {
		if (pt[i].x > worksp->max_X)worksp->max_X = pt[i].x;
		if (pt[i].x < worksp->min_X)worksp->min_X = pt[i].x;
		if (pt[i].y > worksp->max_Y)worksp->max_Y = pt[i].y;
		if (pt[i].y < worksp->min_Y)worksp->min_Y = pt[i].y;
	}
	cout << worksp->min_X << " " << worksp->max_X << " " << worksp->min_Y << " " << worksp->max_Y << " ";
}
void inputPolygon(Point* pt, int n) {
	printf("Enter the vertices of the polygon : ");
	for (int i = 0; i < n; i++) {
		scanf("%lf %lf", &pt[i].x, &pt[i].y);
	}
}
void inputPole(Point* pole) {
	printf("Now input coordinates of the field(x1,x2,y1,y2) : ");
	scanf("%lf %lf %lf %lf", &pole[0].x, &pole[0].y, &pole[1].x, &pole[1].y);
	if (pole[0].x > pole[1].x)swap(pole[0].x, pole[1].x);
	if (pole[0].y > pole[1].y)swap(pole[0].y, pole[1].y);
}
HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
void setColor(int a, int b)
{
	SetConsoleTextAttribute(hnd, (WORD)(a | b << 4));
}
void OutPutBuf(int buf[H][W])
{
	int max_shot = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (buf[i][j] > max_shot)max_shot = buf[i][j];
		}
	}
	for (int i = H - 1; i >= 0; i--)
	{
		for (int j = 0; j < W; j++)
		{
			int in = buf[i][j];
			if (in == 0)setColor(0, 0);
			else if (in >= max_shot * 0.9)setColor(4, 0);
			else if (in >= max_shot * 0.7)setColor(12, 0);
			else if (in >= max_shot * 0.4)setColor(6, 0);
			else setColor(14, 0);
			char ch = 178;
			printf("%c", ch);
			setColor(14, 0);
		}
		printf("\n");
	}
}
void buffer(int buf[H][W], Point rand, WSpace* worksp) {
	int buf_X = (rand.x - worksp->min_X) / (worksp->max_X - worksp->min_X) * W;
	if (buf_X > W - 1)buf_X = W - 1;
	if (buf_X < 0)buf_X = 0;
	int buf_Y = (rand.y - worksp->min_Y) / (worksp->max_Y - worksp->min_Y) * H;
	if (buf_Y > H - 1)buf_Y = H - 1;
	if (buf_Y < 0)buf_Y = 0;
	buf[buf_Y][buf_X]++;
}
void NumofPoints(Point* pt, int buf[H][W], WSpace* worksp, int n, Point* pole, double& N, double M) {
	for (double i = 0; i < M; i++) {
		Point ranD;
		ranD.x = rand() * 1.0 / RAND_MAX * (pole[1].x - pole[0].x) + pole[0].x;
		ranD.y = rand() * 1.0 / RAND_MAX * (pole[1].y - pole[0].y) + pole[0].y;
		if (isInside(pt, n, ranD)) {
			buffer(buf, ranD, worksp);
			N++;
		}
	}
}
double Area(double N, Point* pole, double M) {
	double s = (N / M) * (pole[1].x - pole[0].x) * (pole[1].y - pole[0].y);
	return s;
}
void Polygon(Point* pole) {
	WSpace* worksp = new WSpace;
	int buf[H][W] = { 0 };
	int n;
	double M = 0;
	double N = 0;
	inputPole(pole);
	printf("Enter the number of vertices : ");
	scanf("%i", &n);
	Point* pt = new Point[n];
	int ch;
	printf("Choose the way to enter the vertices of the polygon (1 - random; 2 - independently) : ");
	scanf("%i", &ch);
	switch (ch) {
	case 1: inputrandPolygon(pt, pole, n);
		break;
	case 2: inputPolygon(pt, n);
		break;
	default: printf("Wrong command!!!");
	}
	workSize(pt, worksp, n);
	printf("\nEnter accuracy - ");
	scanf("%lf", &M);
	NumofPoints(pt, buf, worksp, n, pole, N, M);
	printf("Area - ");
	printf("%lf\n", Area(N, pole, M));
	OutPutBuf(buf);
}
void inCircle(Point* pole, int buf[H][W], WSpace* worksp, double& N, Point circ, double R, double M) {
	for (double i = 0; i < M; i++) {
		Point ranD;
		ranD.x = rand() * 1.0 / RAND_MAX * (pole[1].x - pole[0].x) + pole[0].x;
		ranD.y = rand() * 1.0 / RAND_MAX * (pole[1].y - pole[0].y) + pole[0].y;
		if (pow(ranD.x - circ.x, 2) + pow(ranD.y - circ.y, 2) <= R * R) {
			N++;
			buffer(buf, ranD, worksp);
		}
	}
}
void Circle(Point* pole) {
	double R, M;
	double N = 0;
	WSpace* worksp = new WSpace;
	int buf[H][W] = { 0 };
	Point circ;
	inputPole(pole);
	printf("Choose the center of the circle ");
	printf("\nEnter coordinate x : ");
	scanf("%lf", &circ.x);
	printf("Enter coordinate y : ");
	scanf("%lf", &circ.y);
	printf("Enter the radius : ");
	scanf("%lf", &R);
	worksp->max_X = circ.x + R;
	worksp->max_Y = circ.y + R;
	worksp->min_X = circ.x - R;
	worksp->min_Y = circ.y - R;
	printf("Enter accuracy - ");
	scanf("%lf", &M);
	inCircle(pole, buf, worksp, N, circ, R, M);
	printf("\nThe Area is : ");
	printf("%lf\n", Area(N, pole, M));
	OutPutBuf(buf);
}
void inLemniscate(Point* pole, int buf[H][W], WSpace* worksp, int& N, int M, double a) {
	for (double i = 0; i < M; i++) {
		Point ranD;
		ranD.x = rand() * 1.0 / RAND_MAX * (pole[1].x - pole[0].x) + pole[0].x;
		ranD.y = rand() * 1.0 / RAND_MAX * (pole[1].y - pole[0].y) + pole[0].y;
		if (sqrt((ranD.x * ranD.x + ranD.y * ranD.y) * (ranD.x * ranD.x + ranD.y * ranD.y) / (ranD.x * ranD.x - ranD.y * ranD.y) * 2) <= a) {
			N++;
			buffer(buf, ranD, worksp);
		}
	}
}
void Lemniscate(Point* pole) {
	double a, M;
	int N = 0;
	WSpace* worksp = new WSpace;
	int buf[H][W] = { 0 };
	printf("The formula of lemniscate is (x^2 + y^2)^2 = 2*a^2 * (x^2 - y^2) ");
	printf("\nEnter a : ");
	scanf("%lf", &a);
	pole[1].x = 1.41 * a;
	pole[1].y = a / 2;
	pole[0].x = (-1) * 1.41 * a;
	pole[0].y = (-1) * a / 2;
	worksp->max_X = 1.41 * a;
	worksp->max_Y = a / 2;
	worksp->min_X = (-1) * 1.41 * a;
	worksp->min_Y = (-1) * a / 2;
	printf("Enter accuracy - ");
	scanf("%lf", &M);
	inLemniscate(pole, buf, worksp, N, M, a);
	printf("The area is : ");
	printf("%lf\n", Area(N, pole, M));
	OutPutBuf(buf);
}
