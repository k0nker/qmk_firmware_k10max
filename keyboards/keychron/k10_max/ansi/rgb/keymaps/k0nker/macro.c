#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "macro.h"

macro_state_t bz_macro = {0};

void macro_setup(macro_state_t *ma, uint16_t keycode, bool activate, uint8_t type, uint8_t row, uint8_t col) {
    bz_macro.active = false; // Deactivate any currently active macro
    if (activate) {
        ma->btn    = keycode;
        ma->type   = type;
        ma->step   = 0;
        ma->delay  = 0;
        ma->timer  = timer_read();
        ma->active = true;
        ma->row    = row;
        ma->col    = col;
    } else {
        ma->active = false;
        ma->btn    = 0;
        ma->step   = 0;
        ma->delay  = 0;
    }
}

void macro_end_step(macro_state_t *ma, uint16_t delay, bool reset) {
    ma->delay = delay;
    ma->timer = timer_read();
    ma->step++;
    if (reset) {
        if (ma->type == MACRO_ONCE) {
            ma->active = false; // Deactivate the macro after running once
        }
        ma->step = 0;
    }
}

bool macro_ready(macro_state_t *ma) {
    if (timer_elapsed(ma->timer) >= ma->delay && ma->active == true)
        return true;
    else
        return false;
}

/*
 * Macro runner functions for executing the macro steps. Each macro gets it's own function.
 * If a pause is needed, use `macro_end_step(ma, delay, reset)` to set the next step.
 * Do not add waits or sleeps in these functions, as it will block all other keyboard functions.
 * Don't forget to update the `macro_table` with the macro functions.
 */
void macro_BZ_Test(macro_state_t *ma) {
    switch (ma->step) {
        case 0:
            register_code(KC_LSFT);
            SEND_STRING(SS_TAP(X_T));
            unregister_code(KC_LSFT);
            SEND_STRING(SS_TAP(X_E));
            macro_end_step(ma, 1000, false);
            break;
        case 1:
            SEND_STRING("st!");
            macro_end_step(ma, 2000, false);
            break;
        case 2:
            SEND_STRING(" Wrap up!");
            macro_end_step(ma, 1000, false);
            break;
        case 3:
            switch (get_highest_layer(layer_state | default_layer_state)) {
                case 0:
                case 1:
                    SEND_STRING(SS_LGUI("a"));
                    break;
                case 2:
                case 3:
                    SEND_STRING(SS_LCTL("a"));
                    break;
                default:
                    SEND_STRING(SS_LGUI("a"));
                    break;
            }

            SEND_STRING(SS_TAP(X_DEL));
            macro_end_step(ma, 1000, false);
            break;
        default:
            macro_end_step(ma, 0, true);
            break;
    }
}

void macro_BZ_LOVE(macro_state_t *ma) {
    switch (ma->step) {
        case 0:
            SEND_STRING("love");
            macro_end_step(ma, 1000, false);
            break;
        case 1:
            SEND_STRING(" you");
            macro_end_step(ma, 1000, false);
            break;
        default:
            macro_end_step(ma, 0, true);
            break;
    }
}

void macro_BZ_MBTN1(macro_state_t *ma) {
    switch (ma->step) {
        case 0:
            SEND_STRING(SS_TAP(X_MS_BTN1));
            break;
        default:
            macro_end_step(ma, 0, true);
            break;
    }
}

void macro_BZ_MBTN2(macro_state_t *ma) {
    switch (ma->step) {
        case 0:
            SEND_STRING(SS_TAP(X_MS_BTN2));
            break;
        default:
            macro_end_step(ma, 0, true);
            break;
    }
}

void macro_BZ_EQLS(macro_state_t *ma) {
    switch (ma->step) {
        case 0:
            SEND_STRING(SS_TAP(X_EQUAL));
            break;
        default:
            macro_end_step(ma, 0, true);
            break;
    }
}

void macro_BZ_NO3(macro_state_t *ma) {
    switch (ma->step) {
        case 0:
            SEND_STRING(SS_TAP(X_3));
            break;
        default:
            macro_end_step(ma, 0, true);
            break;
    }
}

// clang-format off
const macro_table_entry_t macro_table[] = {
    {BZ_Test,       macro_BZ_Test   },
    {BZ_LOVE,       macro_BZ_LOVE   },
    {BZ_MBTN1,      macro_BZ_MBTN1  },
    {BZ_MBTN2,      macro_BZ_MBTN2  },
    {BZ_EQLS,       macro_BZ_EQLS   },
    {BZ_NO3,        macro_BZ_NO3    },
};

// clang-format on
const size_t macro_table_size = sizeof(macro_table) / sizeof(macro_table[0]);

void macro_runner(macro_state_t *ma) {
    if (macro_ready(ma)) {
        for (size_t i = 0; i < macro_table_size; ++i) {
            if (macro_table[i].keycode == ma->btn) {
                macro_table[i].handler(ma);
                return;
            }
        }
    }
}
