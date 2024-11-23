#include <mpi.h>

#include <array>
#include <ctime>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  MPI::Intracomm &worldComm = MPI::COMM_WORLD;

  const int N = std::atoi(argv[1]);
  const int K = worldComm.Get_size();

  if (K % N != 0) {
    throw MPI::Exception(MPI::ERR_ARG);
  }

  double time = 0, start, end;

  int W = N;
  int H = K / N;

  const int dims[] = {W, H};
  const bool periods[] = {false, false};
  int coords[2], rank;

  start = MPI::Wtime();

  MPI::Cartcomm cartComm = worldComm.Create_cart(2, dims, periods, false);
  rank = cartComm.Get_rank();
  cartComm.Get_coords(rank, 2, coords);

  end = MPI::Wtime();
  time += end - start;

  std::cout << "[" << cartComm.Get_rank() << "] pos = {" << coords[0] << ", "
            << coords[1] << "}; time = " << time << std::endl;

  MPI::Finalize();
}
