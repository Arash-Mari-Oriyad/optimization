 #include "Functions.h"

int main(int argc, char **argv)
{
  string input_file_name;
  int N = 0, K = 0;
  vector<pair<double, double>> Points;
  vector<vector<double>> Distances;

  input_file_name = argv[1];
  readProblem(N, K, Points, input_file_name);
  calculateDistances(Distances, N, Points);
  solve(N, K, Distances, Points);

  return 0;
}
