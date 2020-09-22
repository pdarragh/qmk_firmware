RGB_MATRIX_EFFECT(layer1)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool layer1(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (int i = led_min; i <= led_max; ++i) {
        RGB_MATRIX_TEST_LED_FLAGS();
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER ||
            HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW))) {
            rgb_matrix_set_color(i, RGB_PINK);
        } else {
            rgb_matrix_set_color(i, RGB_WHITE);
        }
    }
    return led_max < DRIVER_LED_TOTAL;
}

#endif
