#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x972f93ab, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x85bd1608, "__request_region" },
	{ 0xbdbfaccb, "kmalloc_caches" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xc4f0da12, "ktime_get_with_offset" },
	{ 0x1ed8b599, "__x86_indirect_thunk_r8" },
	{ 0xdf8a6fb0, "pci_free_irq_vectors" },
	{ 0x6e78a26, "pci_write_config_word" },
	{ 0x909f0446, "single_open" },
	{ 0x77358855, "iomem_resource" },
	{ 0xa0d6e418, "dma_set_mask" },
	{ 0x7e4ee596, "single_release" },
	{ 0x32abcc91, "usb_reset_endpoint" },
	{ 0x8a95d7b4, "pci_disable_device" },
	{ 0xc3b77c73, "i2c_transfer" },
	{ 0x20000329, "simple_strtoul" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0x47c87907, "seq_printf" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xcdf81186, "usb_kill_urb" },
	{ 0x2b289bac, "remove_proc_entry" },
	{ 0xc8ab5383, "device_destroy" },
	{ 0xa8acc3c3, "__register_chrdev" },
	{ 0xc5df7f3, "driver_for_each_device" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x5338cacb, "pci_release_regions" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x9fa7184a, "cancel_delayed_work_sync" },
	{ 0x409bcb62, "mutex_unlock" },
	{ 0x88935827, "dma_free_attrs" },
	{ 0x9b4bb1f6, "device_create_with_groups" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xc875c014, "seq_read" },
	{ 0xa52e8c96, "pv_ops" },
	{ 0x1c5877fe, "dma_set_coherent_mask" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xce3828b7, "dma_get_required_mask" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xc344b97a, "param_ops_charp" },
	{ 0x76767812, "pci_set_master" },
	{ 0x97934ecf, "del_timer_sync" },
	{ 0xef2336c7, "pci_alloc_irq_vectors_affinity" },
	{ 0x500b3c56, "_dev_warn" },
	{ 0xfb578fc5, "memset" },
	{ 0xdbdf6c92, "ioport_resource" },
	{ 0x1e1e140e, "ns_to_timespec64" },
	{ 0x545a574c, "pci_iounmap" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0xa6a28e4e, "current_task" },
	{ 0x37befc70, "jiffies_to_msecs" },
	{ 0xbc5c3077, "usb_deregister" },
	{ 0x977f511b, "__mutex_init" },
	{ 0xc5850110, "printk" },
	{ 0xfef216eb, "_raw_spin_trylock" },
	{ 0x1a6636a6, "sysfs_remove_file_from_group" },
	{ 0x449ad0a7, "memcmp" },
	{ 0x9ec6ca96, "ktime_get_real_ts64" },
	{ 0x6ea4ef03, "class_unregister" },
	{ 0xde80cd09, "ioremap" },
	{ 0x1edb69d6, "ktime_get_raw_ts64" },
	{ 0x495830a1, "usb_set_interface" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x9166fada, "strncpy" },
	{ 0xf92fa91e, "usb_control_msg" },
	{ 0xeb87feef, "pci_read_config_word" },
	{ 0x593c1bac, "__x86_indirect_thunk_rbx" },
	{ 0x1cff402f, "dma_alloc_attrs" },
	{ 0x2ab7989d, "mutex_lock" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xd4c32249, "__class_register" },
	{ 0xaaf4a0b2, "_dev_err" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x4c71357b, "pci_find_capability" },
	{ 0x167c5967, "print_hex_dump" },
	{ 0xb27538b5, "i2c_del_adapter" },
	{ 0x960eaa1b, "_dev_info" },
	{ 0x3f74a06d, "usb_submit_urb" },
	{ 0xb601be4c, "__x86_indirect_thunk_rdx" },
	{ 0x12a38747, "usleep_range" },
	{ 0xb2fcb56d, "queue_delayed_work_on" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0xe9f21a27, "usb_reset_device" },
	{ 0xaab9af8b, "usb_bulk_msg" },
	{ 0x1000e51, "schedule" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0xd1317a39, "usb_clear_halt" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x1035c7c2, "__release_region" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x3ac10ac6, "pci_unregister_driver" },
	{ 0xcc5005fe, "msleep_interruptible" },
	{ 0xdc80a8de, "kmem_cache_alloc_trace" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xe69d0648, "param_ops_byte" },
	{ 0xff074a12, "pci_irq_vector" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0xf6ebc03b, "net_ratelimit" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x1f193312, "seq_lseek" },
	{ 0x37a0cba, "kfree" },
	{ 0x69acdf38, "memcpy" },
	{ 0xe575c496, "pci_request_regions" },
	{ 0xbeb7ca2b, "param_array_ops" },
	{ 0xedc03953, "iounmap" },
	{ 0x80a6d0f7, "__pci_register_driver" },
	{ 0xa4bbb490, "usb_register_driver" },
	{ 0x92540fbf, "finish_wait" },
	{ 0xc916a194, "sysfs_add_file_to_group" },
	{ 0x1375f74c, "i2c_bit_add_bus" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xb0e602eb, "memmove" },
	{ 0x2bc3476e, "pci_iomap" },
	{ 0x4650e97f, "param_ops_ushort" },
	{ 0x226321ee, "proc_create" },
	{ 0x99cedd47, "usb_get_current_frame_number" },
	{ 0x5e515be6, "ktime_get_ts64" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0xd38ebd32, "pci_enable_device" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x132b578d, "param_ops_ulong" },
	{ 0x262c1ff3, "param_ops_uint" },
	{ 0x50cd3737, "usb_free_urb" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x672bb9fb, "usb_alloc_urb" },
	{ 0xc1514a3b, "free_irq" },
};

MODULE_INFO(depends, "i2c-algo-bit");

MODULE_ALIAS("pci:v0000001Cd00000001sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000003sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000004sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000005sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000006sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000007sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000008sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000002sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd0000000Asv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000009sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000010sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000013sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000014sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000017sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000018sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd00000019sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000001Cd0000001Asv*sd*bc*sc*i*");
MODULE_ALIAS("usb:v0C72p000Cd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0C72p000Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0C72p0012d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0C72p0011d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0C72p0013d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0C72p0014d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "C8CBB9327E02303B8B29780");
