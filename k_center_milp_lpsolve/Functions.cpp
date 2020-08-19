#include "Functions.h"

void readProblem(int & N, int & K, vector<pair<double, double>> & Points, string input_file_name)
{
  ifstream input_file(input_file_name);
  string line;
  istringstream iss;
  getline(input_file, line);
  iss.str(line);
  iss >> K;
  line = "";
  iss.clear();
  getline(input_file, line);
  iss.str(line);
  iss >> N;
  while(input_file.peek() != EOF)
  {
    double x, y;
    line = "";
    iss.clear();
    getline(input_file, line);
    iss.str(line);
    iss >> x;
    iss >> y;
    Points.push_back(make_pair(x, y));
  }
  return;
}


double calculateDistance(pair<double, double> p1, pair<double, double> p2)
{
  return sqrt(pow((p1.first - p2.first), 2) + pow((p1.second - p2.second), 2));
}


void calculateDistances(vector<vector<double>> & Distances, int N, vector<pair<double, double>> Points)
{
  Distances.resize(N, vector<double> (N));
  for(int i = 0; i < N; i++)
    for(int j = i; j < N; j++)
    {
      Distances[i][j] = calculateDistance(Points[i], Points[j]);
      Distances[j][i] = Distances[i][j];
    }
  return;
}


void resetRow(double* row, int nPrimalVariables)
{
  for(int i = 0; i < nPrimalVariables + 1; i++)
    row[i] = 0;
  return;
}


void solve(int N, int K, vector<vector<double>> Distances, vector<pair<double, double>> Points)
{
  lprec *lp;
  ll nPrimalVariables = N + N*N + 1;
  ll nPrimalConstraints = N + 1 + N * N + N;
  REAL row[nPrimalVariables + 1];
  lp = make_lp(0, nPrimalVariables);
  for(int i = 0; i < nPrimalVariables; i++)
  {
    if(i < N)
      set_binary(lp, i + 1, TRUE);
    else if (i < N + N * N)
      set_bounds(lp, i + 1, 0.0, 1.0);
    else
      set_unbounded(lp, i + 1);
  }
  set_add_rowmode(lp, TRUE);
  for(int i = 0; i < nPrimalConstraints; i++)
  {
    resetRow(row, nPrimalVariables);
    if(i < N)
    {
      for(int j = 0; j < nPrimalVariables; j++)
      {
        if(j < N)
          row[j + 1] = 0;
        else if(j < N + N * N)
        {
          if(j / N - 1 == i)
            row[j + 1] = 1;
        }
        else
          row[j + 1] = 0;
      }
      add_constraint(lp, row, EQ, 1.0);
    }
    else if(i < N + 1)
    {
      for(int j = 0; j < nPrimalVariables; j++)
      {
        if(j < N)
          row[j + 1] = 1;
        else
          row[j + 1] = 0;
      }
      add_constraint(lp, row, EQ, (double)K);
    }
    else if(i < N + 1 + N * N)
    {
      int temp = i - (N + 1);
      for(int j = 0; j < nPrimalVariables; j++)
      {
        if(j == temp % N)
          row[j + 1] = 1;
        else if(j == N + temp)
          row[j + 1] = -1;
        else
          row[j + 1] = 0;

      }
      add_constraint(lp, row, GE, 0.0);
    }
    else
    {
      int temp = i - (N + 1 + N * N);
      for(int j = 0; j < nPrimalVariables; j++)
      {
        if(j < N)
          row[j + 1] = 0;
        else if(j < N + N * N)
          if(j / N - 1 == temp)
            row[j + 1] = Distances[temp][j % N];
        else
          row[j + 1] = 0;
      }
      row[nPrimalVariables] = -1;
      add_constraint(lp, row, LE, 0.0);
    }
  }
  set_add_rowmode(lp, FALSE);
  for(int i = 0; i < nPrimalVariables; i++)
  {
    if(i < nPrimalVariables - 1)
      row[i + 1] = 0;
    else
      row[i + 1] = 1;
  }
  set_obj_fn(lp, row);
  set_minim(lp);
  //set_verbose(lp, IMPORTANT);
  int ret = solve(lp);
  get_variables(lp, row);
  cout << "The optimal solution is " << get_objective(lp) << endl;
  cout << "The centers are:" << endl;
  for(int i = 0; i < N; i++)
  {
    if(row[i] == 1)
      cout <<Points[i].first << ' ' << Points[i].second << endl;
  }
  delete_lp(lp);
  return;
}
