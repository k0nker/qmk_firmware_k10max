#pragma once
#include <stdint.h>
#include <stdbool.h>

// Macro state struct
typedef struct {
    bool     active;
    uint8_t  type;
    uint8_t  step;
    uint16_t btn;
    uint16_t timer;
    uint16_t delay;
} macro_state_t;

// Macro handler function pointer type
typedef void (*macro_handler_t)(macro_state_t *ma);

// Macro table entry struct
typedef struct {
    uint16_t        keycode;
    macro_handler_t handler;
} macro_table_entry_t;

// Macro table and size
extern const macro_table_entry_t macro_table[];
extern const size_t macro_table_size;
extern macro_state_t bz_macro;

// Macro helpers
void macro_end_step(macro_state_t *ma, uint16_t delay, bool reset);
bool macro_ready(macro_state_t *ma);
void macro_runner(macro_state_t *ma);
void macro_setup(macro_state_t *ma, uint16_t keycode, bool activate, uint8_t type);

// Macro handler prototypes (optional, but helps with IDEs)
void macro_BZ_Test(macro_state_t *ma);
void macro_BZ_LOVE(macro_state_t *ma);
void macro_BZ_MBTN1(macro_state_t *ma);
void macro_BZ_MBTN2(macro_state_t *ma);
void macro_BZ_EQLS(macro_state_t *ma);
void macro_BZ_NO3(macro_state_t *ma);

enum custom_keycodes {
    RESERVE_HOLD = SAFE_RANGE,
    RESERVE_TOGGLE,
    RESERVE_ONCE,
    BZ_MBTN1,
    BZ_MBTN2,
    BZ_NO3,
    BZ_EQLS,
    BZ_Test,
    BZ_LOVE,
};

enum macro_types {
    MACRO_TOGGLE,
    MACRO_HOLD,
    MACRO_ONCE,
};
