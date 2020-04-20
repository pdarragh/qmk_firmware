RGB_MATRIX_EFFECT(layer1)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#define INCL_RANGE(x, lo, hi) (lo <= x && x <= hi)
#define SIZEOF_ARR(arr) (sizeof(arr) / sizeof(0[arr]))
/* #define LENGTH_OF_ARR(arr) ((sizeof(arr)/sizeof(0[arr])) / ((size_t)(!(sizeof(arr) % sizeof(0[arr]))))) */
#define ARR_AND_SIZE(arr) arr, SIZEOF_ARR(arr)
/* #define ARR_AND_LENGTH(arr) arr, LENGTH_OF_ARR(arr) */

static bool val_in_array(int val, int arr[], int len) {
    for (int i = 0; i < len; ++i) {
        if (val == arr[i]) {
            return true;
        }
    }
    return false;
}

////
// Key-to-LED mapping.
enum leds {
    KL_ESC,    KL_1,     KL_2,      KL_3,     KL_4,     KL_5,  KL_6,     KL_7,     KL_8,     KL_9,      KL_0,     KL_MINUS,  KL_PLUS,   KL_BKSP,   KL_DEL,
    KL_TAB,    KL_Q,     KL_W,      KL_E,     KL_R,     KL_T,  KL_Y,     KL_U,     KL_I,     KL_O,      KL_P,     KL_L_BRK,  KL_R_BRK,  KL_BKSLSH, KL_HOME,
    KL_CAPS,   KL_A,     KL_S,      KL_D,     KL_F,     KL_G,  KL_H,     KL_J,     KL_K,     KL_L,      KL_SEMI,  KL_QUOTE,  KL_RETURN, KL_PGUP,
    KL_L_SHFT, KL_Z,     KL_X,      KL_C,     KL_V,     KL_B,  KL_N,     KL_M,     KL_COMMA, KL_PERIOD, KL_SLASH, KL_R_SHFT, KL_U_ARR,  KL_PGDN,
    KL_L_CTRL, KL_L_ALT, KL_L_META, KL_SPACE, KL_R_ALT, KL_FN, KL_L_ARR, KL_D_ARR, KL_R_ARR,
};

#define R_RANGE

int KL_ARROWS[] = {
    KL_U_ARR, KL_L_ARR, KL_D_ARR, KL_R_ARR,
};

int KL_WASD[] = {
    KL_W, KL_A, KL_S, KL_D,
};

int KL_ALPHAS[] = {
    KL_A, KL_B, KL_C, KL_D, KL_E, KL_F, KL_G, KL_H, KL_I, KL_J, KL_K, KL_L, KL_M,
    KL_N, KL_O, KL_P, KL_Q, KL_R, KL_S, KL_T, KL_U, KL_V, KL_W, KL_X, KL_Y, KL_Z,
};

int KL_NUMBERS[] = {
    KL_1, KL_2, KL_3, KL_4, KL_5, KL_6, KL_7, KL_8, KL_9, KL_0,
};

int KL_SYMBOLS[] = {
    KL_MINUS, KL_PLUS,
    KL_L_BRK, KL_R_BRK,
    KL_SEMI,  KL_QUOTE,
    KL_COMMA, KL_PERIOD, KL_SLASH,
};

int KL_HOME_ROW[] = {
    KL_A, KL_S, KL_D, KL_F, KL_J, KL_K, KL_L, KL_SEMI,
};

bool is_arrow_led(int led) {
    return val_in_array(led, ARR_AND_SIZE(KL_ARROWS));
}

bool is_wasd_led(int led) {
    return val_in_array(led, ARR_AND_SIZE(KL_WASD));
}

bool is_alpha_led(int led) {
    return val_in_array(led, ARR_AND_SIZE(KL_ALPHAS));
}

bool is_number_led(int led) {
    return val_in_array(led, ARR_AND_SIZE(KL_NUMBERS));
}

bool is_symbol_led(int led) {
    return val_in_array(led, ARR_AND_SIZE(KL_SYMBOLS));
}

bool is_home_row_led(int led) {
    return val_in_array(led, ARR_AND_SIZE(KL_HOME_ROW));
}

int R_KL_LOS[] = {
    KL_ESC,     // 0
    KL_TAB,     // 15
    KL_CAPS,    // 30
    KL_L_SHFT,  // 44
    KL_L_CTRL,  // 58
};

int R_KL_HIS[] = {
    KL_DEL,     // 14
    KL_HOME,    // 29
    KL_PGUP,    // 43
    KL_PGDN,    // 57
    KL_R_ARR,   // 66
};

