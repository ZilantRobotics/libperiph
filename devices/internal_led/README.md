# Internal LED

Internal LED is a an abstraction under a physical indication existed on the board.

At that moment a single and RGB LEDs are supported. In future multiple or address LEDs might be supported.

The following states are supported:

| System State      | RGB LED state     |
| ----------------- | ----------------- |
| Armed No Fix      | Solid Blue        |
| Armed 3D Fix      | Solid Green       |
| Disarmed No Fix   | Blinking Blue     |
| Disarmed 3D Fix   | Blinking Green    |
| Disarmed Warning  | Blinking Yellow   |
| Disarmed Error    | Blinking Red      |
| Initialization    | Blinking Red/Blue |


## Usage

You should define either `INTERNAL_LED_LEGACY=1` or `INTERNAL_LED_RGB=1`.

Call the following function periodically:

```c++
void internalLedSetState(InternalLedState_t inernal_led_state);
```
