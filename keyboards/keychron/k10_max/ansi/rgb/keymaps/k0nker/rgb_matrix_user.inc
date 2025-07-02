// !!! DO NOT ADD #pragma once !!! //

/* Copyright 2024 ~ 2025 @ k0nker (https:// github.com/k0nker)
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
// This file contains the custom RGB matrix effect for the Keychron K10 Max ANSI RGB keyboard.
// It implements a ripple effect that changes color based on the current layer and allows for multiple ripples to be active at once.
// The effect is defined using the RGB_MATRIX_EFFECT macro and implemented in the `bz_ripple` function.

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(bz_ripple)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
extern int8_t  ripple_row;
extern int8_t  ripple_col;
extern uint8_t ripple_frame;
#    include "ripples.h"

#    define MAX_MANHATTAN_DIST (MATRIX_ROWS + MATRIX_COLS)

static bool bz_ripple(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t v = rgb_matrix_config.hsv.v;

    const uint8_t max_radius     = 16;
    const uint8_t fade_window    = 16;
    uint8_t       ripple_color_r = 1, ripple_color_g = 0, ripple_color_b = 128;

    static uint8_t frame_skip = 0;
    frame_skip                = (frame_skip + 1) % 16; // Only advance ripples every 16 frames

    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
        // Main gradient curve
        float t = (float)col / (MATRIX_COLS - 1);
        // Hold first color for the first % of columns
        if (t < 0.5f)
            t = 0.0f;
        else
            t = (t - 0.5f) / 0.5f; // Remap [0.4,1.0] to [0.0,1.0]

        // Ripple gradient curve
        float tr = (float)col / (MATRIX_COLS - 1);
        // Exponential ease-in for right side: lower number means right dominates most of the board
        tr             = powf(tr, 0.9f); // Range is 0.1-0.9
        uint8_t grad_r = 150;
        uint8_t grad_g = 12;
        uint8_t grad_b = 80;
        switch (get_highest_layer(layer_state | default_layer_state)) {
            case 0:
            case 1:
                grad_r         = (uint8_t)(20 + (0 - 20) * t);
                grad_g         = (uint8_t)(0 + (207 - 0) * t);
                grad_b         = (uint8_t)(63 + (166 - 63) * t);
                ripple_color_r = (uint8_t)(180 + (200 - 180) * tr);
                ripple_color_g = (uint8_t)(10 + (0 - 10) * tr);
                ripple_color_b = (uint8_t)(0 + (128 - 0) * tr);
                break;
            case 2:
            case 3:
                grad_r         = (uint8_t)(255 + (0 - 255) * t);
                grad_g         = (uint8_t)(255 + (207 - 255) * t);
                grad_b         = 0;
                ripple_color_r = (uint8_t)(0 + (128 - 0) * tr);
                ripple_color_g = (uint8_t)(64 + (0 - 64) * tr);
                ripple_color_b = (uint8_t)(255 + (128 - 255) * tr);
                break;
            default:
                break;
        }
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            uint8_t index = g_led_config.matrix_co[row][col];
            if (index >= led_min && index < led_max && index != NO_LED) {
                uint16_t r = grad_r, g = grad_g, b = grad_b;

                // For each ripple, blend if this LED is affected
                for (int i = 0; i < MAX_RIPPLES; i++) {
                    if (ripples[i].active) {
                        uint8_t dist = abs(ripples[i].row - row) + abs(ripples[i].col - col);
                        if (dist == ripples[i].frame) {
                            // At the ripple front: full ripple color
                            r = ripple_color_r;
                            g = ripple_color_g;
                            b = ripple_color_b;
                        } else if (dist < ripples[i].frame && (ripples[i].frame - dist) < fade_window) {
                            // Behind the ripple front: fade out
                            uint8_t fade = 255 - ((ripples[i].frame - dist) * (255 / fade_window));
                            r            = (ripple_color_r * fade + r * (255 - fade)) / 255;
                            g            = (ripple_color_g * fade + g * (255 - fade)) / 255;
                            b            = (ripple_color_b * fade + b * (255 - fade)) / 255;
                        }
                        // Remove the "ahead of the ripple" fade for a sharper front
                    }
                }
                r = (r * v) / 255;
                g = (g * v) / 255;
                b = (b * v) / 255;
                rgb_matrix_set_color(index, r, g, b);
            }
        }
    }

    // Advance all ripples, but only every other frame
    if (frame_skip == 0) {
        for (int i = 0; i < MAX_RIPPLES; i++) {
            if (ripples[i].active) {
                bool still_affecting = false;
                for (uint8_t col = 0; col < MATRIX_COLS && !still_affecting; ++col) {
                    for (uint8_t row = 0; row < MATRIX_ROWS && !still_affecting; ++row) {
                        uint8_t dist = abs(ripples[i].row - row) + abs(ripples[i].col - col);
                        if (dist <= ripples[i].frame && (ripples[i].frame - dist) < fade_window) {
                            still_affecting = true;
                        }
                    }
                }
                // Deactivate if not affecting any key, or if frame is out of bounds
                if ((ripples[i].frame < max_radius + fade_window && still_affecting) && (ripples[i].frame < MAX_MANHATTAN_DIST + fade_window)) {
                    ripples[i].frame++;
                } else {
                    ripples[i].active = false;
                }
            }
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
