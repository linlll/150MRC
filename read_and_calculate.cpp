#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdint.h>

#define POSSIBEL_AVERAGE_ROW_LENGTH 10

typedef struct FileStruct {
  std::ifstream *ifs;
  uint64_t begin;
  uint64_t end;
} FileStruct;

typedef std::unordered_map<std::string, std::pair<long long, long long>> result_t;
typedef result_t* result_pos_t;

void *work(void *arg) {
  FileStruct *fs = (FileStruct*)arg;
  std::string line;
  fs->ifs->seekg(fs->begin, std::ios::beg);
  result_t *map = new result_t;
  while (getline(*(fs->ifs), line)) {
    int i = 0;
    while (line[i++] != ',');
    std::string c = line.substr(0, i-1);
    std::string v = line.substr(i, line.size()-i);
    long long t = std::stoi(v);
    (*map)[c].first += t;
    (*map)[c].second++;
    uint64_t pos = fs->ifs->tellg();
    if (pos >= (uint64_t)(fs->end))
      break;
  }
  return map;
}

int main(int argc, char const *argv[]) {
/*
  // 计算每一行的平均长度
  std::string s;
  std::ifstream ifs(DATA_FILEPATH, std::ios::in);
  if (!ifs.is_open()) {
    fprintf(stderr, "file is not open\n");
    exit(-1);
  }
  long long a = 0, b = 0;
  while(getline(ifs, s)) {
    a += s.size();
    b++;
  }
  printf("%f\n", ((float)a)/b);
  exit(-1);
*/

  long long N = 150000000;
  std::string filepath = "/data/data.txt";
  if (argc != 3) {
    fprintf(stderr, "usage: ./data_generator number filepath\n");
    exit(-1);
  } else {
    N = std::stoll(std::string(argv[1]));
    filepath = std::string(argv[2]);
  }

  std::ifstream ifs1(filepath, std::ios::in);
  if (!ifs1.is_open()) {
    fprintf(stderr, "file is not open\n");
    exit(-1);
  }

  std::ifstream ifs2(filepath, std::ios::in);
  if (!ifs2.is_open()) {
    fprintf(stderr, "file is not open\n");
    exit(-1);
  }

  uint64_t split = (N/2)*POSSIBEL_AVERAGE_ROW_LENGTH;
  ifs1.seekg(split, std::ios::beg);
  while (ifs1.get() != '\n');
  split = ifs1.tellg();

  FileStruct *fs1 = new FileStruct;
  fs1->ifs = &ifs1;
  fs1->begin = 0;
  fs1->end = split;
  FileStruct *fs2 = new FileStruct;
  fs2->ifs = &ifs2;
  fs2->begin = split;
  fs2->end = UINT64_MAX;

  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, work, fs1);
  pthread_create(&tid2, NULL, work, fs2);

  result_t *ret1, *ret2;
  pthread_join(tid1, (void**)(&ret1));
  pthread_join(tid2, (void**)(&ret2));
  for (auto it = ret2->begin(); it != ret2->end(); it++) {
    std::string c = it->first;
    float t = it->second.first;
    long long n = it->second.second;
    (*ret1)[c].first += t;
    (*ret1)[c].second += n;
  }
  for (auto it = ret1->begin(); it != ret1->end(); it++) {
    printf("%s: %.2f\n", it->first.c_str(), ((float)(it->second.first))/it->second.second);
  }
  delete fs1;
  delete fs2;
  delete ret1;
  delete ret2;
  ifs1.close();
  ifs2.close();
  return 0;
}