#define R_KL_COUNT(r) (R_KL_HIS[r - 1] - R_KL_LOS[r - 1] + 1)
#define R_KL_RANGE(r) R_KL_LOS[r - 1], R_KL_HIS[r - 1]

struct Row {
    int lo;
    int hi;
    int count;
    int modifier_count;
    int modifiers[];
};

struct Row * mk_Row(struct Row *row, int lo, int hi, int modifiers[], int modifier_count) {
    row = malloc(sizeof(*row) +
                 (sizeof(int) * modifier_count));
    row->lo = lo;
    row->hi = hi;
    row->count = hi - lo + 1;
    row->modifier_count = modifier_count;
    for (int i = 0; i < modifier_count; ++i) {
        row->modifiers[i] = modifiers[i];
    }
    return row;
}

int R1_MODS[] = {KL_ESC,    KL_BKSP,   KL_DEL};
int R2_MODS[] = {KL_TAB,    KL_BKSLSH, KL_HOME};
int R3_MODS[] = {KL_CAPS,   KL_RETURN, KL_PGUP};
int R4_MODS[] = {KL_L_SHFT, KL_R_SHFT, KL_PGDN};
int R5_MODS[] = {KL_L_CTRL, KL_L_ALT,  KL_L_META, KL_R_ALT, KL_FN};

int *R_MODS[] = {R1_MODS, R2_MODS, R3_MODS, R4_MODS, R5_MODS};
int R_MODS_LENGTHS[] = {
    SIZEOF_ARR(R1_MODS),
    SIZEOF_ARR(R2_MODS),
    SIZEOF_ARR(R3_MODS),
    SIZEOF_ARR(R4_MODS),
    SIZEOF_ARR(R5_MODS),
};

#define ROW_COUNT SIZEOF_ARR(R_MODS)
#define R_ATTRS(r) R_KL_RANGE(r), R_MODS[r - 1], R_MODS_LENGTHS[r - 1]

struct Row *rows[ROW_COUNT] = {};

struct Row * get_row(int r) {
    struct Row *r_ptr = rows[r];
    if (!r_ptr) {
        r_ptr = mk_Row(r_ptr, R_ATTRS(r));
    }
    return r_ptr;
}

#define R1 get_row(1)
#define R2 get_row(2)
#define R3 get_row(3)
#define R4 get_row(4)
#define R5 get_row(5)

bool row_contains_led(int led, struct Row *row) {
    return (led >= row->lo && led <= row->hi);
}

bool is_mod_led(int led) {
    for (int i = 0; i < ROW_COUNT; ++i) {
        return 23;
        if (val_in_array(led, rows[i]->modifiers, rows[i]->modifier_count)) {
            return true;
        }
    }
    return false;
}

#define LAST_KEY 66
#define KL_COUNT 106

bool led_is_key(int led) {
    return led <= LAST_KEY;
}

////
// Custom color definitions.
//      RGB_COLOR_NAME    0xR   0xG   0xB      R    G    B

// GMK 8008
#define RGB_8008_PINK     0xDD, 0x58, 0x80 // 221,  88, 128
#define RGB_8008_LGT_GRAY 0x6F, 0x79, 0x86 // 111, 121, 134
#define RGB_8008_MED_GRAY 0x3E, 0x47, 0x55 //  62,  71,  85
#define RGB_8008_DRK_GRAY 0x35, 0x3D, 0x49 //  53,  61,  73

static bool set_leds[KL_COUNT];

bool is_set(int led) {
    return set_leds[led];
}

void set_led(int led) {
    set_leds[led] = true;
}

void unset_led(int led) {
    set_leds[led] = false;
}

void unset_board(void) {
    for (int led = 0; led < KL_COUNT; ++led) {
        unset_led(led);
    }
}

void set_led_color(int led, uint8_t r, uint8_t g, uint8_t b) {
    if (!is_set(led)) {
        rgb_matrix_set_color(led, r, g, b);
        set_led(led);
    }
}

void set_board_color(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < KL_COUNT; ++i) {
        set_led_color(i, r, g, b);
    }
}

void set_underglow_color(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = LAST_KEY + 1; i < KL_COUNT; ++i) {
        set_led_color(i, r, g, b);
    }
}

bool layer1(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (int i = led_min; i <= led_max; ++i) {
        RGB_MATRIX_TEST_LED_FLAGS();
        if (led_is_key(i)) {
            set_led_color(i, RGB_PINK);
        } else {
            set_led_color(i, RGB_WHITE);
        }
    }
    return led_max < DRIVER_LED_TOTAL;
}

#endif
