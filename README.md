# Using the Temperature & Humidity Sensor in C/C++

## Objective

During the first step in this lab, you will compile a small **I<sup>2</sup>C** program using the Arduino APIs to an program using the UPM Grove LCD and Temperature & Humidity libraries.

You will write code in C/C++ and measure temperature in Celsius and relative humidity using the UPM library, convert it to Fahrenheit, then display it on the LCD.

## Setup the temperature & humidity sensor and LCD screen

![](./images/action.png) Connect **Grove Temperature & Humidity Sensor** to any **I<sup>2</sup>C** port of the Grove Base Shield.
</br>
![](./images/action.png) Connect the **LCD screen** to another **I<sup>2</sup>C** port.

## **I<sup>2</sup>C** using the Arduino API

### Write the Code to Read the Temperature & Humidity Sensor.

Update <span class="icon file">temperature</span> to read the temperature & humidity sensor on program start up and log it to the console.

*  Include the following headers in your C/C++ program

  	```c
    #include "th02.hpp"
    #include "upm_utilities.h"
    ```

*  Create a while loop that will continuously read the temperature value and humidity value from the sensor in Celsius and percent Relative Humidity. We will convert the temperature to Fahrenheit and then print out the values.

*  The code should look like this:

```c
#include "th02.hpp"
#include "upm_utilities.h"

int
main() {

  // Set the subplatform for the shield
  mraa_add_subplatform(MRAA_GROVEPI, "0");

  // Create the temperature & humidity sensor object
  upm::TH02 sensor;

  // Read the temperature and humidity printing both the Celsius and
  // equivalent Fahrenheit temperature and Relative Humidity, waiting two seconds between readings
  while (1) {
      float celsius = sensor.getTemperature();
      float fahrenheit = (celsius * 9.0 / 5.0 + 32.0);
      float humidity = sensor.getHumidity();
      printf("%2.3f Celsius, or %2.3f Fahrenheit\n", celsius, fahrenheit);
      printf("%2.3f%% Relative Humidity\n", humidity);

      upm_delay(2);
  }

  return 0;
}

## Build and Upload your program
When you compile and run your program, you should see the temperature value on your monitor via serial communication.

```
* Now let us add the LCD portion to display the information on your screen. Add the Grove LCD libary.

```c
#include "th02.hpp"
#include "upm_utilities.h"
#include "jhd1313m1.h"

```

* Before the while loop, let us initialize the LCD screen.

```c
// initialize the LCD and check for initialization
jhd1313m1_context lcd = jhd1313m1_init(0, 0x3e, 0x62);

if (!lcd) {
  std::cout << "jhd1313m1_i2c_init() failed" << std::endl;
  return 1;
}

// set the LCD parameters
char string1[20];
char string2[20];
uint8_t rgb[7][3] = {
          {0xd1, 0x00, 0x00},
          {0xff, 0x66, 0x22},
          {0xff, 0xda, 0x21},
          {0x33, 0xdd, 0x00},
          {0x11, 0x33, 0xcc},
          {0x22, 0x00, 0x66},
          {0x33, 0x00, 0x44}};

```
* In the while loop lets set the LCD screen to the data values that are being sent over serial.

```c


snprintf(string1, sizeof(string1), "Temperature:");
snprintf(string2, sizeof(string2), "%2.1f%cF %2.1f%cC", fahrenheit, 223, celsius, 223);
// Alternate rows on the LCD
jhd1313m1_set_cursor(lcd, 0, 0);
jhd1313m1_write(lcd, string1, strlen(string1));
jhd1313m1_set_cursor(lcd, 1, 0);
jhd1313m1_write(lcd, string2, strlen(string2));
// Change the color
uint8_t r = rgb[(int)fahrenheit%7][0];
uint8_t g = rgb[(int)fahrenheit%7][1];
uint8_t b = rgb[(int)fahrenheit%7][2];
jhd1313m1_set_color(lcd, r, g, b);
upm_delay(2);
jhd1313m1_clear(lcd);

snprintf(string1, sizeof(string1), "Humidity:");
snprintf(string2, sizeof(string2), "%2.1f%%", humidity);
// Alternate rows on the LCD
jhd1313m1_set_cursor(lcd, 0, 0);
jhd1313m1_write(lcd, string1, strlen(string1));
jhd1313m1_set_cursor(lcd, 1, 0);
jhd1313m1_write(lcd, string2, strlen(string2));
upm_delay(2);
jhd1313m1_clear(lcd);

```
## Build and Upload your program
When you compile and run your program, you should see the temperature value on your LCD in both Celsius and Fahrenheit. After 2 seconds relative humidity will be displayed.  If you put your finger on the sensor you can see the LCD screen change color.

