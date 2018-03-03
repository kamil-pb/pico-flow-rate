# Fluid Pump Flow Rate Measurement

Microcontroller code to measure flow rate of a fluid pump in mL/min.

The volume of the pump should be specified in the PUMP_VOLUME_ML constant. The pump is expected to have a digital Hall effect sensor present to capture rotations of the pump's motor. Each time the magnet is detected, the period of rotation is computed in milliseconds, and then used to compute the flow rate.

* flow rate (mL/min) = 60000.0 (ms/min) * PUMP_VOLUME_ML (mL) / period (ms)
