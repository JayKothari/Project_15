#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa467383a, "module_layout" },
	{ 0x8c6b435e, "cdev_del" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x6a8aaab2, "cdev_add" },
	{ 0x5fecfef8, "cdev_alloc" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x37ff4c06, "copy_from_user_overflow" },
	{ 0x33d169c9, "_copy_from_user" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x735a0bd5, "native_io_delay" },
	{ 0x1fedf0f4, "__request_region" },
	{ 0x50eedeb8, "printk" },
	{ 0x7c61340c, "__release_region" },
	{ 0xff7559e4, "ioport_resource" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "B5E2F26B97C70E896A30986");
