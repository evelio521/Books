////////////////////////1/////////////////
struct protected_data
{
  std::mutex m;
  char padding[65536];  // 65536字节已经超过一个缓存行的数量级
  my_data data_to_protect;
};

struct my_data
{
  data_item1 d1;
  data_item2 d2;
  char padding[65536];
};
my_data some_array[256];
