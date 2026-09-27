#pragma once

/**
 * К таймеру:
- использовать концепт для типа времени (лучше по поддерживаемым операциям, чтобы можно было использовать классы с переопределёнными параметрами)
- убрать в check возвращение переменной или поменять смысл
- добавить опцию цикличности, чтобы автоматически его перезапускать или ограничиться одним срабатыванием
 */

namespace SWtimer {

template<typename T, T (*time_getter)(void), void (*callback)(void) = nullptr>
class SoftwareTimer {
public:
  /**
   * @brief Init to call instead of constructor with parameters
   */
  void init(T timer_period) {
    period = timer_period;
  }

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
   * @brief Update timer ticks and flag
   */
  void update(void) {
    if (is_enabled_status_) {
      timestamp_ = time_getter();
    }
  }

  /**
   * @brief Reset the timer, set 0 to all ticks and false to flag
   */
  void reset(void) {
    period_elapsed = false;
    timestamp = 0;
    period = 0;
  }

  /**
   * @brief Check if timer period is elapsed
   */
  bool check(void) {
    // TODO: think about using code from `update` method
    if (is_enabled_status_) {
      period_elapsed_ = (time_getter() - timestamp_) >= period;
      if (period_elapsed_ && callback) {
        // to use callback when period is elapsed
        // TODO: use additional options connected with autoreloading etc.
        callback();
      }
      return period_elapsed_;
    }
    return false;
  }

  /**
   * @brief Get is enabled timer state
   */
  bool is_enabled(void) {
    return is_enabled_status_;
  }

  /**
   * @brief Get is timer period elapsed
   */
  bool is_period_elapsed(void) {
    return period_elapsed_;
  }
public:
    T period{0};
protected:
    T timestamp_{0};
    bool is_enabled_status_{false};
    bool period_elapsed_{false};
};

} /* namespace SWtimer */
