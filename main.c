// Обязательная часть любого загружаемого модуля ядра
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h> /* for MODULE_*. module_* */
#include <linux/printk.h> /* for pr_* */
#include <linux/netfilter_ipv4.h>


#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/netfilter.h>
#include <net/ip.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Frost");
MODULE_DESCRIPTION("A simple UDP proxy for linux");
MODULE_VERSION("0.0.1");


// Структура для регистрации функции в netfilter
static struct nf_hook_ops nfho_in;

// Хук для входящих пакетов
static unsigned int hook_packet_in(void *priv,
	                               struct sk_buff *skb,
		                           const struct nf_hook_state *state)
{
	struct iphdr   *ip_header;  /* IP header struct */
	struct udphdr  *udp_header; /* UDP header struct */

	if (!skb) return NF_ACCEPT;

	ip_header = (struct iphdr *) skb_network_header(skb);

	if (ip_header->protocol == IPPROTO_UDP) {
		udp_header = (struct udphdr *)(skb_transport_header(skb) + ip_hdrlen(skb));
		if (udp_header) {
			pr_info("SRC: (%pI4):%d --> DST: (%pI4):%d\n",
					&ip_header->saddr,
					ntohs(udp_header->source),
					&ip_header->daddr,
					ntohs(udp_header->dest));
		}
	}

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


    return result;
}

static void __exit exitModule(void) {
	nf_unregister_net_hook(&init_net, &nfho_in);

    pr_info("Module unloaded!\n");
}

module_init(initModule);
module_exit(exitModule);