#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define SERVO_GPIO 18
#define PWM_RESOLUTION 14
#define PWM_PERIOD_US 20000
#define SERVO_MIN_US 900
#define SERVO_MAX_US 2100
#define ANGLE_MIN 15
#define ANGLE_MAX 165

static void servo_set_angle(int angle) {
    if (angle < ANGLE_MIN) angle = ANGLE_MIN;
    if (angle > ANGLE_MAX) angle = ANGLE_MAX;
    uint32_t pulse = SERVO_MIN_US +
        (uint32_t)angle * (SERVO_MAX_US - SERVO_MIN_US) / 180;
    uint32_t duty = (uint32_t)((uint64_t)pulse * (1U << PWM_RESOLUTION) / PWM_PERIOD_US);
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}

void app_main(void) {
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_14_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer));
    ledc_channel_config_t channel = {
        .gpio_num = SERVO_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel));

    servo_set_angle(90); // Start at center. Test without attached wiper first.
    setvbuf(stdin, NULL, _IONBF, 0);
    printf("READY: send an angle 15..165 followed by newline at 115200 baud\n");
    char buf[20];
    size_t n = 0;
    while (1) {
        int c = getchar(); // USB-UART console input; don't leave idf.py monitor attached.
        if (c == EOF) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }
        if (c == '\n' || c == '\r') {
            if (n) {
                buf[n] = '\0';
                char *end;
                long value = strtol(buf, &end, 10);
                if (*end == '\0' && value >= ANGLE_MIN && value <= ANGLE_MAX) {
                    servo_set_angle((int)value);
                }
                n = 0;
            }
        } else if (n < sizeof(buf) - 1 && c >= '0' && c <= '9') {
            buf[n++] = (char)c;
        } else {
            n = 0;
        }
    }
}