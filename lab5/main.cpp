#include <mpi.h>

#include <array>
#include <ctime>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  MPI::Intracomm &worldComm = MPI::COMM_WORLD;

  const int procRank = worldComm.Get_rank();
  const int procCount = worldComm.Get_size();

  const int N = (procRank + 1) % 2;
  const double A = 3.0 + 0.1 * procRank;

  MPI::Intracomm groupComm = worldComm.Split(N, procRank);

  if (N == 1) {
    const int groupRank = groupComm.Get_rank();
    std::cout << "[" << procRank << "/" << groupRank << "] A = " << A
              << std::endl;
    double buf;
    groupComm.Allreduce(&A, &buf, 1, MPI::DOUBLE, MPI::SUM);
    std::cout << "[" << procRank << "/" << groupRank << "] Result = " << buf
              << std::endl;
  }

  MPI::Finalize();
}
