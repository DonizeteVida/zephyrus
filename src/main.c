#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>

#include <zephyr/fs/fs.h>
#include <ff.h>

#include <lvgl.h>

static FATFS fat_fs;
static struct fs_mount_t fs = {
	.type = FS_FATFS,
	.fs_data = &fat_fs,
	.mnt_point = "/SD:",
	.storage_dev = "SD",
};

static void button_clicked_cb(lv_event_t *event)
{
	LOG_INF("Button event: %s", lv_event_code_get_name(lv_event_get_code(event)));
}

static void write_file()
{
	int ret;

	ret = fs_mount(&fs);
	if (ret < 0)
	{
		printk("Mount failed: %d\n", ret);
	}

	struct fs_file_t file;
	fs_file_t_init(&file);

	ret = fs_open(&file, "/SD:/hello.txt", FS_O_CREATE | FS_O_WRITE);
	if (ret < 0)
	{
		printk("Open failed: %d\n", ret);
	}

	const char *data = "Hello Zephyr!";
	ret = fs_write(&file, data, strlen(data));
	if (ret < 0)
	{
		printk("Write failed: %d\n", ret);
	}
	fs_close(&file);
}

int main(void)
{
	const struct device *display_dev;
	lv_obj_t *label;
	lv_obj_t *button;
	int ret;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev))
	{
		LOG_ERR("Device not ready");
		return 0;
	}

	write_file();

	button = lv_button_create(lv_screen_active());
	lv_obj_add_event_cb(button, button_clicked_cb, LV_EVENT_CLICKED, NULL);
	lv_obj_center(button);

	label = lv_label_create(button);
	lv_label_set_text(label, "Hello world!");

	lv_timer_handler();
	ret = display_blanking_off(display_dev);
	if (ret < 0 && ret != -ENOSYS)
	{
		LOG_ERR("Failed to turn blanking off (error %d)", ret);
		return 0;
	}

	while (1)
	{
		lv_timer_handler();
		k_msleep(5);
	}

	return 0;
}
