#include <iostream>
#include <fstream>
#include <time.h>
#include <random>
#include <vector>

int main(int argc, char const *argv[]) {
  long long N = 150000000;
  std::string filepath = "/data/data.txt";
  if (argc != 3 && argc != 2) {
    fprintf(stderr, "usage: ./data_generator [number] filepath\n");
    exit(-1);
  }
  if (argc == 2) {
    filepath = std::string(argv[1]);
  }
  if (argc == 3) {
    N = std::stoll(std::string(argv[1]));
    filepath = std::string(argv[2]);
  }

  const std::vector<std::string> city = {"zhejiang", "nanjing", "shanghai", "beijing", "shandong"};
  std::mt19937 gen;
  std::vector<std::uniform_int_distribution<>> temp_dis = {
    std::uniform_int_distribution<>(0, 2),
    std::uniform_int_distribution<>(-10, 24),
    std::uniform_int_distribution<>(-30, 9),
    std::uniform_int_distribution<>(-14, -10),
    std::uniform_int_distribution<>(25, 35)
  };
  std::uniform_int_distribution<> city_dis(0,4); // city
  std::ofstream ofs(filepath, std::ios::out);
  if (!ofs.is_open()) {
    fprintf(stderr, "file is not open\n");
    exit(-1);
  }

  while (N--) {
    int i = city_dis(gen);
    ofs << city[i] << "," << temp_dis[i](gen) << "\n";
  }

  ofs.close();
  return 0;
}
