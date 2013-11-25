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
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x72449206, "cdev_init" },
	{ 0x4acb38cf, "davinci_free_dma" },
	{ 0x17a142df, "__copy_from_user" },
	{ 0x7fbb0bcd, "davinci_stop_dma" },
	{ 0x98082893, "__copy_to_user" },
	{ 0x4ba89b2e, "malloc_sizes" },
	{ 0x146276a7, "davinci_start_dma" },
	{ 0xf7802486, "__aeabi_uidivmod" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x829871bc, "__down_interruptible_failed" },
	{ 0xdd132261, "printk" },
	{ 0x91fbf659, "wait_for_completion_interruptible" },
	{ 0x2196324, "__aeabi_idiv" },
	{ 0xc204f2cf, "cdev_add" },
	{ 0xe521e876, "kmem_cache_alloc" },
	{ 0xbc10dd97, "__put_user_4" },
	{ 0xb794c614, "request_irq" },
	{ 0xa2cabd99, "wait_for_completion_interruptible_timeout" },
	{ 0x381f6ad5, "init_waitqueue_head" },
	{ 0x72270e35, "do_gettimeofday" },
	{ 0x3aed6c2, "davinci_set_dma_params" },
	{ 0x37a0cba, "kfree" },
	{ 0xead9ce7, "remap_pfn_range" },
	{ 0xb9acd3d9, "__put_user_2" },
	{ 0x1042cbb5, "__up_wakeup" },
	{ 0xc4922191, "complete" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x9388003b, "__down_trylock_failed" },
	{ 0xee0d8b27, "davinci_request_dma" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

