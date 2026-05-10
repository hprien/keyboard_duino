#include <Arduino.h>

#define keyboard_out_pin_n 6
static const uint8_t keyboard_out_pins[keyboard_out_pin_n] = {
    A0, A1, A2, A3, A4, A5
};

#define keyboard_in_pin_n 7
static const uint8_t keyboard_in_pins[keyboard_in_pin_n] = {
    5, 6, 7, 8, 9, 10, 11
};

typedef struct {
    uint8_t out_pin_index;
    uint8_t in_pin;
} Key;


void set_one_keyboard_out_pin(uint8_t pin_to_enable) {
    for(uint8_t pin_index = 0; pin_index < keyboard_out_pin_n; pin_index++) {
        uint8_t pin = keyboard_out_pins[pin_index];
        bool new_state = pin == pin_to_enable ? HIGH : LOW;
        digitalWrite(pin, new_state);
    }
}

uint8_t key_code(uint8_t key_group, uint8_t key_in_group) {
    static const uint8_t key_groups_n = keyboard_in_pin_n;
    uint8_t raw_key_code = key_group * key_groups_n + key_in_group;

    if(raw_key_code == 5) {
        return 0;
    }

    uint8_t linear_key_code = raw_key_code - key_group;
    linear_key_code -= 5;

    return linear_key_code;
}

void setup() {
    Serial.begin(115200);

    for(uint8_t pin_index = 0; pin_index < keyboard_out_pin_n; pin_index++) {
        pinMode(keyboard_out_pins[pin_index], OUTPUT);
    }

    for(uint8_t pin_index = 0; pin_index < keyboard_in_pin_n; pin_index++) {
        pinMode(keyboard_in_pins[pin_index], INPUT);
    }
}

void loop() {
    for(uint8_t key_in_group = 0; key_in_group < keyboard_out_pin_n; key_in_group++) {
        set_one_keyboard_out_pin(keyboard_out_pins[key_in_group]);

        for(uint8_t key_group = 0; key_group < keyboard_in_pin_n; key_group++) {
            bool key_state = digitalRead(keyboard_in_pins[key_group]);
            if(key_state) {
                Serial.print("key code: ");
                Serial.println(key_code(key_group, key_in_group));
                delay(200);
            }
        }
    }
}