/**
 * @file us_to_jp_keymap.c
 * @brief US配列キーボードのキーコードをJP配列キーボードのキーコードに変換するためのマッピング処理を実装したファイルです。
 */

typedef struct {
    bool is_shift_required;
    uint16_t keycode;
} KeyMapping;

typedef struct {
    uint16_t source_key;
    KeyMapping target_mappings[2];
} KeyMappingEntry;

// Mapping table from US keyboard keys to JP keyboard keys
// Each entry specifies how a key behaves with or without shift pressed
const KeyMappingEntry us_to_jp_keymap[] = {
    {KC_2,    {{false, KC_NO},   {false, KC_LBRC}}},
    {KC_6,    {{false, KC_NO},   {false, KC_EQL}}},
    {KC_7,    {{false, KC_NO},   {true,  KC_6}}},
    {KC_8,    {{false, KC_NO},   {true,  KC_QUOT}}},
    {KC_9,    {{false, KC_NO},   {true,  KC_8}}},
    {KC_0,    {{false, KC_NO},   {true,  KC_9}}},
    {KC_MINS, {{false, KC_NO},   {true,  KC_INT1}}},
    {KC_SCLN, {{false, KC_NO},   {false, KC_QUOT}}},
    {KC_EQL,  {{true,  KC_MINS}, {true,  KC_SCLN}}},
    {KC_LBRC, {{false, KC_RBRC}, {true,  KC_RBRC}}},
    {KC_RBRC, {{false, KC_BSLS}, {true,  KC_BSLS}}},
    {KC_BSLS, {{false, KC_INT3}, {true,  KC_INT3}}},
    {KC_QUOT, {{true,  KC_7},    {true,  KC_2}}},
    {KC_GRV,  {{true,  KC_LBRC}, {true,  KC_EQL}}},
};


const int us_to_jp_keymap_count = sizeof(us_to_jp_keymap) / sizeof(us_to_jp_keymap[0]);

int find_key_mapping_index(uint16_t keycode) {
    for (int i = 0; i < us_to_jp_keymap_count; i++) {
        if (us_to_jp_keymap[i].source_key == keycode) {
            return i;
        }
    }
    return -1; // Mapping not found
}

uint16_t pending_unregister_key = 0;
uint16_t last_handled_keycode = 0;

uint16_t pending_unregister_shift_key = 0;
uint16_t pending_register_shift_key = 0;

void reset_pending_keys(uint16_t keycode, bool pressed) {
    if (keycode == pending_register_shift_key && !pressed) {
        pending_register_shift_key =0;
    }
    if (keycode == pending_unregister_shift_key && pressed) {
            pending_unregister_shift_key = 0;
    }
    if (keycode == last_handled_keycode && !pressed) {
        if (pending_unregister_key) {
            unregister_code(pending_unregister_key);
            pending_unregister_key = 0;
        }
        if (pending_unregister_shift_key) {
            unregister_code(pending_unregister_shift_key);
            pending_unregister_shift_key = 0;
        }
        if (pending_register_shift_key) {
            register_code(pending_register_shift_key);
            pending_register_shift_key = 0;
        }
    }
}

bool process_key_mapping(int mapping_index,uint16_t keycode, bool pressed) {
    if (!pressed || mapping_index < 0 || mapping_index >= us_to_jp_keymap_count) return true;

    uint16_t current_shift_key = 0;
    uint16_t mapped_keycode = KC_NO;
    if (keyboard_report->mods & MOD_BIT(KC_LSFT)) {
        current_shift_key = KC_LSFT;
    } else if (keyboard_report->mods & MOD_BIT(KC_RSFT)) {
        current_shift_key = KC_RSFT;
    }
    if (current_shift_key) {
        // Shift is pressed
        mapped_keycode = us_to_jp_keymap[mapping_index].target_mappings[1].keycode;
        if (mapped_keycode == KC_NO) {
            return true; // No mapping required
        } else if (us_to_jp_keymap[mapping_index].target_mappings[1].is_shift_required) {
            register_code(mapped_keycode);
            pending_unregister_key = mapped_keycode;
        } else {
            unregister_code(current_shift_key);
            register_code(mapped_keycode);
            pending_register_shift_key = current_shift_key;
            pending_unregister_key = mapped_keycode;
        }

    } else {
        // Shift is not pressed
        mapped_keycode = us_to_jp_keymap[mapping_index].target_mappings[0].keycode;
        if (mapped_keycode == KC_NO) {
            return true; // No mapping required
        } else if (us_to_jp_keymap[mapping_index].target_mappings[0].is_shift_required) {
            register_code(KC_LSFT);
            register_code(mapped_keycode);
            pending_unregister_key = mapped_keycode;
            pending_unregister_shift_key = KC_LSFT;
        } else {
            register_code(mapped_keycode);
            pending_unregister_key = mapped_keycode;
        }
    }
    last_handled_keycode = keycode;
    return false;
}

bool convert_us_to_jp_keymap(uint16_t keycode, bool pressed) {
    reset_pending_keys(keycode, pressed);
    int mapping_index = find_key_mapping_index(keycode);
    if (mapping_index >= 0) {
        return process_key_mapping(mapping_index, keycode, pressed);
    }
    return true;
}
