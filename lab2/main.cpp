#include <mpi.h>

#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  MPI::Comm &comm = MPI::COMM_WORLD;

  // Общее количество сообщений, которое должно быть отправлено
  const int msgCount = std::atoi(argv[1]);

  int procCount = comm.Get_size();
  int procRank = comm.Get_rank();

  if (procCount % 2 == 0 || procCount < 3) {
    throw MPI::Exception(0);
  }

  int group = procRank % 2;
  int groupsCount = (procCount - 1) / 2;

  double time = 0;

  // master: Отправляем количество сообщений каждой паре и ждём их обменов
  if (procRank == 0) {
    int msgPerPair = msgCount / groupsCount;
    int msgLastPair = msgPerPair + msgCount % groupsCount;

    double start = MPI::Wtime();
    
    for (size_t i = 0; i < groupsCount - 1; i++) {
      comm.Send(&msgPerPair, 1, MPI::INT, i * 2 + 1, 0);
    }
    comm.Send(&msgLastPair, 1, MPI::INT, (groupsCount - 1) * 2 + 1, 0);

    double end = MPI::Wtime();
    time += end - start;

    std::cout << "[master:0] end of sending " << std::endl;

    start = MPI::Wtime();
    for (size_t i = 0; i < groupsCount; i++) {
      comm.Recv(nullptr, 0, MPI::BOOL, i * 2 + 1, 0);
    }
    end = MPI::Wtime();
    time += end - start;

    std::cout << "[master:0]" << " time: " << time << "s." << std::endl;
  }
  // odd slave
  else if (group == 1) {
    int currentMsgCount;
    bool end = false;
    comm.Recv(&currentMsgCount, 1, MPI::INT, 0, MPI::ANY_TAG);

    std::cout << "[" << procRank
              << "] message count recieved: " << currentMsgCount << std::endl;

    for (size_t i = 0; i < currentMsgCount - 1; i++) {
      comm.Send(&end, 1, MPI::BOOL, procRank + 1, 0);
      comm.Recv(nullptr, 0, MPI::BOOL, procRank + 1, 0);
    }
    end = true;
    comm.Send(&end, 1, MPI::BOOL, procRank + 1, 0);
    comm.Recv(nullptr, 0, MPI::BOOL, procRank + 1, 0);
    
    std::cout << "[" << procRank << "] end of sending" << std::endl;
    comm.Send(nullptr, 0, MPI::BOOL, 0, 0);
  }
  // even slave
  else {
    bool end = false;
    while (!end) {
      comm.Recv(&end, 1, MPI::BOOL, procRank - 1, MPI::ANY_TAG);
      comm.Send(nullptr, 0, MPI::BOOL, procRank - 1, 0);
    }
    std::cout << "[" << procRank << "] end of recieving" << std::endl;
  }

  MPI::Finalize();
}
