#include "pico/stdlib.h"

// Pin definitions
const uint LED1_PIN = 11;      // GPIO 11 for LED1
const uint LED2_PIN = 12;      // GPIO 12 for LED2
const uint BUTTON1_PIN = 15;   // GPIO 15 for Button 1
const uint BUTTON2_PIN = 14;   // GPIO 14 for Button 2
const uint USB_DETECT_PIN = 4; // GPIO 4 for USB-to-TTL detection

// Last known state of the USB connection
bool last_usb_state = false;

// Function to perform the action for LED
void perform_action(uint led_pin) {
    gpio_put(led_pin, 1); // Turn on the LED
    sleep_ms(2000);       // Keep the LED on for 2 seconds
    gpio_put(led_pin, 0); // Turn off the LED
}

int main() {
    // Initialize stdio (for debugging, if needed)
    stdio_init_all();

    // Initialize LEDs as output
    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_put(LED1_PIN, 0); // Turn off LED1 initially

    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_put(LED2_PIN, 0); // Turn off LED2 initially

    // Initialize buttons as input with pull-up resistors
    gpio_init(BUTTON1_PIN);
    gpio_set_dir(BUTTON1_PIN, GPIO_IN);
    gpio_pull_up(BUTTON1_PIN);

    gpio_init(BUTTON2_PIN);
    gpio_set_dir(BUTTON2_PIN, GPIO_IN);
    gpio_pull_up(BUTTON2_PIN);

    // Initialize USB detect pin as input with pull-down resistor
    gpio_init(USB_DETECT_PIN);
    gpio_set_dir(USB_DETECT_PIN, GPIO_IN);
    gpio_pull_down(USB_DETECT_PIN);

    while (true) {
        // Check for button 1 press
        if (gpio_get(BUTTON1_PIN) == 0) { // Button press is active low
            perform_action(LED1_PIN);
        }

        // Check for button 2 press
        if (gpio_get(BUTTON2_PIN) == 0) { // Button press is active low
            perform_action(LED2_PIN);
        }

        // Check USB detect pin state
        bool current_usb_state = gpio_get(USB_DETECT_PIN);
        if (current_usb_state != last_usb_state) { // State changed
            last_usb_state = current_usb_state; // Update the last known state

            if (current_usb_state) {
                // USB just connected
                perform_action(LED1_PIN);
            } else {
                // USB just disconnected
                perform_action(LED2_PIN);
            }
        }

        sleep_ms(10); // Small delay to reduce CPU usage
    }
}
