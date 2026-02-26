/*
 * climate_example.c
 * TRONLOOP — Kullanım Örneği
 *
 * main.c içinde nasıl kullanılır:
 */

#include "climate_control.h"

/* HAL nesneleri — CubeMX tarafından üretilir */
extern I2C_HandleTypeDef  hi2c1;
extern TIM_HandleTypeDef  htim2;

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_I2C1_Init();
    MX_TIM2_Init();

    PID_t          pid;
    ClimateState_t state = {0};

    /* 25°C hedefle başlat */
    Climate_Init(&pid, 25.0f, &hi2c1, &htim2);

    while (1) {
        /* Her 1 saniyede bir güncelle */
        Climate_Update(&pid, &state, &hi2c1, &htim2);

        if (state.sensor_error) {
            /* Hata işle — CAN üzerinden ana üniteye bildir */
        } else {
            /*
             * state.measured   → ölçülen sıcaklık
             * state.cold_pct   → soğuk valf açıklığı %
             * state.hot_pct    → sıcak valf açıklığı %
             * state.pid_output → [-100, +100]
             *
             * CAN frame'e koy, BeagleBone'a gönder
             */
        }

        /* Hedef sıcaklığı CAN'dan güncellemek için: */
        /* Climate_SetTarget(&pid, yeni_hedef); */

        HAL_Delay(1000);
    }
}
