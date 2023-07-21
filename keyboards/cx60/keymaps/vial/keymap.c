/*
Copyright 2021 Horns Lyn <hornslyn@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H

typedef union {
  uint32_t raw;
  struct {
    bool     caps_indicator :1;
  };
} user_config_t;

user_config_t user_config;

enum custom_keycodes {
	CX_TOG = QK_KB_0
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0: Base Layer */
    LAYOUT_all(
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_DEL,
        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RALT, MO(1), KC_LEFT, KC_DOWN, KC_RGHT),

    /* 1: Function Layer */
    LAYOUT_all(
        QK_BOOT,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        BL_TOGG, BL_STEP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_HUI, RGB_VAI, RGB_HUD,
        RGB_SAI, RGB_SAD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_TOG, RGB_VAD, RGB_MOD)
};

/* rgb underglow capslock indicator; by Dominic Lee/MYMKB; */
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS( {0, 14, HSV_RED} );
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST( my_capslock_layer );

bool led_update_user(led_t led_state) {
	if (user_config.caps_indicator) {
		rgblight_set_layer_state(0, led_state.caps_lock);
	};
	return true;
}

void eeconfig_init_user(void) {
	user_config.raw = 0;
	user_config.caps_indicator = 1;
	eeconfig_update_user(user_config.raw);
}

void keyboard_post_init_user(void) {
	/* Enable the LED layers */
	rgblight_layers = my_rgb_layers;

	user_config.raw = eeconfig_read_user();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case CX_TOG:
			if (record->event.pressed) {
				// Toggle whether the caps indicator should be enabled.
				user_config.caps_indicator ^= 1;
				// Save indicator toggle status.
				eeconfig_update_user(user_config.raw);
				// Force update check.
				led_update_user(host_keyboard_led_state());
			}
			return false;
		default:
			return true;
	}
}
/* rgb underglow capslock indicator; end; */
