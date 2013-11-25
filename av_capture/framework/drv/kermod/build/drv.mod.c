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
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0x9c3cb32f, "struct_module" },
	{ 0x11672b7c, "cdev_del" },
	{ 0x756df332, "i2c_attach_client" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x72449206, "cdev_init" },
	{ 0x1a585df3, "davinci_dma_link_lch" },
	{ 0x4acb38cf, "davinci_free_dma" },
	{ 0x17a142df, "__copy_from_user" },
	{ 0x7fbb0bcd, "davinci_stop_dma" },
	{ 0x98082893, "__copy_to_user" },
	{ 0x7bd699cb, "i2c_detach_client" },
	{ 0x43dadca, "i2c_del_driver" },
	{ 0x8c788f31, "davinci_get_dma_params" },
	{ 0x4ba89b2e, "malloc_sizes" },
	{ 0xdfe41aa4, "i2c_transfer" },
	{ 0x146276a7, "davinci_start_dma" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x829871bc, "__down_interruptible_failed" },
	{ 0xdd132261, "printk" },
	{ 0x91fbf659, "wait_for_completion_interruptible" },
	{ 0x6cbf60b9, "i2c_register_driver" },
	{ 0xc204f2cf, "cdev_add" },
	{ 0xe521e876, "kmem_cache_alloc" },
	{ 0x381f6ad5, "init_waitqueue_head" },
	{ 0x3aed6c2, "davinci_set_dma_params" },
	{ 0x37a0cba, "kfree" },
	{ 0x1042cbb5, "__up_wakeup" },
	{ 0xc4922191, "complete" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xee0d8b27, "davinci_request_dma" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

