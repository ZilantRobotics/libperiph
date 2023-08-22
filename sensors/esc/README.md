# T-motor ESC Flame/Alpha/Thunder drivers

## ESC Flame/Alpha usage

ESC Flame driver parses UART feedback from [T-motor flame series](https://uav-en.tmotor.com/html/UAV/Multirotor/ESC/flame/).

This driver is suitable for T-motor ESC Alpha and Flame series:
- [ALPHA Series](https://store.tmotor.com/category-59-b0-ALPHA+Series.html)
- [FLAME Series](https://store.tmotor.com/category-20-b0-FLAME+Series.html)

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/esc/alpha.png?raw=true" alt="drawing" width=350>

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/esc/flame.png?raw=true" alt="drawing" width=350>

Protocol description: https://wiki.paparazziuav.org/wiki/Alpha_esc_with_telemetry_output

Please, follow [flame.h](flame.h) for usage description.

## ESC Thunder usage

ESC Thunder driver parses UART feedback from [T-motor 300A 24S ESC Thunder](https://store.tmotor.com/goods-975-Thunder+300A+24S.html).

<img src="https://github.com/ZilantRobotics/libperiph/blob/docs/assets/sensors/esc/thunder.png?raw=true" alt="drawing" width=350>

Protocol description: https://m.xcopter.com/download/Thunder%20300A%20ESC%20Manual.pdf

Please, follow [thunder.h](thunder.h) for usage description.
