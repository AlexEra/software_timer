#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <chrono>
#include <thread>
#include "software_timer.hpp"

// #define TEST_AUTORELOAD_WITH_CALLBACK
// #define TEST_ONESHOT_WITH_CALLBACK
#define TEST_ONESHOT_WITHOUT_CALLBACK

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
#if defined(TEST_AUTORELOAD_WITH_CALLBACK) || defined(TEST_ONESHOT_WITH_CALLBACK)
  SoftwareTimer<long long, get_current_time_in_ms, cb> tim;
#elif defined(TEST_ONESHOT_WITHOUT_CALLBACK)
  SoftwareTimer<long long, get_current_time_in_ms> tim;
#endif
  tim.period = 2500;

#ifdef TEST_AUTORELOAD_WITH_CALLBACK
  tim.is_autoreload_enabled = true;
#endif

  cout << "Start\r\n";
  tim.start();

#ifdef TEST_AUTORELOAD_WITH_CALLBACK
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    tim.check_timer();
  }
#elif defined (TEST_ONESHOT_WITH_CALLBACK) || defined(TEST_ONESHOT_WITHOUT_CALLBACK)
  while (true) {
    if (tim.check_timer()) {
      break;
    }
  }
  cout << "Finished\n";
#endif
}
