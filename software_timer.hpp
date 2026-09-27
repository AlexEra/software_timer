#pragma once

/**
 * К таймеру:
- использовать концепт для типа времени (лучше по поддерживаемым операциям, чтобы можно было использовать классы с переопределёнными параметрами)
 */

namespace SWtimer {

template<typename T, T (*time_getter)(void), void (*callback)(void) = nullptr>
class SoftwareTimer {
public:
  /**
   * @brief Start timer
   */
  void start(void) {
    timestamp_ = time_getter();
    period_elapsed_ = false;
    is_enabled_status_ = true;
  }

  /**
   * @brief Stop timer
   */
  void stop(void) {
    is_enabled_status_ = false;
    period_elapsed_ = false;
  }

  /**
   * @brief Reset the timer, set 0 to all ticks and false to flag
   */
  void reset(void) {
    period_elapsed_ = false;
    timestamp_ = 0;
    period = 0;
  }

  /**
   * @brief Check if timer period is elapsed
   */
  bool check_timer(void) {
    if (!is_enabled_status_) {
      return false;
    }
    period_elapsed_ = (time_getter() - timestamp_) >= period;
    if (period_elapsed_) {
      if (callback) {
        // to use callback when period is elapsed
        callback();
      }
      if (is_autoreload_enabled) {
        // work in the loop
        period_elapsed_ = false;
        timestamp_ = time_getter();
      } else {
        // single job
        is_enabled_status_ = false; 
      }
    }
    return period_elapsed_;
  }

  /**
   * @brief Get is enabled timer state
   */
  bool is_enabled(void) const {
    return is_enabled_status_;
  }

  /**
   * @brief Get is timer period elapsed
   */
  bool is_period_elapsed(void) const {
    return period_elapsed_;
  }

public:
  T period{0}; // FIXME: type T should be able to casting to int
  bool is_autoreload_enabled{false};
protected:
  T timestamp_{0}; // FIXME: type T should be able to casting to int
  bool is_enabled_status_{false};
  bool period_elapsed_{false};
};

} /* namespace SWtimer */
