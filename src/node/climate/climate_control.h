#ifndef CLIMATE_CONTROL_H
#define CLIMATE_CONTROL_H

#include "stm32l4xx_hal.h"
#include <stdint.h>

/* ── TMP117 ──────────────────────────────────────────────────────────────── */
#define TMP117_ADDR_GND     (0x48 << 1)   /* ADD0 = GND */
#define TMP117_ADDR_VCC     (0x49 << 1)   /* ADD0 = VCC */
#define TMP117_ADDR_SDA     (0x4A << 1)   /* ADD0 = SDA */
#define TMP117_ADDR_SCL     (0x4B << 1)   /* ADD0 = SCL */
#define TMP117_REG_TEMP     0x00
#define TMP117_LSB          0.0078125f    /* °C / LSB */

/* ── Valf PWM ────────────────────────────────────────────────────────────── */
/* TIM2 CH1 = Soğuk Valf, TIM2 CH2 = Sıcak Valf                             */
/* PWM: 50Hz, duty 1000–2000 µs → valf kapalı–tam açık                      */
#define VALVE_PWM_MIN       1000u         /* µs — valf kapalı (%0)           */
#define VALVE_PWM_MAX       2000u         /* µs — valf tam açık (%100)       */
#define VALVE_MIN_PCT       5.0f          /* minimum açıklık — sıfır akış yok */

/* ── PID ─────────────────────────────────────────────────────────────────── */
#define PID_KP              15.0f
#define PID_KI              0.3f
#define PID_KD              2.0f
#define PID_DT              1.0f          /* s — kontrol döngüsü periyodu    */
#define PID_OUT_MIN        -100.0f        /* %100 soğuk                      */
#define PID_OUT_MAX         100.0f        /* %100 sıcak                      */
#define PID_INTEGRAL_CLAMP  50.0f         /* windup koruması                 */

/* ── Sıcaklık Güvenlik Sınırları ─────────────────────────────────────────── */
#define TEMP_MIN_C         -10.0f
#define TEMP_MAX_C          60.0f
#define TEMP_SENSOR_ERR    -999.0f        /* okuma hatası sentinel           */

/* ── Tipler ──────────────────────────────────────────────────────────────── */

typedef struct {
    float kp, ki, kd;
    float integral;
    float prev_error;
    float setpoint;         /* hedef sıcaklık (°C) */
} PID_t;

typedef struct {
    float   setpoint;       /* hedef sıcaklık (°C)  */
    float   measured;       /* ölçülen sıcaklık (°C) */
    float   pid_output;     /* PID çıkışı [-100, +100] */
    float   cold_pct;       /* soğuk valf açıklığı [0–100] */
    float   hot_pct;        /* sıcak valf açıklığı [0–100] */
    uint8_t sensor_error;   /* 1 = sensör okunamadı */
} ClimateState_t;

/* ── API ──────────────────────────────────────────────────────────────────── */

void    Climate_Init   (PID_t *pid, float setpoint,
                        I2C_HandleTypeDef *hi2c,
                        TIM_HandleTypeDef *htim);

void    Climate_Update (PID_t *pid, ClimateState_t *state,
                        I2C_HandleTypeDef *hi2c,
                        TIM_HandleTypeDef *htim);

void    Climate_SetTarget (PID_t *pid, float setpoint_c);

float   TMP117_ReadTemp   (I2C_HandleTypeDef *hi2c, uint8_t addr);

#endif /* CLIMATE_CONTROL_H */
