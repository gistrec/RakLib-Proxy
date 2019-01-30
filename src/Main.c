#include "Config.h"


static int __init initModule(void) {
	init_network(); // Инициализируем netpool и netfilter хук
    add_server("192.168.0.100", 8888); // Добавляем сервер

    printk("Module loaded!");
    return 0;
}

static void __exit exitModule(void) {
	nf_unregister_net_hook(&init_net, &nfho_in);

    printk("Module unloaded!");
}