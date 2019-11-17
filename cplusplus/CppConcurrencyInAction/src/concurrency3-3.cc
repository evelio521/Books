#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>

//////////////1//////////////
std::shared_ptr<some_resource> resource_ptr;
std::once_flaf resource_flag;

void init_resource() {
  resource_ptr.reset(new some_resource);
}

void foo() {
  std::call_once(resource_flag, init_resource);
  resource_ptr->do_something();
}

struct connection_info {};

struct data_packet {};

struct connection_handle {
  void send_data(data_packet const&) {}
  data_packet receive_data() {
    return data_packet();
  }
};

struct remote_connection_manager {
  connection_handle open(connection_info const&) {
    return connection_handle();
  }
} connection_manager;


class X {
 private:
  connection_info connection_details;
  connection_handle connection;
  std::once_flag connection_init_flag;

  void open_connection() {
    connection=connection_manager.open(connection_details);
  }
 public:
  X(connection_info const& connection_details_):
      connection_details(connection_details_) {}
  void send_data(data_packet const& data) {
    std::call_once(connection_init_flag,&X::open_connection,this);
    connection.send_data(data);
  }
  data_packet receive_data() {
    std::call_once(connection_init_flag,&X::open_connection,this);
    return connection.receive_data();
  }
};

//////////////2//////////////
#include <map>
#include <string>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>

class dns_entry {};

class dns_cache {
  std::map<std::string,dns_entry> entries;
  boost::shared_mutex entry_mutex;
 public:
  dns_entry find_entry(std::string const& domain) {
    boost::shared_lock<boost::shared_mutex> lk(entry_mutex);
    std::map<std::string,dns_entry>::const_iterator const it=
        entries.find(domain);
    return (it==entries.end())?dns_entry():it->second;
  }
  void update_or_add_entry(std::string const& domain,
                           dns_entry const& dns_details) {
    std::lock_guard<boost::shared_mutex> lk(entry_mutex);
    entries[domain]=dns_details;
  }
};

int main() {
  std::cout <<"End\n";
}
