#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <chrono>
#include <thread>
#include "software_timer.hpp"

using std::cout, std::endl;
using SWtimer::SoftwareTimer;

long long get_current_time_in_ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void cb(void) {
  cout << "Timer says \"Hello\"\r\n";
}

int main() {
  SoftwareTimer<long long, get_current_time_in_ms, cb> tim;
  tim.is_autoreload_enabled = true;
  tim.period = 2500;

  cout << "Start\r\n";
  tim.start();

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    tim.check_timer();
  }
}
