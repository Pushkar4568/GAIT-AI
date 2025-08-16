# GAIT-AI

IMU & EMG based GAIT Analysis

This project is aimed at analyzing human gait using IMU (Inertial Measurement Unit) sensors and EMG (Electromyography) sensors. It can be used for rehabilitation monitoring, physiotherapy, and movement analysis.

# Project Overview

IMU Sensors: Track motion and orientation of legs during walking or running.

EMG Sensors: Measure muscle activity in real-time to detect flexion and relaxation of muscles such as the biceps and quadriceps.

Microcontroller: STM32F4 series is used to read sensor data and process signals.

# EMG Sensor Work Done So Far

ADC Reading: Read the EMG analog signal via STM32 ADC.

DC Offset Removal: Shift EMG around 0 V to remove the sensor’s midpoint voltage (~1.65 V).

Positive-only EMG Signal: Take absolute value to work with rectified muscle activity.

Signal in Millivolts: Convert raw voltage to mV for easier interpretation.

Threshold for Flex Detection:

Unflexed: EMG < 150 mV

Flexed: EMG > 150 mV

# Realtime Testing: Verified resting EMG ~126 mV and flexed EMG ~433 mV for biceps.

# Code Structure

EMG.c → Reads raw ADC values from the EMG sensor and outputs EMG signal in mV.

EMG.h → Header file for EMG functions.

main.c → Initializes ADC and UART, reads EMG signal, and prints output for testing.

# Next Steps

Add smoothing/filtering to reduce spikes in EMG signal.

Implement auto-calibration for resting and flexed thresholds.

Integrate with IMU sensor data for full gait analysis.


# Hardware Used

STM32F4 microcontroller

EMG sensor

2x 9 V battery for EMG, 3.3 V for STM32

Electrodes placed on biceps/quadriceps

# How to Use

Connect EMG sensor output to STM32 ADC pin.

Power the sensor with 9 V and STM32 with 3.3 V.

Compile and upload the code to STM32.

Open serial monitor to see EMG readings in mV.

Flex and relax muscle to verify readings above/below 150 mV.
