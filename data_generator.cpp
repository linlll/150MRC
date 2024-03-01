#include <iostream>
#include <fstream>
#include <time.h>
#include <random>
#include <vector>

int main(int argc, char const *argv[]) {
  const std::vector<std::string> city = {"zhejiang", "nanjing", "shanghai", "beijing", "shandong"};
  std::mt19937 gen;
  std::vector<std::uniform_real_distribution<>> temp_dis = {
    std::uniform_real_distribution<>(0, 2.468),
    std::uniform_real_distribution<>(-10.2, 24.4997),
    std::uniform_real_distribution<>(-30, 9.76),
    std::uniform_real_distribution<>(-14.06, -10),
    std::uniform_real_distribution<>(25, 35.9)
  };
  std::uniform_int_distribution<> city_dis(0,4); // city
  std::ofstream ofs("data.txt", std::ios::out);

  clock_t start = clock();
  long long N = 150000000;
  while (N--) {
    int i = city_dis(gen);
    ofs << city[i] << "," << temp_dis[i](gen) << "\n";
  }

  ofs.close();
  return 0;
}
