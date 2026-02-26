/*
 * climate_control.c
 * TRONLOOP — Node İklim Kontrol
 *
 * Hedef sıcaklığa ulaşmak için soğuk/sıcak hava karışımını
 * PID algoritmasıyla ayarlar.
 *
 * Platform : STM32L476 + HAL
 * Sensör   : TMP117 (I2C)
 * Valfler  : PWM servo valf × 2 (TIM2 CH1 = soğuk, CH2 = sıcak)
 *
 * Karışım mantığı:
 *   pid_output ∈ [-100, +100]
 *   cold_pct = clamp(50 - pid_output/2, MIN, 100)
 *   hot_pct  = clamp(50 + pid_output/2, MIN, 100)
 *
 *   pid_output =   0 → soğuk %50, sıcak %50 (nötr)
 *   pid_output = +100 → soğuk %0,  sıcak %100 (tam sıcak)
 *   pid_output = -100 → soğuk %100, sıcak %0  (tam soğuk)
 *
 * Faz 2 kapsamındadır. Faz 1'de bu modül derlenmez.
 */

#include "climate_control.h"
#include <math.h>

/* ── Yardımcı: clamp ─────────────────────────────────────────────────────── */
static inline float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* ── Yardımcı: valf yüzdesini TIM Compare değerine çevir ─────────────────── */
static uint32_t pct_to_compare(float pct)
{
    /* pct: 0.0 – 100.0
     * compare: VALVE_PWM_MIN – VALVE_PWM_MAX (µs) */
    float us = VALVE_PWM_MIN + (pct / 100.0f) * (VALVE_PWM_MAX - VALVE_PWM_MIN);
    return (uint32_t)us;
}

/* ── TMP117 Sıcaklık Okuma ───────────────────────────────────────────────── */
/*
 * TMP117 sıcaklık kaydı: 16-bit, işaretli, MSB önce
 * Formül: T(°C) = register_value × 0.0078125
 */
float TMP117_ReadTemp(I2C_HandleTypeDef *hi2c, uint8_t addr)
{
    uint8_t  buf[2];
    int16_t  raw;
    HAL_StatusTypeDef status;

    /* Sıcaklık kaydını oku */
    status = HAL_I2C_Mem_Read(hi2c, addr,
                              TMP117_REG_TEMP, I2C_MEMADD_SIZE_8BIT,
                              buf, 2, 10);

    if (status != HAL_OK) {
        return TEMP_SENSOR_ERR;   /* hata sentinel */
    }

    raw = (int16_t)((buf[0] << 8) | buf[1]);
    return (float)raw * TMP117_LSB;
}

/* ── PID Hesapla ─────────────────────────────────────────────────────────── */
static float PID_Compute(PID_t *pid, float measured)
{
    float error    = pid->setpoint - measured;
    float p_term   = pid->kp * error;

    /* İntegral — windup koruması */
    pid->integral += error * PID_DT;
    pid->integral  = clampf(pid->integral,
                            -PID_INTEGRAL_CLAMP,
                             PID_INTEGRAL_CLAMP);
    float i_term   = pid->ki * pid->integral;

    /* Türev — gürültü filtresi uygulanabilir */
    float d_term   = pid->kd * (error - pid->prev_error) / PID_DT;
    pid->prev_error = error;

    return clampf(p_term + i_term + d_term, PID_OUT_MIN, PID_OUT_MAX);
}

/* ── Valf Karışımını Ayarla ──────────────────────────────────────────────── */
/*
 * pid_out = +100 → tam sıcak  (soğuk kapalı, sıcak tam açık)
 * pid_out =    0 → %50 / %50  (nötr — VALVE_MIN_PCT altına düşmez)
 * pid_out = -100 → tam soğuk  (soğuk tam açık, sıcak kapalı)
 *
 * Toplam debi her zaman korunur: cold_pct + hot_pct ≈ sabit
 */
static void Valve_SetMix(TIM_HandleTypeDef *htim,
                         float pid_out,
                         float *cold_pct_out,
                         float *hot_pct_out)
{
    float cold_pct = clampf(50.0f - pid_out / 2.0f, VALVE_MIN_PCT, 100.0f);
    float hot_pct  = clampf(50.0f + pid_out / 2.0f, VALVE_MIN_PCT, 100.0f);

    /* TIM2 CH1 = soğuk valf */
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, pct_to_compare(cold_pct));
    /* TIM2 CH2 = sıcak valf */
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, pct_to_compare(hot_pct));

    if (cold_pct_out) *cold_pct_out = cold_pct;
    if (hot_pct_out)  *hot_pct_out  = hot_pct;
}

/* ── Güvenlik Kontrolü ───────────────────────────────────────────────────── */
static uint8_t Temp_IsSafe(float t)
{
    return (t > TEMP_MIN_C && t < TEMP_MAX_C);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Public API
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Climate_Init — PID yapısını başlat, PWM kanallarını çalıştır
 */
void Climate_Init(PID_t *pid, float setpoint,
                  I2C_HandleTypeDef *hi2c,
                  TIM_HandleTypeDef *htim)
{
    (void)hi2c;  /* şimdilik kullanılmıyor, ileride config için */

    pid->kp         = PID_KP;
    pid->ki         = PID_KI;
    pid->kd         = PID_KD;
    pid->integral   = 0.0f;
    pid->prev_error = 0.0f;
    pid->setpoint   = clampf(setpoint, TEMP_MIN_C, TEMP_MAX_C);

    /* PWM kanallarını başlat — nötr pozisyon */
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);

    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, pct_to_compare(50.0f));
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, pct_to_compare(50.0f));
}

/*
 * Climate_Update — Her ~1 saniyede bir çağrılır
 *
 * 1. TMP117'den sıcaklık oku
 * 2. PID hesapla
 * 3. Valf karışımını güncelle
 * 4. Durumu state yapısına yaz
 */
void Climate_Update(PID_t *pid, ClimateState_t *state,
                    I2C_HandleTypeDef *hi2c,
                    TIM_HandleTypeDef *htim)
{
    float temp = TMP117_ReadTemp(hi2c, TMP117_ADDR_GND);

    /* Sensör hatası — valfler nötr'de bırakılır */
    if (fabsf(temp - TEMP_SENSOR_ERR) < 0.001f || !Temp_IsSafe(temp)) {
        state->sensor_error = 1;
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, pct_to_compare(50.0f));
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, pct_to_compare(50.0f));
        return;
    }

    state->sensor_error = 0;
    state->measured     = temp;
    state->setpoint     = pid->setpoint;

    /* PID çıkışı hesapla */
    float pid_out = PID_Compute(pid, temp);
    state->pid_output = pid_out;

    /* Vafleri ayarla */
    Valve_SetMix(htim, pid_out, &state->cold_pct, &state->hot_pct);
}

/*
 * Climate_SetTarget — Hedef sıcaklığı güncelle, integrali sıfırla
 */
void Climate_SetTarget(PID_t *pid, float setpoint_c)
{
    pid->setpoint  = clampf(setpoint_c, TEMP_MIN_C, TEMP_MAX_C);
    pid->integral  = 0.0f;   /* hedef değişince windup'ı temizle */
    pid->prev_error = 0.0f;
}
