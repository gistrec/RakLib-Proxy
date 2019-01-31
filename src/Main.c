#include "Config.h"


static int __init initModule(void) {
    init_network(); // Инициализируем netpool и netfilter хук
    memset(clients, 0, sizeof(struct Client) * MAX_CLIENTS);

    // Добавляем сервер [Network Byte Order]
    add_server(htonl(0xc0a80064), htons(8888), true); // 192.168.0.100:8888

    pr_info("Module loaded!\n");
    return 0;
}

static void __exit exitModule(void) {
    nf_unregister_net_hook(&init_net, &nfho_in);

    pr_info("Module unloaded!\n");
}