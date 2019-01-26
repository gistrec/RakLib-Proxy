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


// #include "packets/Packets.c"

/**
 * Порт, который используют клиенты для подключения
 */
#define EXTERNAL_PORT 8080

#define MAX_SERVERS

#define BUFFER_SIZE 1000

#include "Utils.c"

#include "PacketParser.c"
#include "NetworkManager.c"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Frost");
MODULE_DESCRIPTION("A simple UDP proxy for linux");
MODULE_VERSION("0.0.1");


static int __init initModule(void);
static void __exit exitModule(void);


module_init(initModule);
module_exit(exitModule);