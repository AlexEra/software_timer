#pragma once

/**
 * К таймеру:
- можно добавить коллбек
- использовать концепт для типа времени (лучше по поддерживаемым операциям, чтобы можно было использовать классы с переопределёнными параметрами)
- разобраться с конструктором (соблюсти правило 5 или 0)
- подумать над тем, чтобы засунуть адрес функции в качестве аргумента шаблона к классу — это для ускорения работы, чтобы заинлайнить вызов и не использовать сам указатель, потому что он нигде не будет храниться;
- добавить флаг старта и использовать его, чтобы не занулять каждый вызов метода update
- как только period_elapsed == true, то вызываем callback, если он не равен nullptr
- это делать в методе check, убрав возвращение переменной или поменяв смысл
- добавить опцию цикличности, чтобы автоматически его перезапускать или ограничиться одним срабатыванием
 */

namespace SWtimer {

template<typename T, T (*time_getter)(void), void (*callback)(void *) = nullptr>
class SoftwareTimer {
public:
  /*
    * @brief Init to call instead of constructor with parameters
    */
  void init(T timer_period) {
      period = timer_period;
  }
  /*
  * @brief Start timer
  */
  void start(void) {
    if (p_current_time_getter != nullptr) {
      timestamp = p_current_time_getter();
      period_elapsed = false;
      is_enabled_status = true;
    }
  }
  void stop(void);
  void update(void);
  void reset(void);
  bool check(void);
  bool is_enabled(void);
  bool is_period_elapsed(void);
public:
    T period{0};
protected:
    T timestamp{0};
    bool is_enabled_status{false};
    bool period_elapsed{false};
private:
    T (*p_current_time_getter) (void) { nullptr };
};

/*
 * @brief Stop timer
 */
template<typename T, T (*time_getter)(void), void (*callback)(void *)>
SoftwareTimer<T, time_getter, callback>::stop(void) {
    is_enabled_status = false;
}

/*
 * @brief Update timer ticks and flag
 */
template<typename T, T (*time_getter)(void), void (*callback)(void *)>
SoftwareTimer<T, time_getter, callback>::update(void) {
    if (p_current_time_getter != nullptr) {
        timestamp = p_current_time_getter();
        period_elapsed = false;
    }
}

/*
 * @brief Reset the timer, set 0 to all ticks and false to flag
 */
template<typename T, T (*time_getter)(void), void (*callback)(void *)>
SoftwareTimer<T, time_getter, callback>::reset(void) {
    period_elapsed = false;
    timestamp = 0;
    period = 0;
}

/*
 * @brief Check if timer period is elapsed
 */
template<typename T, T (*time_getter)(void), void (*callback)(void *)>
bool SoftwareTimer<T>::check(void) {
    if (p_current_time_getter != nullptr && is_enabled_status) {
        period_elapsed = (p_current_time_getter() - timestamp) >= period;
        return period_elapsed;
    }
    return false;
}

/*
 * @brief Get is enabled timer state
 */
template<typename T, T (*time_getter)(void), void (*callback)(void *)>
bool SoftwareTimer<T>::is_enabled(void) {
    return is_enabled_status;
}

/*
 * @brief Get is timer period elapsed
 */
template<typename T, T (*time_getter)(void), void (*callback)(void *)>
bool SoftwareTimer<T>::is_period_elapsed(void) {
    return period_elapsed;
}

} /* namespace SWtimer */
