#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

#include <lvgl.h>

static void button_clicked_cb(lv_event_t * event)
{
	LOG_INF("Button event: %s", lv_event_code_get_name(lv_event_get_code(event)));
}

int main(void)
{
	const struct device *display_dev;
	lv_obj_t *label;
	lv_obj_t *button;
	int ret;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready");
		return 0;
	}

	button = lv_button_create(lv_screen_active());
	lv_obj_add_event_cb(button, button_clicked_cb, LV_EVENT_CLICKED, NULL);
	lv_obj_center(button);

    label = lv_label_create(button);
	lv_label_set_text(label, "Hello world!");

	lv_timer_handler();
	ret = display_blanking_off(display_dev);
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("Failed to turn blanking off (error %d)", ret);
		return 0;
	}

    while(1) {
        lv_timer_handler();
        k_msleep(5);
    }

	return 0;
}
