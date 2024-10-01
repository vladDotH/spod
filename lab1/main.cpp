#include <mpi.h>

#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  MPI::Comm &comm = MPI::COMM_WORLD;

  int procCount = comm.Get_size();
  int procRank = comm.Get_rank();

  if (procRank == 0) {
    std::cout << "Hello from process: " << procRank << std::endl;

    std::vector<int> messages(procCount - 1);

    double start = MPI::Wtime();

    for (size_t i = 0; i < procCount - 1; i++) {
      comm.Recv(&messages[i], 1, MPI::INT, MPI::ANY_SOURCE, MPI::ANY_TAG);
    }

    double end = MPI::Wtime();

    for (auto msg : messages) {
      std::cout << "\t Recieved hello from process: " << msg << std::endl;
    }

    std::cout << "Elapsed time: " << (end - start) << "s." << std::endl;
  } else {
    const int destProc = 0;
    comm.Send(&procRank, 1, MPI::INT, destProc, 0);
  }

  MPI::Finalize();
}
