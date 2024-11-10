#include <mpi.h>

#include <ctime>
#include <iostream>

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  MPI::Intracomm &worldComm = MPI::COMM_WORLD;

  double time = 0, start, end;

  const int procRank = worldComm.Get_rank();
  const int procCount = worldComm.Get_size();

  const int N = (procRank + 1) % 2;
  const int color = N == 1 ? 1 : MPI::UNDEFINED;

  start = MPI::Wtime();
  MPI::Intracomm groupComm = worldComm.Split(color, MPI::UNDEFINED);
  end = MPI::Wtime();
  time += end - start;

  if (N == 1) {
    const int groupRank = groupComm.Get_rank();
    const double A = 3.0 + 0.1 * groupRank;
    std::cout << "[" << procRank << "/" << groupRank << "] A = " << A
              << std::endl;
    double buf;

    start = MPI::Wtime();
    groupComm.Allreduce(&A, &buf, 1, MPI::DOUBLE, MPI::SUM);
    end = MPI::Wtime();
    time += end - start;

    std::cout << "[" << procRank << "/" << groupRank << "] Result = " << buf
              << std::endl;

    std::cout << "[" << procRank << "/" << groupRank << "] Time: " << time
              << "s" << std::endl;
  }

  MPI::Finalize();
}