## The Final program looks like this
```c
#include "th02.hpp"
#include "upm_utilities.h"
#include "jhd1313m1.h"

int
main() {

  // Set the subplatform for the shield
  mraa_add_subplatform(MRAA_GROVEPI, "0");

  // Create the temperature & humidity sensor object
  upm::TH02 sensor;

  // initialize the LCD and check for initialization
  jhd1313m1_context lcd = jhd1313m1_init(0, 0x3e, 0x62);

  if (!lcd) {
    printf("jhd1313m1_i2c_init() failed\n");
    return 1;
  }

  // set the LCD parameters
  char string1[20];
  char string2[20];
  uint8_t rgb[7][3] = {
            {0xd1, 0x00, 0x00},
            {0xff, 0x66, 0x22},
            {0xff, 0xda, 0x21},
            {0x33, 0xdd, 0x00},
            {0x11, 0x33, 0xcc},
            {0x22, 0x00, 0x66},
            {0x33, 0x00, 0x44}};

  // Read the temperature and humidity printing both the Celsius and
  // equivalent Fahrenheit temperature and Relative Humidity, waiting two seconds between readings
  while (1) {
      float celsius = sensor.getTemperature();
      float fahrenheit = (celsius * 9.0 / 5.0 + 32.0);
      float humidity = sensor.getHumidity();
      printf("%2.3f Celsius, or %2.3f Fahrenheit\n", celsius, fahrenheit);
      printf("%2.3f%% Relative Humidity\n", humidity);

      snprintf(string1, sizeof(string1), "Temperature:");
      snprintf(string2, sizeof(string2), "%2.1f%cF %2.1f%cC", fahrenheit, 223, celsius, 223);
      // Alternate rows on the LCD
      jhd1313m1_set_cursor(lcd, 0, 0);
      jhd1313m1_write(lcd, string1, strlen(string1));
      jhd1313m1_set_cursor(lcd, 1, 0);
      jhd1313m1_write(lcd, string2, strlen(string2));
      // Change the color
      uint8_t r = rgb[(int)fahrenheit%7][0];
      uint8_t g = rgb[(int)fahrenheit%7][1];
      uint8_t b = rgb[(int)fahrenheit%7][2];
      jhd1313m1_set_color(lcd, r, g, b);
      upm_delay(2);
      jhd1313m1_clear(lcd);

      snprintf(string1, sizeof(string1), "Humidity:");
      snprintf(string2, sizeof(string2), "%2.1f%%", humidity);
      // Alternate rows on the LCD
      jhd1313m1_set_cursor(lcd, 0, 0);
      jhd1313m1_write(lcd, string1, strlen(string1));
      jhd1313m1_set_cursor(lcd, 1, 0);
      jhd1313m1_write(lcd, string2, strlen(string2));
      upm_delay(2);
      jhd1313m1_clear(lcd);
  }

  return 0;
}
```

There are a number of additional examples available for reference as [how-to-code-samples](https://github.com/intel-iot-devkit/how-to-code-samples) on git hub

## Additional resources

Information, community forums, articles, tutorials and more can be found at the [Intel Developer Zone](https://software.intel.com/iot).

For reference code for any sensor/actuator from the Grove* IoT Commercial Developer Kit, visit [https://software.intel.com/en-us/iot/hardware/sensors](https://software.intel.com/en-us/iot/hardware/sensors)
