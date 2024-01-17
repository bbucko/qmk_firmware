/* Copyright 2023 @ Keychron (https://www.keychron.com)
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

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "deferred_exec.h"
#include "dip_switch.h"
#include "transport.h"
#include "factory_test.h"

#define KC_MUTE_WEBEX LSG(KC_M)

#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h"
#endif

#ifdef OS_DETECTION_DEBUG_ENABLE
enum custom_keycodes {
    STORE_SETUPS = SAFE_RANGE,
    PRINT_SETUPS,
};
#endif

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

enum custom_keycodes {
    MUTE_WEBEX = SAFE_RANGE,
    TG_WIN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_SNAP,  KC_DEL,   RGB_TOG,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[MAC_FN] = LAYOUT_ansi_84(
     TG_WIN,  KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______,  RGB_MOD,
     _______, BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     RGB_TOG, RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     _______, RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
     _______,           _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
     _______, _______,  _______,                                KC_MUTE_WEBEX,                          _______,  _______,  _______,  _______,  _______,  _______),

[WIN_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_PSCR,  KC_DEL,   RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN_FN] = LAYOUT_ansi_84(
     TG_WIN,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______,  RGB_TOG,
     _______, BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     RGB_TOG, RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     _______, RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
     _______,           _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
     _______, _______,  _______,                                KC_MUTE_WEBEX,                          _______,  _______,  _______,  _______,  _______,  _______),
};


#ifdef TAP_DANCE_ENABLE
tap_dance_action_t tap_dance_actions[] = { };
#endif

// clang-format on
uint32_t get_host_os(uint32_t trigger_time, void *cb_arg) {
#ifdef OS_DETECTION_ENABLE
    if (get_transport() == TRANSPORT_USB) {
        switch (detected_host_os()) {
            case OS_WINDOWS:
#    ifdef CONSOLE_ENABLE
                uprintf("OS_WIN, switching to WIN_BASE");
#    endif
                set_single_persistent_default_layer(WIN_BASE);
                default_layer_set(1UL << WIN_BASE);
                break;
            case OS_MACOS:
            case OS_IOS:
#    ifdef CONSOLE_ENABLE
                uprintf("OS_MAC, switching to MAC_BASE");
#    endif
                set_single_persistent_default_layer(MAC_BASE);
                default_layer_set(1UL << MAC_BASE);
                break;
            case OS_UNSURE:
            case OS_LINUX:
            default:
#    ifdef CONSOLE_ENABLE
                uprintf("OS_UNSURE");
#    endif
                break;
        }
    }
#endif
    return 0;
}

// clang-format on
void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
    debug_enable   = true;
    debug_keyboard = true;
#endif

    // Set default layer based on the detected OS after a 1000 ms delay.
    defer_exec(500, get_host_os, NULL);
}

// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    if (!process_record_keychron_common(keycode, record)) {
        // Skip all further processing of this key
        return false;
    }

    switch (keycode) {
        // case MUTE_WEBEX:
            // Shift-Apple-M
            // SEND_STRING(SS_LSFT(SS_LCMMD(("m")));
            // return false;
        case TG_WIN:
            if (IS_LAYER_OFF(WIN_FN)) {
                layer_on(WIN_BASE);
                set_single_persistent_default_layer(WIN_BASE);
            } else if (IS_LAYER_ON(WIN_FN)) {
                layer_off(WIN_BASE);
                set_single_persistent_default_layer(MAC_BASE);
            }

            return false;
#ifdef OS_DETECTION_DEBUG_ENABLE
        case STORE_SETUPS:
            if (record->event.pressed) {
                store_setups_in_eeprom();
            }
            return false;
        case PRINT_SETUPS:
            if (record->event.pressed) {
                print_stored_setups();
            }
            return false;
#endif
    }

    // Process all other keycodes normally
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case MAC_BASE:
            // rgblight_setrgb(0x00, 0x00, 0xFF);
            break;
        case WIN_BASE:
            // rgblight_setrgb(0xFF, 0x00, 0x00);
            break;
        default: //  for any other layers, or the default layer
            // rgblight_setrgb(0x00, 0xFF, 0xFF);
            break;
    }
    return state;
}

// clang-format on
bool dip_switch_update_user(uint8_t index, bool active) {
    if (!process_dip_switch_factory_test(index, active)) {
        return false;
    }

#ifdef CONSOLE_ENABLE
    uprintf("dip_switch_update_user: index: %u :: active: %d :: keychron:  %d", index, active, keychon_dip_switch);
#endif

    if (get_transport() == TRANSPORT_USB) {
        switch (detected_host_os()) {
            case OS_WINDOWS:
            case OS_LINUX:
                default_layer_set(1UL << WIN_BASE);
                break;
            case OS_MACOS:
            case OS_IOS:
                default_layer_set(1UL << MAC_BASE);
                break;
            case OS_UNSURE:
                default_layer_set(1UL << (active ? WIN_BASE : MAC_BASE));
                break;
        }
    }
    return true;
}
