#include "dpic.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

static const size_t MAX_WORDS = 1024 * 1024 / 4; // 1MiB / 4 bytes
static uint32_t pmem_words[MAX_WORDS];
static size_t pmem_words_size = 0;

static uint32_t dynamic_pc_inst[MAX_WORDS];
static size_t dynamic_pc_inst_size = 0;

static const uint32_t default_pc_inst[] = {
  0x000010b7,
  0x02008093,
  0x00500113,
  0x002081b3,
  0x00302023,
  0x00002203,
  0x00200223,
  0x00404283,
  0x024003e7,
  0x00c00067,
};

bool load_hex_program(const char *path) 
{
  FILE *fp = fopen(path, "r");
  if (!fp) 
  {
    perror("load_hex fopen");
    return false;
  }

  char line[512];
  size_t cur = 0;
  size_t max_idx = 0;

  // pass 1: compute max word index
  while (fgets(line, sizeof(line), fp)) {
    char *tok = strtok(line, " \t\r\n");
    while (tok) {
      if (tok[0] == '#') break;

      size_t len = strlen(tok);
      if (len > 0 && tok[len - 1] == ':') {
        tok[len - 1] = '\0';
        cur = strtoul(tok, NULL, 16); // v3 words addressed
      } else {
        char *end = NULL;
        unsigned long v = strtoul(tok, &end, 16);
        if (end != tok && *end == '\0') {
          (void)v;
          if (cur + 1 > max_idx) max_idx = cur + 1;
          cur++;
        }
      }

      tok = strtok(NULL, " \t\r\n");
    }
  }

  if (max_idx == 0) {
    fclose(fp);
    printf("load_hex: no valid instruction in %s\n", path);
    return false;
  }

  if (max_idx > MAX_WORDS) {
    fclose(fp);
    printf("load_hex: program too large (%zu words), max %zu\n", max_idx, MAX_WORDS);
    return false;
  }

  dynamic_pc_inst_size = max_idx;
  for (size_t i = 0; i < dynamic_pc_inst_size; i++) {
    dynamic_pc_inst[i] = 0;
  }

  pmem_words_size = MAX_WORDS;
  for (size_t i = 0; i < pmem_words_size; i++) {
    pmem_words[i] = 0;
  }

  // pass 2: load instructions/data
  rewind(fp);
  cur = 0;
  size_t loaded = 0;

  while (fgets(line, sizeof(line), fp)) {
    char *tok = strtok(line, " \t\r\n");
    while (tok) {
      if (tok[0] == '#') break;

      size_t len = strlen(tok);
      if (len > 0 && tok[len - 1] == ':') {
        tok[len - 1] = '\0';
        cur = strtoul(tok, NULL, 16); // v3 words addressed
      } else {
        char *end = NULL;
        unsigned long v = strtoul(tok, &end, 16);
        if (end != tok && *end == '\0') {
          if (cur < dynamic_pc_inst_size) {
            dynamic_pc_inst[cur] = (uint32_t)v;
            if (cur < pmem_words_size) {
              pmem_words[cur] = (uint32_t)v;
            }
            loaded++;
          }
          cur++;
        }
      }

      tok = strtok(NULL, " \t\r\n");
    }
  }

  fclose(fp);
  printf("load_hex: allocated %zu words, loaded %zu instructions from %s\n", dynamic_pc_inst_size, loaded, path);
  return true;
}

uint32_t pc_read(uint32_t addr) {
  uint32_t index = addr >> 2;
  if (index < dynamic_pc_inst_size) {
    return dynamic_pc_inst[index];
  }
  if (index < sizeof(default_pc_inst) / sizeof(default_pc_inst[0])) {
    return default_pc_inst[index];
  }
  return 0;
}

extern "C" void npc_ebreak(int code) {
  printf("DPI-C: ebreak at PC=0x%08x\n", code);
  fflush(stdout);
  exit(0);
}

void init_pmem(size_t bytes) {
  size_t words = (bytes + 3) / 4;
  if (words == 0) words = 1;
  if (words > MAX_WORDS) words = MAX_WORDS;

  pmem_words_size = words;
  for (size_t i = 0; i < pmem_words_size; i++) {
    pmem_words[i] = 0;
  }
}

extern "C" uint32_t pmem_read_u32(uint32_t raddr) {
  uint32_t index = raddr >> 2;
  if (index >= pmem_words_size) {
    printf("p read out range : 0x%08x\n", raddr);
    return 0;
  }
  return pmem_words[index];
}

extern "C" uint8_t pmem_read_u8(uint32_t raddr) {
  uint32_t index = raddr >> 2;
  if (index >= pmem_words_size) {
    printf("mem read out of range : 0x%08x\n", raddr);
    return 0;
  }
  uint32_t word = pmem_words[index];
  uint32_t byte_off = raddr & 3u;
  return (word >> (byte_off * 8)) & 0xff;
}

extern "C" void pmem_write_u32(uint32_t waddr, uint32_t wdata) {
  uint32_t index = waddr >> 2;
  if (index >= pmem_words_size) {
    printf("mem write out of range : 0x%08x\n", waddr);
    return;
  }
  printf("pmem_write_u32 addr=0x%08x data=0x%08x\n", waddr, wdata);
  fflush(stdout);
  pmem_words[index] = wdata;
}

extern "C" void pmem_write_u8(uint32_t addr, uint8_t data) {
  uint32_t index = addr >> 2;
  if (index >= pmem_words_size) {
    fprintf(stderr, "pmem_write_u8 out of range addr=0x%08x\n", addr);
    return;
  }
  uint32_t byte_off = addr & 3u;
  uint32_t word = pmem_words[index];
  uint32_t mask = ~(0xffu << (byte_off * 8));
  pmem_words[index] = (word & mask) | ((uint32_t)data << (byte_off * 8));
  printf("pmem_write_u8 addr=0x%08x data=0x%02x\n", addr, data);
  fflush(stdout);
}




