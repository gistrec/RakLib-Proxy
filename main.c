// Обязательная часть любого загружаемого модуля ядра
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter_ipv4.h> 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Frost");
MODULE_DESCRIPTION("A simple UDP proxy for linux");
MODULE_VERSION("0.01");

// Счетчик входящих пакетов
static unsigned int input_packets = 0;

// Структура для регистрации функции в netfilter
static struct nf_hook_ops nfho_in;

// Хук для входящих пакетов
unsigned int hook_packet_in(unsigned int hooknum, struct sk_buff *skb,
		const struct net_device *in, const struct net_device *out,
		int (*okfn)(struct sk_buff *)) {
	input_packets++;
	pr_info("Пришел входящий пакет. Всего пришло: %d", input_packets);
	return NF_ACCEPT;
}

// __init это подсказка ядру, что функция используется только во время инициализации модуля,
// а значит, после инициализации модуля эту функцию можно выгрузить из памяти.
// Аналогично и для __exit
static int __init init(void) {
	// Функция, в которую будут передаваться пакеты
	nfho_in.hook = hook_packet_in;
	// Определяет место перехвата пакетов. Нас интересуют входящие пакеты
	nfho_in.hooknum = NF_INET_LOCAL_IN;
	nfho_in.pf = PF_INET;
	nfho_in.priority = NF_IP_PRI_FIRST;
	// Регестрируем хук
	nf_register_hook(&nfho_in);

    pr_info("Module loaded!\n");
    return 0;
}

static void __exit exit(void) {
	nf_unregister_hook(&nfho_in);

    pr_info("Module unloaded!\n");
}

module_init(init);
module_exit(exit);