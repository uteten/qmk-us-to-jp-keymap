# qmk-us-to-jp-keymap
qmk_firmwareでUS配列のキーボードをJISキーボードとして利用するためのコードです

## KeyMappingEntry 変換テーブル

USキーボードのキーをJPキーボードさせた際に、以下の変換を行います

| USキー    | Shiftなしの動作 | Shiftありの動作    | 備考                      |
| ------- | ---------- | ------------- | ------------------------- |
| `2`     | 変換不要       | `[`           | Shiftを解除する必要がある   |
| `6`     | 変換不要       | `=`           | Shiftを解除する必要がある   |
| `7`     | 変換不要       | Shift + `6`   |                          |
| `8`     | 変換不要       | Shift + `'`   |                          |
| `9`     | 変換不要       | Shift + `(`   |                          |
| `0`     | 変換不要       | Shift + `)`   |                          |
| `-`     | 変換不要       | Shift + 国際キー1 |                          |
| `;`     | 変換不要       | `'`           | Shiftを解除する必要がある   |
| `=`     | `-`           | Shift + `;`   |                          |
| `[`     | `]`           | Shift + `]`   |                          |
| `]`     | `\`           | Shift + `\`   |                          |
| `\`     | 国際キー3      | Shift + 国際キー3 |                       |
| `'`     | Shift + `7`   | Shift + `2`   |                          |
| `` ` `` | `[`           | Shift + `=`   |                          |

### KeyMappingEntry (KC_コード表記)

以下は、KC_コードの対応表です。

| USキー      | Shiftなしの動作      | Shiftありの動作       |
| ----------- | -------------------- | -------------------- |
| `KC_2`      | `KC_NO`              | `KC_LBRC`            |
| `KC_6`      | `KC_NO`              | `KC_EQL`             |
| `KC_7`      | `KC_NO`              | `KC_6` (Shift)       |
| `KC_8`      | `KC_NO`              | `KC_QUOT` (Shift)    |
| `KC_9`      | `KC_NO`              | `KC_8` (Shift)       |
| `KC_0`      | `KC_NO`              | `KC_9` (Shift)       |
| `KC_MINS`   | `KC_NO`              | `KC_INT1` (Shift)    |
| `KC_SCLN`   | `KC_NO`              | `KC_QUOT`            |
| `KC_EQL`    | `KC_MINS`            | `KC_SCLN` (Shift)    |
| `KC_LBRC`   | `KC_RBRC`            | `KC_RBRC` (Shift)    |
| `KC_RBRC`   | `KC_BSLS`            | `KC_BSLS` (Shift)    |
| `KC_BSLS`   | `KC_INT3`            | `KC_INT3` (Shift)    |
| `KC_QUOT`   | `KC_7` (Shift)       | `KC_2` (Shift)       |
| `KC_GRV`    | `KC_LBRC` (Shift)    | `KC_EQL` (Shift)     |

## 使用方法

keymap.cの中で`#include "qmk_us_to_jp_keymap.c"`して  
以下の行を `process_record_user` 関数の先頭に追記します:
   ```c
   if (!convert_us_to_jp_keymap(keycode,record->event.pressed)) return false;
   ```
   
## 例: 追記後のコード

```c
#include "qmk_us_to_jp_keymap.c"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!convert_us_to_jp_keymap(keycode,record->event.pressed)) return false; // 変換実行時は処理を終了

    // 他のカスタム処理をここに記述
    if (record->event.pressed) {
        switch (keycode) {
            // カスタムキーコードの処理
        }
    }

    return true; // 他の処理を継続
}
```
