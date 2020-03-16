#pragma once

#ifndef REGRESSION_H
#define REGRESSION_H

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

void polyRegression(const int num, 
					const std::vector<double>& y, 
						  double &a, 
						  double &b, 
						  double &c,
						  int start = 0)
{ // ax^2 + bx+ c = y
	//cout << "frame count : " << num << endl;
	if (num < 3) {
		a = 0,
		b = 0,
		c = 20;
		printf("frame : \n", num);
		return;
	}
	std::vector<int> x(num);
	iota(x.begin(), x.end(), start);
	//cout << "1";
	int n = x.size();
	std::vector<int> r(n);
	std::iota(r.begin(), r.end(), start);
	//cout << "2";
	double xm = std::accumulate(x.begin(), x.end(), 0.0) / x.size();
	double ym = std::accumulate(y.begin(), y.end(), 0.0) / y.size();
	if (isnan(xm) != 0) {
		printf("xm is nan \n");
	}
	if (isnan(ym) != 0) {
		printf("ym is nan \n");
	}
	//printf("xm %f, ym %f\n", xm, ym);
	//cout << "3";
	double x2m = std::transform_reduce(r.begin(), r.end(), 0.0, std::plus<double>{}, [](double a) {return a * a; }) / r.size();
	double x3m = std::transform_reduce(r.begin(), r.end(), 0.0, std::plus<double>{}, [](double a) {return a * a * a; }) / r.size();
	double x4m = std::transform_reduce(r.begin(), r.end(), 0.0, std::plus<double>{}, [](double a) {return a * a * a * a; }) / r.size();
	//printf("x2m %f, x3m %f, x4m %f\n", x2m, x3m, x4m);
	
	if (isnan(x2m) != 0) {
		printf("x2m is nan \n");
	}
	if (isnan(x3m) != 0) {
		printf("x3m is nan \n");
	}
	if (isnan(x4m) != 0) {
		printf("x4m is nan \n");
	}
	//cout << "4";
	double xym = std::transform_reduce(x.begin(), x.end(), y.begin(), 0.0, std::plus<double>{}, std::multiplies<double>{});
	xym /= fmin(x.size(), y.size());
	if (isnan(xym) != 0) {
		printf("xym is nan \n");
	}
	//cout << "5";
	double x2ym = std::transform_reduce(x.begin(), x.end(), y.begin(), 0.0, std::plus<double>{}, [](double a, double b) { return a * a * b; });
	x2ym /= fmin(x.size(), y.size());
	if (isnan(x2ym) != 0) {
		printf("x2ym : %f \n", x2ym);
	}
	//printf("x4m is nan \n");
	//cout << "5";
	double sxx = x2m - xm * xm;
	double sxy = xym - xm * ym;
	double sxx2 = x3m - xm * x2m;
	double sx2x2 = x4m - x2m * x2m;
	double sx2y = x2ym - x2m * ym;

	//printf("sxx = %f, sxy = %f, sxx2 = %f, sx2x2 = %f, sx2y = %f", sxx, sxy, sxx2, sx2x2, sx2y);

	if ((sxx * sx2x2 - sxx2 * sxx2) == 0) {
		printf("(sxx * sx2x2 - sxx2 * sxx2) is 0 \n");
	}

	a = (sx2y * sxx - sxy * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
	b = (sxy * sx2x2 - sx2y * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
	c = ym - b * xm - a * x2m;

	/*auto abc = [a, b, c](int xx) {
		return c + b * xx + a * xx*xx;
	};*/

	//std::cout << "y = " << a << "x^2" << " + " << b << "x + " << c << std::endl;
	//std::cout << " Input  Approximation" << std::endl;
	//std::cout << " x   y     y1" << std::endl;
	
	/*
	auto xit = x.cbegin();
	auto xend = x.cend();
	auto yit = y.cbegin();
	auto yend = y.cend();
	while (xit != xend && yit != yend) {
		printf("%2d %3d  %5.1f\n", *xit, *yit, abc(*xit));
		xit = std::next(xit);
		yit = std::next(yit);
	}*/
}


//////////////////////////////////////////////////////////////////////////




double getSlope(vector<double> &x, vector<double> &y)
{
	double sx = accumulate(x.begin(), x.end(), 0);
	double sy = accumulate(y.begin(), y.end(), 0);
	double sxx = inner_product(x.begin(), x.end(), x.begin(), 0);
	double sxy = inner_product(x.begin(), x.end(), y.begin(), 0);
	int n = static_cast<int>(x.size());
	// (n*sxy — sx*sy)/(n*sxx — sx*sx)
	double nor = n * sxy - sx*sy;
	double denor = n * sxx - sx*sx;
	if (denor != 0)
	{
		return nor / denor;
	}
	return numeric_limits<double>::max();
}
double getIntercept(vector<double> &x, vector<double> &y, double slope)
{
	double sx = accumulate(x.begin(), x.end(), 0);
	double sy = accumulate(y.begin(), y.end(), 0);
	int n = static_cast<int>(x.size());
	return (sy - slope * sx) / n;
}
// slope:a
// intercept:b
// derivative of slope: da
// derivative of intercept: db
double getCost(vector<double> &x, vector<double> &y, double a, double b, double &da, double &db)
{
	int n = static_cast<int>(x.size());
	double sx = accumulate(x.begin(), x.end(), 0);
	double sy = accumulate(y.begin(), y.end(), 0);
	double sxx = inner_product(x.begin(), x.end(), x.begin(), 0);
	double sxy = inner_product(x.begin(), x.end(), y.begin(), 0);
	double syy = inner_product(y.begin(), y.end(), y.begin(), 0);
	double cost = syy - 2 * a*sxy - 2 * b*sy + a * a*sxx + 2 * a*b*sx + n * b*b;
	cost /= n;
	da = 2 * (-sxy + a * sxx + b * sx) / n;
	db = 2 * (-sy + a * sx + n * b) / n;
	return cost;
}
void linearRegression(const int num, vector<double> &y, double& slope, double& intercept, int start = 0)
{
	double lrate = 0.0002;//0.0002;
	double threshold = 0.0001;//0.0001;
	int iter = 0;
	while (true)
	{
		double da = 0;
		double db = 0;
		std::vector<double> x(num);
		iota(x.begin(), x.end(), start);

		double cost = getCost(x, y, slope, intercept, da, db);
		/*if (iter % 1000 == 0)
		{
			cout << "Iter: " << iter << " cost = " << cost << " da = " << da << " db = " << db << endl;
		}*/
		iter++;
		if (abs(da) < threshold && abs(db) < threshold)
		{
			//cout << "y = " << slope << " * x + " << intercept << endl;
			break;
		}
		slope -= lrate * da;
		intercept -= lrate * db;
	}
}
void linearRegression2(const int num, vector<double> &y, double& slope, double& intercept, int start = 0) {
	std::vector<double> x(num);
	iota(x.begin(), x.end(), start);

	slope = getSlope(x, y);
	intercept = getIntercept(x, y, slope);
}

#endif