#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

int main(int argc, char const *argv[]) {
  std::string filepath = "/data/data.txt";
  if (argc != 2) {
    fprintf(stderr, "usage: ./read_and_calculate filepath\n");
    exit(-1);
  } else {
    filepath = std::string(argv[1]);
  }

  std::ifstream ifs(filepath, std::ios::in);
  if (!ifs.is_open()) {
    fprintf(stderr, "file is not open\n");
    exit(-1);
  }
  std::unordered_map<std::string, std::pair<long long, long long>> map;
  std::string line;
  while (std::getline(ifs, line)) {
    int i = 0;
    while (line[i++] != ',');
    std::string c = line.substr(0, i-1);
    std::string v = line.substr(i, line.size()-i);
    long long t = std::stoll(v);
    if (map.find(c) != map.end()) {
      map[c].first = t;
      map[c].second = 1;
    } else {
      map[c].first += t;
      map[c].second++;
    }
  }
  for (auto it = map.begin(); it != map.end(); it++) {
    printf("%s: %.2f\n", it->first.c_str(), ((float)(it->second.first))/it->second.second);
  }
  ifs.close();
  return 0;
}
