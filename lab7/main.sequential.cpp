#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std::chrono;

int main(int argc, char **argv) {
  const std::string fileName = argv[1];
  std::ifstream file(fileName);
  int n, item;

  file >> n;

  std::vector<std::vector<int>> A(n, std::vector<int>(n)),
      B(n, std::vector<int>(n));

  std::vector<std::vector<double>> C(n, std::vector<double>(n));

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      file >> A[i][j];
    }
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      file >> B[i][j];
    }
  }

  file.close();

  auto begin = steady_clock::now();

  for (size_t y = 0; y < n; y++) {
    for (size_t x = 0; x < n; x++) {
      C[y][x] = 0;
      for (size_t j = 0; j < n; j++) {
        C[y][x] += A[y][j] * B[j][x];
      }
    }
  }

  auto end = steady_clock::now();
  double time = duration_cast<nanoseconds>(end - begin).count() / 1000000000.0;
  std::cout << "time: " << time << " s" << std::endl;

  std::ofstream output("result-seq." + fileName);
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      output << C[i][j] << " ";
    }
    output << std::endl;
  }
  output.close();
}
