#include "cuckoofilter/cuckoofilter.h"
#include "cuckoofilter/hashutil.h"
#include <time.h>


using namespace std;
using namespace cuckoofilter;

#ifndef FALLTHROUGH_INTENDED
#define FALLTHROUGH_INTENDED do { } while (0)
#endif

#ifndef PLATFORM_IS_LITTLE_ENDIAN
#define PLATFORM_IS_LITTLE_ENDIAN (__BYTE_ORDER == __LITTLE_ENDIAN)
#endif

static const bool kLittleEndian = PLATFORM_IS_LITTLE_ENDIAN;

inline uint32_t DecodeFixed32(const char* ptr) {
  if (kLittleEndian) {
    // Load the raw bytes
    uint32_t result;
    memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
    return result;
  } else {
    return ((static_cast<uint32_t>(static_cast<unsigned char>(ptr[0])))
        | (static_cast<uint32_t>(static_cast<unsigned char>(ptr[1])) << 8)
        | (static_cast<uint32_t>(static_cast<unsigned char>(ptr[2])) << 16)
        | (static_cast<uint32_t>(static_cast<unsigned char>(ptr[3])) << 24));
  }
}

uint32_t Hash(const char* data, size_t n, uint32_t seed) {
  // Similar to murmur hash
  const uint32_t m = 0xc6a4a793;
  const uint32_t r = 24;
  const char* limit = data + n;
  uint32_t h = seed ^ (n * m);

  // Pick up four bytes at a time
  while (data + 4 <= limit) {
    uint32_t w = DecodeFixed32(data);
    data += 4;
    h += w;
    h *= m;
    h ^= (h >> 16);
  }

  // Pick up remaining bytes
  switch (limit - data) {
    case 3:
      h += data[2] << 16;
      FALLTHROUGH_INTENDED;
    case 2:
      h += data[1] << 8;
      FALLTHROUGH_INTENDED;
    case 1:
      h += data[0];
      h *= m;
      h ^= (h >> r);
      break;
    default:
      break;
  }
  return h;
}

cuckoofilter::CuckooFilter<uint32_t, 12> build_cuckoo_filter(uint64_t cap) {
  cuckoofilter::CuckooFilter<uint32_t, 12> filter(cap);
  return filter;
}

#define get16bits(d) (*((const uint16_t *)(d)))

uint32_t NativeLevelDBHash(const char* data, size_t n, uint32_t seed) {
  // Similar to murmur hash
  const uint32_t m = 0xc6a4a793;
  const uint32_t r = 24;
  const char* limit = data + n;
  uint32_t h = seed ^ (n * m);

  // Pick up four bytes at a time
  while (data + 4 <= limit) {
    uint32_t w = DecodeFixed32(data);
    data += 4;
    h += w;
    h *= m;
    h ^= (h >> 16);
  }

  // Pick up remaining bytes
  switch (limit - data) {
    case 3:
      h += static_cast<uint8_t>(data[2]) << 16;
      FALLTHROUGH_INTENDED;
    case 2:
      h += static_cast<uint8_t>(data[1]) << 8;
      FALLTHROUGH_INTENDED;
    case 1:
      h += static_cast<uint8_t>(data[0]);
      h *= m;
      h ^= (h >> r);
      break;
  }
  return h;
}

uint32_t MurmurHash(const void *buf, size_t len, uint32_t seed) {
  // 'm' and 'r' are mixing constants generated offline.
  // They're not really 'magic', they just happen to work well.

  const unsigned int m = 0x5bd1e995;
  const int r = 24;

  // Initialize the hash to a 'random' value
  uint32_t h = seed ^ len;

  // Mix 4 bytes at a time into the hash
  const unsigned char *data = (const unsigned char *)buf;

  while (len >= 4) {
    unsigned int k = *(unsigned int *)data;

    k *= m;
    k ^= k >> r;
    k *= m;

    h *= m;
    h ^= k;

    data += 4;
    len -= 4;
  }

  // Handle the last few bytes of the input array
  switch (len) {
    case 3:
      h ^= data[2] << 16;
    case 2:
      h ^= data[1] << 8;
    case 1:
      h ^= data[0];
      h *= m;
  };

  // Do a few final mixes of the hash to ensure the last few
  // bytes are well-incorporated.
  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h;
}

uint32_t MurmurHash(const std::string &s, uint32_t seed) {
  return MurmurHash(s.data(), s.length(), seed);
}

void filter_test() {
    int succ_count, read_succ = 0;
    char _key[128] = { 0 };
    vector<uint32_t> read_detail;
    std::string _key_format = "%8d";
    cuckoofilter::CuckooFilter<uint32_t, 12> filter = build_cuckoo_filter(100000000);
    for (int i = 0; i < 100000; i++) {

        snprintf(_key, sizeof(_key), _key_format.c_str(), i);
        std::string key(_key);
        uint32_t hash = MurmurHash(key, 0);
        cuckoofilter::Status s = filter.Contain(hash);
        if (s == cuckoofilter::NotFound) {
            cuckoofilter::Status ws = filter.Add(hash);
            if (ws == cuckoofilter::Ok) {
                succ_count++;
            }
        } else {
            if (s == cuckoofilter::Ok) {
                read_succ++;
                read_detail.push_back(i);
            }
        }
         
        
    }
    cout << "Add succ count " << succ_count << endl;
    cout << "Read succ count " << read_succ << endl;
    
    // for (auto i : read_detail) {
    //     cout << i << ",";
    // }
}

void hash_vs() {
    char _key[128] = { 0 };
    size_t range = 1000000000;
    vector<string> keys;
    std::string _key_format = "%16d";
    for (int i = 0; i < range; i++) {
        snprintf(_key, sizeof(_key), _key_format.c_str(), i);
        keys.push_back(_key);
    }

    clock_t start,end;
    double sec;
    start = clock();
    for (int i = 0; i < range; i++) {
        uint32_t hash = MurmurHash(keys[i].data(), keys[i].size(), 0);
    }
    end = clock();
    sec = double(end - start) / CLOCKS_PER_SEC;
    printf("Murmurhash %lu records and seconds %lf s.\n", range, sec);


    start = clock();
    for (int i = 0; i < range; i++) {
        uint32_t hash = NativeLevelDBHash(keys[i].data(), keys[i].size(), 0);
    }
    end = clock();
    sec = double(end - start) / CLOCKS_PER_SEC;
    printf("NativeHash %lu records and seconds %lf s.\n", range, sec);


}

int main() {
    hash_vs();

    return 0;
}