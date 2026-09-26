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

template<typename T>
class SoftwareTimer {
public:
    SoftwareTimer(void) { };
    SoftwareTimer(T (*p_current_time_get) (void), T timer_period);
    void init(T (*p_current_time_get) (void), T timer_period);
    void start(void);
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
 * @brief Constructor with current time (ticks) getter
 * @param p_current_time_get Time (ticks) getter
 * @param timer_period Period of the timer
 */
template<typename T>
SoftwareTimer<T>::SoftwareTimer(
    T (*p_current_time_get) (void),
    T timer_period
):
p_current_time_getter{p_current_time_get},
period{timer_period} {
    // nope
}

/*
 * @brief Init to call instead of constructor with parameters
 */
template<typename T>
void SoftwareTimer<T>::init(
    T (*p_current_time_get) (void),
    T timer_period
) {
    p_current_time_getter = p_current_time_get;
    period = timer_period;
}

/*
 * @brief Start timer
 */
template<typename T>
void SoftwareTimer<T>::start(void) {
    if (p_current_time_getter != nullptr) {
        timestamp = p_current_time_getter();
        period_elapsed = false;
        is_enabled_status = true;
    }
}

/*
 * @brief Stop timer
 */
template<typename T>
void SoftwareTimer<T>::stop(void) {
    is_enabled_status = false;
}

/*
 * @brief Update timer ticks and flag
 */
template<typename T>
void SoftwareTimer<T>::update(void) {
    if (p_current_time_getter != nullptr) {
        timestamp = p_current_time_getter();
        period_elapsed = false;
    }
}

/*
 * @brief Reset the timer, set 0 to all ticks and false to flag
 */
template<typename T>
void SoftwareTimer<T>::reset(void) {
    period_elapsed = false;
    timestamp = 0;
    period = 0;
}

/*
 * @brief Check if timer period is elapsed
 */
template<typename T>
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
template<typename T>
bool SoftwareTimer<T>::is_enabled(void) {
    return is_enabled_status;
}

/*
 * @brief Get is timer period elapsed
 */
template<typename T>
bool SoftwareTimer<T>::is_period_elapsed(void) {
    return period_elapsed;
}
