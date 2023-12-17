#include <stdio.h>

#include <zephyr/shell/shell.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

// LEDのGPIO設定を取得する
#if !DT_NODE_EXISTS(DT_NODELABEL(board_led))
#error "led0 node not properly defined."
#endif
static const struct gpio_dt_spec board_led =
	GPIO_DT_SPEC_GET_OR(DT_NODELABEL(board_led), gpios, {0});

// LEDの状態を変更するコマンド
static int cmd_led_toggle(const struct shell *shell, size_t argc, char **argv)
{
    static bool toggle = false;
    static bool initialized = false;
    int err;

    // 初回(initializedがfalse)の場合はGPIO OUTの初期化処理を行う
    if(!initialized) { 
        if (!gpio_is_ready_dt(&board_led)) {
            printf("The load switch pin GPIO port is not ready.\n");
            return 0;
        }

        printf("Initializing pin with inactive level.\n");

        err = gpio_pin_configure_dt(&board_led, GPIO_OUTPUT_INACTIVE);
        if (err != 0) {
            printf("Configuring GPIO pin failed: %d\n", err);
            return 0;
        }

        toggle = false;
        err = gpio_pin_set_dt(&board_led, toggle);
        if (err != 0) {
            printf("Setting GPIO pin level failed: %d\n", err);
        }
        initialized = true;
    } else {
        toggle = !toggle;
    }

    // LEDの状態を変更する
    printf("Toggling LED to %s\n", toggle ? "on" : "off");
    err = gpio_pin_set_dt(&board_led, toggle);
    if (err != 0) {
        printf("Setting GPIO pin level failed: %d\n", err);
    }

    return 0;
}

// コマンド登録
SHELL_CMD_REGISTER(toggle,NULL, "Toggle LED", cmd_led_toggle);

int main(void)
{
    printf("Hello Fuckin World!\n");
	return 0;
}
