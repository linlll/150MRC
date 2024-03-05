#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef struct FileStruct {
  char *start_addr;
  long long size;
} FileStruct;

void *work(void *arg) {
  FileStruct *fs = (FileStruct*)arg;

  long long *ret = new long long[10];
  memset(ret, 0, sizeof(long long)*10);
  char *addr = fs->start_addr;
  long long size = 0;
  while (1) {
    int i = 0, j;
    char c = addr[0];
    switch (c) {
    case 'z': i = 8; j = 0; break;
    case 's': i = 8; j = 1; break;
    case 'n': i = 7; j = 3; break;
    case 'b': i = 7; j = 4; break;
    default: break;
    }

    if (c == 's' && addr[4] == 'g') {
      c++;
      j++;
    }

    int t = 0;
    int sign = 1;
    i++;
    if (addr[i] == '-') {
      sign = -1;
      i++;
    }
    while (addr[i] != '\n') {
      t = 10 * t + addr[i] - '0';
      i++;
    }
    t *= sign;

    ret[2*j] += t;
    ret[2*j+1]++;

    addr += i + 1;
    size += i + 1;
    if (size >= fs->size) {
      break;
    }
  }
  return ret;
}

int main(int argc, char const *argv[]) {
  std::string filepath = "data.txt";
  if (argc != 2) {
    fprintf(stderr, "usage: ./data_generator filepath\n");
    exit(-1);
  } else {
    filepath = std::string(argv[1]);
  }

  const int thread_num = 18;

  int fd = open(filepath.c_str(), O_RDONLY);
  if (fd < 0) {
    fprintf(stderr, "file not open.\n");
    exit(-1);
  }

  struct stat attr;
  if (fstat(fd, &attr) < 0) {
    fprintf(stderr, "fstat failed.\n");
    exit(-1);
  }

  FileStruct *fs = new FileStruct[thread_num];
  if (fs == nullptr) {
    fprintf(stderr, "malloc failed.\n");
    exit(-1);
  }

  void *addr = mmap(NULL, attr.st_size, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0);
  if (addr == nullptr) {
    fprintf(stderr, "mmap failed.\n");
    exit(-1);
  }

  uint64_t _offset = 0;
  for (int i = 0; i < thread_num; i++) {
    uint64_t offset = (i+1)*attr.st_size/thread_num;
    char c;
    while (1) {
      c = ((char*)addr)[offset];
      if (c == '\n')
        break;
      offset++;
    }
    offset = MIN(offset, attr.st_size);
    fs[i].start_addr = &((char*)addr)[_offset];
    fs[i].size = offset - _offset + 1;
    _offset = offset + 1;
  }
  fs[thread_num-1].size--;

  pthread_t *tid = new pthread_t[thread_num];
  if (tid == nullptr) {
    fprintf(stderr, "malloc failed.\n");
    exit(-1);
  }

  for (int i = 0; i < thread_num; i++) {
    pthread_create(&tid[i], NULL, work, &fs[i]);
  }

  long long **ret = new long long*[thread_num];
  for (int i = 0; i < thread_num; i++) {
    pthread_join(tid[i], (void**)(&ret[i]));
  }

  for (int i = thread_num-1; i > 0; i--) {
    for (int j = 0; j < 5; j++) {
      ret[i-1][2*j] += ret[i][2*j];
      ret[i-1][2*j+1] += ret[i][2*j+1];
    }
  }

  printf("%s: %.2f\n", "zhejiang", ((float)ret[0][0])/ret[0][1]);
  printf("%s: %.2f\n", "shandong", ((float)ret[0][2])/ret[0][3]);
  printf("%s: %.2f\n", "shanghai", ((float)ret[0][4])/ret[0][5]);
  printf("%s: %.2f\n", "nanjing", ((float)ret[0][6])/ret[0][7]);
  printf("%s: %.2f\n", "beijing", ((float)ret[0][8])/ret[0][9]);

  if (munmap(addr, attr.st_size) < 0) {
    fprintf(stderr, "munmap failed.\n");
    exit(-1);
  }
  delete fs;
  for (int i = 0; i < thread_num; i++) {
    delete ret[i];
  }
  delete ret;
  return 0;
}
