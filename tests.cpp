#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <chrono>
#include <thread>
#include "software_timer.hpp"

// #define TEST_AUTORELOAD_WITH_CALLBACK
// #define TEST_ONESHOT_WITH_CALLBACK
// #define TEST_ONESHOT_WITHOUT_CALLBACK
#define CONCEPT_TEST

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

#ifdef CONCEPT_TEST
struct tt_t {
  long long val;

  tt_t(int value = 0) : val{static_cast<long long>(value)} {  }

  tt_t(tt_t &other) : val{other.val} {  }

  tt_t operator-(const tt_t &other) {
    tt_t ret;
    ret.val = val - other.val;
    return ret;
  }

  tt_t& operator=(int value) {
    val = static_cast<long long>(value);
    return *this;
  }

  tt_t &operator=(const tt_t &other) {
    val = other.val;
    return *this;
  }

  bool operator>=(const tt_t& other) {
    return val >= other.val;
  }

  tt_t& operator++() {
    val++;
    return *this;
  }
};

static tt_t tt_instance{0};

tt_t get_tt(void) {
  tt_instance.val++;
  return tt_instance;
}
#endif /* CONCEPT_TEST */

int main() {
#if defined(TEST_AUTORELOAD_WITH_CALLBACK) || defined(TEST_ONESHOT_WITH_CALLBACK)
  SoftwareTimer<long long, get_current_time_in_ms, cb> tim;
#elif defined(TEST_ONESHOT_WITHOUT_CALLBACK)
  SoftwareTimer<long long, get_current_time_in_ms> tim;
#elif defined (CONCEPT_TEST)
  SoftwareTimer<tt_t, get_tt> tim;
#endif
  tim.period = 1;

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
#elif defined (TEST_ONESHOT_WITH_CALLBACK) || defined(TEST_ONESHOT_WITHOUT_CALLBACK) || defined(CONCEPT_TEST)
  while (true) {
    if (tim.check_timer()) {
      break;
    }
  }
  cout << "Finished\n";
#endif
}
