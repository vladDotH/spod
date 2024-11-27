#include <mpi.h>

#include <array>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

template <typename T>
void readMatrixToBlocksArray(std::ifstream &file, std::vector<T> &vec, size_t N,
                             size_t Q) {
  auto q = N / Q, K = q * q;
  T value;
  for (size_t y = 0; y < N; y++) {
    for (size_t x = 0; x < N; x++) {
      auto blockX = x / q, blockY = y / q, blockI = blockY * Q + blockX,
           padY = y % q, padX = x % q;
      file >> value;
      vec[blockI * K + padY * q + padX] = value;
    }
  }
}

template <typename TA, typename TB, typename TC>
void multiplicateBlocksArray(std::vector<TA> &A, std::vector<TB> &B,
                             std::vector<TC> &C, size_t q) {
  for (size_t i = 0; i < q * q; i++) {
    auto y = i / q, x = i % q;
    for (size_t j = 0; j < q; j++) {
      C[i] += A[y * q + j] * B[j * q + x];
    }
  }
}

int main(int argc, char **argv) {
  const int ROOT = 0;
  double time = 0, start, end;

  MPI::Init(argc, argv);
  MPI::Intracomm &worldComm = MPI::COMM_WORLD;

  const size_t blocks = worldComm.Get_size();

  // Размер матрицы в блоках
  size_t Q = std::sqrt(blocks);

  if (Q * Q != blocks) {
    std::cerr << "Incorrect blocks number (must be square)\n";
    worldComm.Abort(MPI::ERR_ARG);
  }

  // Создаём группу с декартовой топологией
  const int dims[] = {int(Q), int(Q)};
  const bool periods[] = {true, true};
  MPI::Cartcomm cartComm = worldComm.Create_cart(2, dims, periods, true);

  // Ранг в решётке
  const size_t rank = cartComm.Get_rank();

  // Размер и длина блока
  size_t q, blockSize;
  // Размер матриц
  size_t N;

  // Входные блоки
  std::vector<int> A, B, sendBufA, sendBufB;
  // Выходной блок
  std::vector<double> C;

  // Исходные Массивы блоков (для главного процесса)
  std::vector<int> arrA, arrB;
  // Выходной масив блоков главного процесса
  std::vector<double> arrC;

  // Имя файла (для главного процесса)
  std::string fileName;

  // Чтение входных данных из файла
  if (rank == ROOT) {
    fileName = argv[1];

    std::ifstream file(fileName);
    file >> N;

    if (N % Q != 0) {
      std::cerr << "Incorrect blocks number (Q must be divisor of N)\n";
      worldComm.Abort(MPI::ERR_ARG);
    }

    arrA = std::vector<int>(N * N);
    arrB = std::vector<int>(N * N);
    arrC = std::vector<double>(N * N);

    readMatrixToBlocksArray(file, arrA, N, Q);
    readMatrixToBlocksArray(file, arrB, N, Q);

    file.close();
  }

  start = MPI::Wtime();

  // Отправляем други процесса размер исходных матриц
  cartComm.Bcast(&N, 1, MPI::UNSIGNED_LONG, ROOT);

  q = N / Q;
  blockSize = q * q;

  // Аллокация блоков
  A = std::vector<int>(blockSize);
  B = std::vector<int>(blockSize);
  sendBufA = std::vector<int>(blockSize);
  sendBufB = std::vector<int>(blockSize);
  C = std::vector<double>(blockSize, 0);

  // Первоначальное распределение блоков матриц А и В
  cartComm.Scatter(arrA.data(), blockSize, MPI::INT, A.data(), blockSize,
                   MPI::INT, ROOT);
  cartComm.Scatter(arrB.data(), blockSize, MPI::INT, B.data(), blockSize,
                   MPI::INT, ROOT);

  // Ранги соседей по X и Y
  int prevX, nextX, prevY, nextY;

  // Координаты текущего процесса и буфер для координат соседей
  std::array<int, 2> pos, posBuf;
  cartComm.Get_coords(rank, 2, pos.data());

  // Ранги соседей в строке и столбце для начального сдвига
  posBuf = {pos[0], pos[1] + pos[0]};
  nextX = cartComm.Get_cart_rank(posBuf.data());
  posBuf = {pos[0], pos[1] - pos[0]};
  prevX = cartComm.Get_cart_rank(posBuf.data());

  posBuf = {pos[0] + pos[1], pos[1]};
  nextY = cartComm.Get_cart_rank(posBuf.data());
  posBuf = {pos[0] - pos[1], pos[1]};
  prevY = cartComm.Get_cart_rank(posBuf.data());

  MPI::Request reqA, reqB;
  std::array<MPI::Request, 2> reqs;

  std::copy(A.begin(), A.end(), sendBufA.begin());
  std::copy(B.begin(), B.end(), sendBufB.begin());
  // Начальный сдвиг по рядам А и колонкам B
  reqA = cartComm.Isend(sendBufA.data(), blockSize, MPI::INT, nextX, 0);
  reqB = cartComm.Isend(sendBufB.data(), blockSize, MPI::INT, nextY, 0);
  cartComm.Recv(A.data(), blockSize, MPI::INT, prevX, MPI::ANY_TAG);
  cartComm.Recv(B.data(), blockSize, MPI::INT, prevY, MPI::ANY_TAG);
  reqs = {reqA, reqB};
  MPI::Request::Waitall(2, reqs.data());

  // Получаем ближайших соседей по X и Y для основного сдвига
  cartComm.Shift(0, 1, prevY, nextY);
  cartComm.Shift(1, 1, prevX, nextX);

  for (size_t i = 0; i < Q; i++) {
    // Умножаем текущие блоки
    multiplicateBlocksArray(A, B, C, q);
    std::copy(A.begin(), A.end(), sendBufA.begin());
    std::copy(B.begin(), B.end(), sendBufB.begin());
    // Отправляем и принимаем следующие блоки в ряду и столбце
    reqA = cartComm.Isend(sendBufA.data(), blockSize, MPI::INT, nextX, 0);
    reqB = cartComm.Isend(sendBufB.data(), blockSize, MPI::INT, nextY, 0);
    cartComm.Recv(A.data(), blockSize, MPI::INT, prevX, MPI::ANY_TAG);
    cartComm.Recv(B.data(), blockSize, MPI::INT, prevY, MPI::ANY_TAG);
    reqs = {reqA, reqB};
    MPI::Request::Waitall(2, reqs.data());
  }

  // Сборка блоков результата
  cartComm.Gather(C.data(), blockSize, MPI::DOUBLE, arrC.data(), blockSize,
                  MPI::DOUBLE, ROOT);

  end = MPI::Wtime();
  time += end - start;

  // Вывод результата
  if (rank == ROOT) {
    std::ofstream file("result-par." + fileName);
    for (size_t y = 0; y < N; y++) {
      for (size_t x = 0; x < N; x++) {
        auto blockX = x / q, blockY = y / q, blockI = blockY * Q + blockX,
             padY = y % q, padX = x % q;

        file << arrC[blockI * blockSize + padY * q + padX] << " ";
      }
      file << std::endl;
    }
    file.close();
    std::cout << time << "s." << std::endl;
  }

  MPI::Finalize();
}
