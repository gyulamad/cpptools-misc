# Values.ini File Structure

This document describes the structure and properties for `.ini` files used with the `ValuesT` and `ValueT` classes in `Values.hpp` and `Value.hpp` respectively.

Each section in the `.ini` file represents a single `ValueT` object, and the key-value pairs within that section define its properties.

## Value Properties:

1.  **`name`**
    *   **Type**: `string`
    *   **Required/Optional**: Required (implicitly set by the section name in the `.ini` file).
    *   **Description**: The name of the value. This is used as the section header (e.g., `[Parameter1]`).

2.  **`value`**
    *   **Type**: `real` (defaults to `float`)
    *   **Required/Optional**: Optional, but highly recommended.
    *   **Default**: `0.0`
    *   **Description**: The current numerical value.

3.  **`lower`**
    *   **Type**: `real` (defaults to `float`)
    *   **Required/Optional**: Optional.
    *   **Default**: `-INFINITY`
    *   **Description**: The lower bound for the `value`.

4.  **`upper`**
    *   **Type**: `real` (defaults to `float`)
    *   **Required/Optional**: Optional.
    *   **Default**: `INFINITY`
    *   **Description**: The upper bound for the `value`.

5.  **`step`**
    *   **Type**: `real` (defaults to `float`)
    *   **Required/Optional**: Optional.
    *   **Default**: `NAN` (Not A Number)
    *   **Description**: If set to a positive number, the value will be discretized to multiples of `step` within the `lower` and `upper` bounds. If `NAN`, the value is continuous.

6.  **`constant`**
    *   **Type**: `bool`
    *   **Required/Optional**: Optional.
    *   **Default**: `false`
    *   **Description**: If `true`, the value cannot be changed after initialization.

7.  **`rounded`**
    *   **Type**: `bool`
    *   **Required/Optional**: Optional.
    *   **Default**: `false`
    *   **Description**: If `true`, the `value` will be rounded to the nearest whole number.

8.  **`clips`**
    *   **Type**: `bool`
    *   **Required/Optional**: Optional.
    *   **Default**: `false`
    *   **Description**: If `true`, the `value` will be clipped to stay within `lower` and `upper` bounds if it goes out of range. If `false` and the `value` goes out of bounds, it might throw an error (depending on `throws`).

9.  **`throws`**
    *   **Type**: `bool`
    *   **Required/Optional**: Optional.
    *   **Default**: `true`
    *   **Description**: If `true`, an error will be thrown if the `value` goes out of bounds (and `clips` is `false`). If `false`, no error will be thrown in such a scenario.

## Example .ini file with all properties:

```ini
[MyParam1]
value = 10.5
lower = 0.0
upper = 100.0
step = 0.5
constant = false
rounded = true
clips = true
throws = true

[MyParam2]
value = 25.0
lower = 10.0
upper = 50.0
step = NAN
constant = false
rounded = false
clips = false
throws = true

[MyConstant]
value = 100
constant = true