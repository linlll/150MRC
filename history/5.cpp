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

typedef std::unordered_map<char, std::pair<long long, long long>> result_t;
typedef result_t* result_pos_t;

void *work(void *arg) {
  FileStruct *fs = (FileStruct*)arg;
  std::string line;
  fs->ifs->seekg(fs->begin, std::ios::beg);
  result_t *map = new result_t;
  while (getline(*(fs->ifs), line)) {
    int i = 0;
    char c = line[0];
    switch (line[0]) {
    case 'z': i = 8; break;
    case 's': i = 8; break;
    case 'n': i = 7; break;
    case 'b': i = 7; break;
    default: break;
    }

    if (c == 's' && line[4] == 'g') c++;

    std::string v = line.substr(i+1, line.size()-i-1);
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
  long long N = 150000000;
  std::string filepath = "/data/data.txt";
  if (argc != 3) {
    fprintf(stderr, "usage: ./data_generator number filepath\n");
    exit(-1);
  } else {
    N = std::stoll(std::string(argv[1]));
    filepath = std::string(argv[2]);
  }

  const int thread_num = 18;

  std::ifstream ifs(filepath, std::ios::in);
  if (!ifs.is_open()) {
    fprintf(stderr, "file is not open\n");
    exit(-1);
  }

  FileStruct *fs = new FileStruct[thread_num];
  if (fs == nullptr) {
    fprintf(stderr, "malloc failed.\n");
    exit(-1);
  }
  uint64_t _split = 0;
  for (int i = 0; i < thread_num; i++) {
    uint64_t split = ((i+1)*N/thread_num)*POSSIBEL_AVERAGE_ROW_LENGTH;
    ifs.seekg(split, std::ios::beg);
    while (ifs.get() != '\n');
    split = ifs.tellg();

    fs[i].ifs = new std::ifstream(filepath, std::ios::in);
    if (!fs[i].ifs->is_open()) {
      fprintf(stderr, "file not open.\n");
      exit(-1);
    }
    fs[i].begin = _split;
    fs[i].end = split;
    _split = split;
  }
  fs[thread_num-1].end = UINT64_MAX;

  pthread_t *tid = new pthread_t[thread_num];
  if (tid == nullptr) {
    fprintf(stderr, "malloc failed.\n");
    exit(-1);
  }
  for (int i = 0; i < thread_num; i++) {
    pthread_create(&tid[i], NULL, work, &fs[i]);
  }

  result_t **ret = new result_t*[thread_num];
  for (int i = 0; i < thread_num; i++) {
    pthread_join(tid[i], (void**)(&ret[i]));
  }

  for (int i = thread_num-1; i > 0; i--) {
    for (auto it = ret[i]->begin(); it != ret[i]->end(); it++) {
      char c = it->first;
      float t = it->second.first;
      long long n = it->second.second;
      (*ret[i-1])[c].first += t;
      (*ret[i-1])[c].second += n;
    }
  }
  for (auto it = ret[0]->begin(); it != ret[0]->end(); it++) {
    std::string c;
    switch (it->first) {
    case 'z': c = "zhejiang"; break;
    case 'n': c = "nanjing"; break;
    case 't': c = "shanghai"; break;
    case 's': c = "shandong"; break;
    case 'b': c = "beijing"; break;
    default: break;
    }
    printf("%s: %.2f\n", c.c_str(), ((float)(it->second.first))/it->second.second);
  }
  for (int i = 0; i < thread_num; i++) {
    fs[i].ifs->close();
    delete fs[i].ifs;
  }
  delete fs;
  for (int i = 0; i < thread_num; i++) {
    delete ret[i];
  }
  delete ret;
  return 0;
}
