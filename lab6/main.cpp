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

  int W = N;
  int H = K / N;

  const int dims[] = {W, H};
  const bool periods[] = {false, false};

  MPI::Cartcomm cartComm = worldComm.Create_cart(2, dims, periods, false); 

  const int rank = cartComm.Get_rank();

  int coords[2];

  cartComm.Get_coords(rank, 2, coords);

  std::cout << "[" << cartComm.Get_rank() << "] pos = {" << coords[0] << ", "
            << coords[1] << "}" << std::endl;

  MPI::Finalize();
}
