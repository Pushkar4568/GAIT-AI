#include "stm32f4xx.h"
#include "adc.h"

#define VREF      3.3f
#define ADC_RES   12
#define ADC_MAX   ((1 << ADC_RES) - 1)
#define V_MID     (VREF / 2.0f)   // Midpoint voltage 1.65V

// Read EMG signal in millivolts (positive-only)
float EMG_read(void) {
    // Start ADC conversion
    start_conversion_adc();
    uint16_t adc_val = (uint16_t)adc_read();

    // Convert ADC value to voltage
    float v_adc = ((float)adc_val / ADC_MAX) * VREF;

    // Remove DC offset and take absolute value
    float emg_signal = v_adc - V_MID;
    if (emg_signal < 0) emg_signal = -emg_signal;

    // Convert to millivolts
    float emg_mv = emg_signal * 1000.0f;

    return emg_mv;
}
