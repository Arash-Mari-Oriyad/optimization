#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include "LPSolveLibraries/lp_lib.h"

using namespace std;

#define ll long long


void readProblem(int &, int &, vector<pair<double, double>> &, string);
double calculateDistance(pair<double, double>, pair<double, double>);
void calculateDistances(vector<vector<double>> &, int, vector<pair<double, double>>);
void resetRow(double*, int);
void solve(int, int, vector<vector<double>>, vector<pair<double, double>>);
