#include <stdio.h>

#include <zephyr/shell/shell.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

// LEDのGPIO設定を取得する
#if !DT_NODE_EXISTS(DT_NODELABEL(board_led))
#error "led0 node not properly defined."
#endif
// 拡張したGPIOの設定を取得する
#if !DT_NODE_EXISTS(DT_NODELABEL(board_gpio))
#error "extended_gpio node not properly defined."
#endif

// デバイスツリーのGPIO設定を取得する
const struct gpio_dt_spec gpio_dt_specs[] = {
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(board_led), gpios, {0}),
    GPIO_DT_SPEC_GET_BY_IDX_OR(DT_NODELABEL(board_gpio), gpios, 0, {0}),
    GPIO_DT_SPEC_GET_BY_IDX_OR(DT_NODELABEL(board_gpio), gpios, 1, {0})
};

// GPIOを初期化する
static int initialize_gpio(const struct gpio_dt_spec *spec, gpio_flags_t extra_flags)
{
    int err;

    if (!gpio_is_ready_dt(spec)) {
        printf("The load switch pin GPIO port is not ready.\n");
        return -1;
    }

    printf("Initializing pin with inactive level.\n");

    err = gpio_pin_configure_dt(spec, extra_flags);
    if (err != 0) {
        printf("Configuring GPIO pin failed: %d\n", err);
        return -1;
    }

    return 0;
}

// すべてのGPIOを初期化する
static int initialize_all_gpios(void)
{
    int err;

    for(int i = 0; i < ARRAY_SIZE(gpio_dt_specs); i++) {
        err = initialize_gpio(&gpio_dt_specs[i], GPIO_OUTPUT_ACTIVE);
        if (err != 0) {
            printf("Initializing GPIO pin failed: %d\n", err);
            return err;
        }
    }

    return 0;
}

// GPIOの状態を変更するコマンド
static int cmd_gpio_toggle(const struct shell *shell, size_t argc, char **argv)
{
    long idx;
    int err;
    const struct gpio_dt_spec *spec;
    gpio_pin_t value;

    // コマンドライン引数をチェックする
    if (argc != 2) {
        printf("Usage: %s <gpio_idx>\n", argv[0]);
        return -1;
    }
    idx = strtol(argv[1], NULL, 10);
    if( idx < 0 || idx >= ARRAY_SIZE(gpio_dt_specs) ) {
        printf("Invalid GPIO index: %ld\n", idx);
        return -1;
    }

    // コマンドラインのIDXから対応するGPIO設定を取得する
    spec = &gpio_dt_specs[idx];

    // GPIOの状態を変更する
    value = gpio_pin_get_dt(spec);
    if (value != 0 && value != 1) {
        printf("Getting GPIO pin level failed: %d\n", value);
        return -1;
    }

    value = !value;
    
    err = gpio_pin_set_dt(spec, value);
    if (err != 0) {
        printf("Setting GPIO pin level failed: %d\n", err);
        return -1;
    }

    printf("Toggling GPIO[%ld] to %s\n", idx, value ? "on" : "off");

    return 0;
}

// コマンド登録
SHELL_CMD_REGISTER(gpio, NULL, "Toggle GPIO", cmd_gpio_toggle);

// メイン関数
int main(void)
{
    int err;

    err = initialize_all_gpios();
    if( err != 0 ) {
        printf("GPIO initialization failed: %d\n", err);
        return -1;
    }

	return 0;
}