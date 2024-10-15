#include <mpi.h>

#include <ctime>
#include <iostream>
#include <vector>

// Тег сообщения - посылка или синхронизация
enum Tag { message, sync };

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  MPI::Comm &comm = MPI::COMM_WORLD;

  // Общее количество посылок от первой группы
  const int msgCount = std::atoi(argv[1]);

  const int procCount = comm.Get_size();
  const int procRank = comm.Get_rank();

  if (procCount < 2) {
    std::cerr << "Incorrect processes number" << std::endl;
    comm.Abort(0);
  }

  std::srand(std::time(0));

  // Данныео процессе и его группе
  const int group = procRank % 2;
  const int oddCount = procCount / 2;
  const int evenCount = procCount / 2 + procCount % 2;
  const int procIndexInGroup = procRank / 2;

  MPI::Status status;

  double time = 0, start, end;

  if (group == 1) {
    // Сколько сообщений должен отправить данный процесс
    int msgToSend = msgCount / oddCount +
                    (procIndexInGroup < (msgCount % oddCount) ? 1 : 0);

    for (size_t i = 0; i < msgCount; i++) {
      // Пока не отправлено нужное количество - отправляем
      if (i < msgToSend) {
        int dest = 2 * (std::rand() % evenCount);
        start = MPI::Wtime();
        comm.Send(nullptr, 0, MPI::INT, dest, Tag::message);
        end = MPI::Wtime();
        time += (end - start);
        std::cout << "[" << procRank << "] sent to " << dest << std::endl;
      }

      // Ждём ответ от чётного процесса или синхронизацию от нечётного

      start = MPI::Wtime();
      comm.Recv(nullptr, 0, MPI::INT, MPI::ANY_SOURCE, MPI::ANY_TAG, status);
      end = MPI::Wtime();
      time += (end - start);

      if (status.Get_tag() == Tag::sync) {
        std::cout << "[" << procRank << "] recieved sync from "
                  << status.Get_source() << std::endl;
      } else if (status.Get_tag() == Tag::message) {
        std::cout << "[" << procRank << "] recieved from "
                  << status.Get_source() << std::endl;

        // Если это ответ от чётного - отправляем посылку для синхронизации
        // другим нечётным процессам
        for (size_t i = 0; i < oddCount; i++) {
          int dest = (2 * i) + 1;
          if (dest == procRank) continue;
          start = MPI::Wtime();
          comm.Send(nullptr, 0, MPI::INT, dest, Tag::sync);
          end = MPI::Wtime();
          time += (end - start);
        }
      }
    }
  } else {
    for (size_t i = 0; i < msgCount; i++) {
      start = MPI::Wtime();
      comm.Recv(nullptr, 0, MPI::INT, MPI::ANY_SOURCE, MPI::ANY_TAG, status);
      end = MPI::Wtime();
      time += (end - start);

      if (status.Get_tag() == Tag::sync) {
        std::cout << "[" << procRank << "] recieved sync from "
                  << status.Get_source() << std::endl;
      } else if (status.Get_tag() == Tag::message) {
        std::cout << "[" << procRank << "] recieved from "
                  << status.Get_source() << std::endl;
        // Если пришло сообщение от нечётного - отправляем посылку для
        // синхронизации другим чётным процессам
        for (size_t i = 0; i < evenCount; i++) {
          int dest = 2 * i;
          if (dest == procRank) continue;
          start = MPI::Wtime();
          comm.Send(nullptr, 0, MPI::INT, dest, Tag::sync);
          end = MPI::Wtime();
          time += (end - start);
        }

        // И ответ нечётному процессу
        int dest = 2 * (std::rand() % oddCount) + 1;
        start = MPI::Wtime();
        comm.Send(nullptr, 0, MPI::INT, dest, Tag::message);
        end = MPI::Wtime();
        time += (end - start);
      }
    }
  }

  std::cout << "[" << procRank << "]" << " time: " << time << "s." << std::endl;

  MPI::Finalize();
}
