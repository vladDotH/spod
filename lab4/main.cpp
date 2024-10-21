#include <mpi.h>

#include <array>
#include <ctime>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  MPI::Comm &comm = MPI::COMM_WORLD;

  const int rootProc = 0;
  const int procRank = comm.Get_rank();
  const int procCount = comm.Get_size();

  const double procPrivateNumber = 3. + procRank / 10.;

  double time = 0, start, end;

  if (procRank == rootProc) {
    double recvBuffer[procCount];

    start = MPI::Wtime();
    comm.Gather(&procPrivateNumber, 1, MPI::DOUBLE, recvBuffer, 1, MPI::DOUBLE,
                rootProc);
    end = MPI::Wtime();
    time += end - start;

    for (size_t i = 0; i < procCount; i++) {
      std::cout << "[" << i << "] -> " << recvBuffer[i] << std::endl;
    }

    std::cout << "[" << procRank << "]" << " time: " << time << "s."
              << std::endl;
  } else {
    comm.Gather(&procPrivateNumber, 1, MPI::DOUBLE, nullptr, 0, MPI::DOUBLE,
                rootProc);
  }

  MPI::Finalize();
}
