#include "config.h"


// Структура для регистрации функции в netfilter
static struct nf_hook_ops nfho_in;

/**
 * В функцию передаются входящие пакеты
 */
static unsigned int hook_packet_in(void *priv,
	                               struct sk_buff *skb,
		                           const struct nf_hook_state *state)
{
	struct iphdr   *ip_header;  /* IP header struct */
	struct udphdr  *udp_header; /* UDP header struct */

	u16 sport, dport;           /* Source and destination ports */
    u32 saddr, daddr;           /* Source and destination addresses */

	if (!skb) return NF_ACCEPT;

	ip_header  = ip_hdr(skb);   // Getting IP Header
    udp_header = udp_hdr(skb); // Getting UDP Header

    /* Skip if it's not UDP packet */
    if (ip_header->protocol != IPPROTO_UDP)
        return NF_ACCEPT;

    /* Convert network endianness to host endiannes */
    saddr = ntohl(ip_header->saddr);
    daddr = ntohl(ip_header->daddr);
    sport = ntohs(udp_header->source);
    dport = ntohs(udp_header->dest);

    //if (dport != EXTERNAL_PORT) return NF_ACCEPT;

    MyTestNetworkSend();


    printk("RECEIVE, print_udp: %pI4h:%d -> %pI4h:%d\n", &saddr,
                                                         sport,
                                                         &daddr,
                                                         dport);

	return NF_ACCEPT;
}

// __init это подсказка ядру, что функция используется только во время инициализации модуля,
// а значит, после инициализации модуля эту функцию можно выгрузить из памяти.
// Аналогично и для __exit
static int __init initModule(void) {
	int result;

	// Функция, в которую будут передаваться пакеты
	nfho_in.hook = hook_packet_in;
	// Определяет место перехвата пакетов. Нас интересуют входящие пакеты
	nfho_in.hooknum  = NF_INET_LOCAL_IN;
	nfho_in.pf       = PF_INET;
	nfho_in.priority = NF_IP_PRI_FIRST;

	// Регестрируем хук
	result = nf_register_net_hook(&init_net, &nfho_in);
	if (result) {
		pr_err("Не удалось зарегестрировать netfilter хук\n");
	}else {
		pr_info("Модуль загружен!\n");
	}

	init_network(); // Инициализируем netpool

    return result;
}

static void __exit exitModule(void) {
	nf_unregister_net_hook(&init_net, &nfho_in);

    pr_info("Module unloaded!\n");
}