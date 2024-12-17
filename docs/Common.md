## Common Input Events 


#### `InputEventType::ENABLED`
Fired after an input is enabled. 

#### `InputEventType::DISABLED`
Fired after an input is disabled.

#### `InputEventType::IDLE`
Fired after no event has been fired for the specified idle duration unless the input is disabled. By default this is 10 seconds.

## Common Methods

The following methods are available in all input classes.

### Setup

Setup methods are typically called from within `setup()` function but can be updated at runtime.

##### Setting a Callback for Free Functions

#### `void setCallback(CallbackFunction func)`

Sets the function to be called on the event being fired and takes a single argument of your function name.

Your function signature must match the input class. Eg for an [`EventButton`](docs/EventButton.md):
```cpp
  void onButtonEvent(InputEventType et, EventButton & eb) {
    ...
  }
```
and set the callback function as: `myButton.setCallback(onButtonEvent);`

##### Setting a Callback for Class Methods
**Important:** This method will be available only to boards that allow the use of `std::function` (ie `#include <functional>`) - you will additionally be offered an override of `setCallback` to easily set a class instance & method as the callback:

#### `void setCallback(T* instance, void (T::*method))`

As with the free function above, your method signature must match the input class. Eg for an [`EventButton`](docs/EventButton.md):
```cpp
  void Foo::onButtonEvent(InputEventType et, EventButton & eb) {
    ...
  }
```
and set the callback as: `myButton.setCallback(&foo, &Foo::onButtonEvent);`

Note: you can still pass a lambda to the free function `setCallback` if that is you preferred style:
`myButton.setCallback([&](EventButton &btn) { foo.onButtonEvent(btn); });`


#### `void enable(bool e = true);`
Enable or disable a function. Default is to enable, pass `false` to disable.

#### `void setIdleTimeout(unsigned int timeoutMs);`
Set the idle timeout in ms. Default is 10000 (10 seconds). 


#### `void setInputId(uint8_t id);`
Set the input identifier (not unique, default=0, not used internally). Used when multiple inputs share a common callback function such as a HAL.

#### `void setInputValue(uint8_t val);`
Set the input value (not unique, default=0, not used internally). This is intended to store an enum, be used to identify the purpose of a multi-function button.

#### `void blockEvent(InputEventType et)`
By default, all event types are fired, but this allows you to block an event to stop it from firing.

#### `void allowEvent(InputEventType et)`
Allow an event to fire that has previously been blocked.

#### `void blockAllEvents()`
Stop all events from firing - usually used in conjunction with `allowEvent()`

#### `void allowAllEvents()`
Clear all excluded event types (ie allow all events to fire).

#### `bool isEventAllowed(InputEventType et)`
Returns true if the event is not excluded.


### Loop

#### `void update()`
Each input *must* have its `update()` method called within `loop()`. This reads the state of the input & pin(s) and fires the appropriate event type.

### Status


#### `bool isEnabled();`
Returns `true` or `false`.

#### `bool isIdle();`
Returns `true` if no activity for  longer than `setIdleTimeout()` or `false` if not - irrespective of whether the idle event type has been fired.

#### `unsigned long msSinceLastEvent();`
Returns the number of ms since any event was fired for this input.

#### `uint8_t getInputId();`
Get the button identifier. If not set will return 0.

#### `uint8_t getInputValue();`
Get the input value.

