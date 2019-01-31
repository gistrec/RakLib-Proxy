#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h> /* Для функций MODULE_*. module_* */
#include <linux/printk.h> /* Для функций pr_* */
#include <linux/netfilter_ipv4.h> /* Для констант NF_ */

#include <linux/ip.h>
#include <linux/tcp.h>
#include <net/ip.h>
#include <net/tcp.h>

#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/netfilter.h>
#include <net/ip.h>
#include <linux/netpoll.h>

// Адрес прокси [Host byte order]
#define PROXY_ADDRESS 0xc0a80069 // 192.168.0.105
// Порт, который используют клиенты для подключения
#define EXTERNAL_PORT 8080

#define MAX_SERVERS 10
#define MAX_CLIENTS 100

// Минимальный порт, который может назначен клиенту
// Максимальный порт: MIN_BIND_PORT + MAX_CLIENTS
#define MIN_BIND_PORT 1000

#define BUFFER_SIZE 2000

#include "Utils.c"
#include "packets/Packets.c"

#include "server/Server.h"
#include "client/Client.h"

#include "server/ServerManager.c"
#include "client/ClientManager.c"

#include "Network.c"


MODULE_LICENSE("GPL");
MODULE_VERSION("0.1.0");
MODULE_AUTHOR ("Alex Frost - gistrec@mail.ru");
MODULE_DESCRIPTION("RakLib proxy for linux");


static int  __init initModule(void);
static void __exit exitModule(void);

#ifndef TESTING
module_init(initModule);
module_exit(exitModule);
#endif