/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
 * Copyright 2024 ~ 2025 @ k0nker (https://github.com/k0nker)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// This file contains the main keymap for the Keychron K10 Max ANSI RGB keyboard.
// It also includes a custom macro system to handle toggle, hold, and run-once macros.
// References to the custom ripple effect are included, which allows for visual feedback on key presses.

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "ripples.h"
#include "macro.h"

#define LAYER_CYCLE_START 0
#define LAYER_CYCLE_END 4

int8_t   ripple_col           = -1;
int8_t   ripple_row           = -1;
uint8_t  ripple_frame         = 255;
ripple_t ripples[MAX_RIPPLES] = {0};
uint16_t r = 0, g = 0, b = 0;

uint16_t macro_pulse = 0;
uint16_t macro_ripple = 0;

// A table of the LED int number for each key in rgb index.
const uint16_t PROGMEM keymap_leds[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
};

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_108(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,     KC_SNAP,  BZ_LOVE,  BZ_Test,  BZ_NO3,   BZ_EQLS,  BZ_MBTN2, BZ_MBTN1,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,    KC_INS,   KC_HOME,  KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,    KC_DEL,   KC_END,   KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,                                   KC_P4,    KC_P5,    KC_P6,    KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,              KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KC_ROPTN, MO(MAC_FN), KC_RCTL,    KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT,  KC_PENT),

    [MAC_FN] = LAYOUT_ansi_108(
        _______,            KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,    _______,  _______,  RGB_TOG,  _______,  _______,  _______,  _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,                                  _______,  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,              _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,            _______,  _______),

    [WIN_BASE] = LAYOUT_ansi_108(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,     KC_PSCR,  KC_CTANA, _______,  BZ_NO3,   BZ_EQLS,  BZ_MBTN2, BZ_MBTN1,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,    KC_INS,   KC_HOME,  KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,    KC_DEL,   KC_END,   KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,                                   KC_P4,    KC_P5,    KC_P6,    KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,              KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LWIN,  KC_LALT,  KC_LCTL,                                KC_SPC,                                 KC_RALT,  KC_RWIN,  MO(WIN_FN), KC_RCTL,    KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT,  KC_PENT),

    [WIN_FN] = LAYOUT_ansi_108(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,    _______,  _______,  RGB_TOG,  _______,  _______,  _______,  KC_NO,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,                                  _______,  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,              _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,            _______,  _______)
};

// clang-format on
int get_led_number(uint16_t keycode) {
    // Iterate through all layers, rows, and columns to find the keycode
    for (uint8_t layer = 0; layer < sizeof(keymaps) / sizeof(keymaps[0]); layer++) {
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                if (pgm_read_word(&keymaps[layer][row][col]) == keycode) {
                    // Calculate the index for keymap_leds based on row and col
                    uint8_t idx = row * MATRIX_COLS + col - 1;
                    return pgm_read_word(&keymap_leds[idx]);
                }
            }
        }
    }
    return 1; // Return 1 if not found
}

void add_ripple(uint8_t row, uint8_t col) {
    for (int i = 0; i < MAX_RIPPLES; i++) {
        if (!ripples[i].active) {
            ripples[i].row    = row;
            ripples[i].col    = col;
            ripples[i].frame  = 0;
            ripples[i].active = true;
            break;
            if (i == MAX_RIPPLES - 1) {
                // If no inactive ripple found, replace the oldest one
                ripples[0].row   = row;
                ripples[0].col   = col;
                ripples[0].frame = 0;
            }
        }
    }
}

/*
 * This is where you can set what type of macro each keycode will run.
 * You can set it to toggle, hold, or run once by placing the keycode
 * below the appropriate case statement:
 * - RESERVE_TOGGLE: The macro will toggle on and off when the key is pressed.
 * - RESERVE_HOLD: The macro will run while the key is held down and stop when released.
 * - RESERVE_ONCE: The macro will run once when the key is pressed and will not repeat.
 */
bool process_record_user(uint16_t kc, keyrecord_t *rc) {
    if (rc->event.pressed) {
        add_ripple(rc->event.key.row, rc->event.key.col);
    }
    switch (kc) {
        // Put all TOGGLE macros here
        case RESERVE_TOGGLE:
        case BZ_MBTN1:
        case BZ_MBTN2:
        case BZ_EQLS:
        case BZ_NO3:
        case BZ_Test:
            if (rc->event.pressed) {
                if (bz_macro.active && bz_macro.btn == kc) {
                    macro_setup(&bz_macro, kc, false, MACRO_TOGGLE, rc->event.key.row, rc->event.key.col);
                } else {
                    macro_setup(&bz_macro, kc, true, MACRO_TOGGLE, rc->event.key.row, rc->event.key.col);
                }
            }
            return false;
        // Put all HOLD macros here
        case RESERVE_HOLD:
            if (rc->event.pressed) {
                macro_setup(&bz_macro, kc, true, MACRO_HOLD, rc->event.key.row, rc->event.key.col);
            } else {
                macro_setup(&bz_macro, kc, false, MACRO_HOLD, rc->event.key.row, rc->event.key.col);
            }
            return false;
        // Put all RUN ONCE macros here
        case RESERVE_ONCE:
        case BZ_LOVE:
            if (rc->event.pressed) {
                if (bz_macro.active && bz_macro.btn == kc) {
                    macro_setup(&bz_macro, kc, false, MACRO_ONCE, rc->event.key.row, rc->event.key.col);
                } else {
                    macro_setup(&bz_macro, kc, true, MACRO_ONCE, rc->event.key.row, rc->event.key.col);
                }
            }
            return false;
        default:
            break;
    }
    if (!process_record_keychron_common(kc, rc)) {
        return false;
    }

    return true;
}

void matrix_scan_user(void) {
    if (bz_macro.active && timer_elapsed(macro_pulse) >= 10) {
        macro_runner(&bz_macro);
        macro_pulse = timer_read();
        if (timer_elapsed(macro_ripple) >= 1500) {
            add_ripple(bz_macro.row, bz_macro.col);
            macro_ripple = timer_read();
        }
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t v = rgb_matrix_config.hsv.v;
    if (bz_macro.active) {
        rgb_matrix_set_color(get_led_number(bz_macro.btn), RGB_RED);
    }
    if (get_highest_layer(layer_state) > 0) {
        uint8_t layer = get_highest_layer(layer_state);

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED && keymap_key_to_keycode(layer, (keypos_t){col, row}) > KC_TRNS) {
                    r = (255 * v) / 255;
                    g = (0 * v) / 255;
                    b = (0 * v) / 255;

                    rgb_matrix_set_color(index, r, g, b);
                }
            }
        }
    }
    if (host_keyboard_led_state().caps_lock) {
        for (uint8_t i = led_min; i < led_max; i++) {
            r = (255 * v) / 255;
            g = (20 * v) / 255;
            b = (5 * v) / 255;
            rgb_matrix_set_color(i, r, g, b);
        }
    }
    return false;
}
