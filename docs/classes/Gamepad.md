## Gamepad
The `Gamepad` class is a simple way to manage gamepad input in Astro. They are automatically tracked
behind the scenes and you need only index the four possible gamepads with 0-3. Most of the methods are very self
explanatory and they are all safe to use at any time regardless of whether or not the desired controller is currently
connected.

 + [stick_deadzone](#stick_deadzone-getter)
 + [stick_deadzone=](#stick_deadzone-setter)
 + [trigger_deadzone](#trigger_deadzone-getter)
 + [trigger_deadzone=](#trigger_deadzone-setter)
 + [controllers_connected](#controllers_connected)
 + [button_pressed](#button_pressed)
 + [button_released](#button_released)
 + [button](#button)
 + [left_stick_x](#left_stick_x)
 + [left_stick_y](#left_stick_y)
 + [right_stick_x](#right_stick_x)
 + [right_stick_y](#right_stick_y)
 + [left_trigger](#left_trigger)
 + [right_trigger](#right_trigger)
 + [rumble](#rumble)
 + [is_connected](#is_connected)
 + [name](#name)

The following values are provided to index buttons:

 + `BUTTON_A`
 + `BUTTON_B`
 + `BUTTON_X`
 + `BUTTON_Y`
 + `BUTTON_BACK`
 + `BUTTON_GUIDE`
 + `BUTTON_START`
 + `BUTTON_LEFT_STICK`
 + `BUTTON_RIGHT_STICK`
 + `BUTTON_LEFT_SHOULDER`
 + `BUTTON_RIGHT_SHOULDER`
 + `BUTTON_DPAD_UP`
 + `BUTTON_DPAD_DOWN`
 + `BUTTON_DPAD_LEFT`
 + `BUTTON_DPAD_RIGHT`

### stick_deadzone (Getter)
`static stick_deadzone`

Returns the current stick deadzone for all controllers and sticks. It is 0.1 by default.

### stick_deadzone= (Setter)
`static stick_deadzone=(dz)`

Parameters
 + `dz -> Num` Deadzone value from 0-1.

Sets the stick deadzone for all controllers and all sticks.

### trigger_deadzone (Getter)
`static trigger_deadzone`

Returns the current trigger deadzone for all controllers and triggers. It is 0.1 by default.

### trigger_deadzone= (Setter)
`static trigger_deadzone=(dz)`

Parameters
 + `dz -> Num` Deadzone value from 0-1.

Sets the trigger deadzone for all controllers and all triggers.

### controllers_connected
`static controllers_connected`

Returns the number of controllers currently connected.

### button_pressed
`static button_pressed(index, button)`

Parameters
 + `index -> Num` Controller index to check.
 + `button -> Num` Button to check, should be one of the `Gamepad.BUTTON_*` values.

Returns true if the specified button of the specified controller was just pressed. Returns false if the controller is not connected.

### button_released
`static button_released(index, button)`

Parameters
 + `index -> Num` Controller index to check.
 + `button -> Num` Button to check, should be one of the `Gamepad.BUTTON_*` values.

Returns true if the specified button of the specified controller was just released. Returns false if the controller is not connected.

### button
`static button(index, button)`

Parameters
 + `index -> Num` Controller index to check.
 + `button -> Num` Button to check, should be one of the `Gamepad.BUTTON_*` values.

Returns true if the specified button of the specified controller is currently held down. Returns false if the controller is not connected.

### left_stick_x
`static left_stick_x(index)`

Parameters
 + `index -> Num` Controller index to check.
 
Returns the value of the left stick horizontally from -1 to 1. Returns 0 if the controller is not connected or the stick is
below the deadzone.

### left_stick_y
`static left_stick_y(index)`

Parameters
 + `index -> Num` Controller index to check.
 
Returns the value of the left stick vertically from -1 to 1. Returns 0 if the controller is not connected or the stick is
below the deadzone.

### right_stick_x
`static right_stick_x(index)`

Parameters
 + `index -> Num` Controller index to check.

Returns the value of the right stick horizontally from -1 to 1. Returns 0 if the controller is not connected or the stick is
below the deadzone.

### right_stick_y
`static right_stick_y(index)`

Parameters
 + `index -> Num` Controller index to check.

Returns the value of the right stick vertically from -1 to 1. Returns 0 if the controller is not connected or the stick is
below the deadzone.

### left_trigger
`static left_trigger(index)`

Parameters
 + `index -> Num` Controller index to check.
 
Returns the value of the left trigger from 0 to 1. Returns 0 if the controller is not connected or the stick is
below the deadzone.

### right_trigger
`static right_trigger(index)`

Parameters
 + `index -> Num` Controller index to check.
 
Returns the value of the right trigger from 0 to 1. Returns 0 if the controller is not connected or the stick is
below the deadzone.

### rumble
`static rumble(index, intensity, ms)`

Parameters
 + `index -> Num` Controller index to check.
 + `intensity -> Num` Intensity of the rumble from 0 to 1.
 + `ms -> Num` Duration of the rumble in milliseconds.

Rumbles the specified controller. Does nothing if the controller does not support rumble.

### is_connected
`static is_connected(index)`

Parameters
 + `index -> Num` Controller index to check.

Returns true if the specified controller is connected.

### name
`static name(index)`

Parameters
 + `index -> Num` Controller index to check.
 
Returns a string name for the specified controller. If there is none connected in that index, it returns `"N/A"`.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)