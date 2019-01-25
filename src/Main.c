#include "Config.h"

// CLIENT ip: 192.168.2.36
// MCPE server ip : 192.168.2.37
// PROXY server ip: 192.168.2.40


// MCPE server listen 8888 Port
// Proxy listen server on 8080 port
// Proxy listen client on 8000 port

// Client listen [Random] port


// Структура для регистрации функции в netfilter
static struct nf_hook_ops nfho_in;

static u16 client_port = 0;


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

    char* data;
    int data_len;

	u8 mac[ETH_ALEN];

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

    /* Пропускаем если порт назначения не соответствует */
    if (dport != 8080 && dport != 8000) return NF_ACCEPT;

	// skb->len показывает содержимое фрейма, т.е. ipv4 пакет с заголовком
	// Чтобы получить длину, нужно отнять от этого размер ipv4 header && udp header
    data = (char *)(skb->data + sizeof(struct iphdr) + sizeof(struct udphdr));
    data_len = skb->len - sizeof(struct iphdr) - sizeof(struct udphdr);

    printk("RECEIVE, 0x%1ph %pI4h:%d -> %pI4h:%d\n", &data[0],
    	   &saddr, sport, &daddr, dport);

    dump(data, data_len > 80 ? 80 : data_len);
    printk(KERN_CONT "\n"); // Перенос на новую строку

    // У двух пакетов есть адреса. Нужно их сменить
    // Server->Client  0x07  ID_OPEN_CONNECTION_REQUEST_2 - должен содержать адрес сервера
    // Client->Server  0x08  ID_OPEN_CONNECTION_REPLY_2   - должен содержать адрес клиента
    if (data[0] == 0x07) openConnectionRequest2(data);
    if (data[0] == 0x08) openConnectionReply2(data, client_port);

    // Сообщение пришло от клиента
    // Нужно отправить его серверу
    if (dport == 8000) {
    	// if (client_port == 0) client_port = sport;
    	client_port = sport;
    	mac[0] = 0x40; mac[1] = 0xe2; mac[2] = 0x30;
    	mac[3] = 0x0e; mac[4] = 0x00; mac[5] = 0x99;
    	memcpy(np.remote_mac, mac, ETH_ALEN);

    	send_packet(
    		htonl(ip_to_int("192.168.2.37")), // DST: Адрес сервера
    		htonl(ip_to_int("192.168.2.40")), // SRC: Адрес прокси
    		8888, // DST: Порт сервера
    		8080, // SRC: Порт прокси
    		data,
    		data_len
    	);
    }

    // Сообщение пришло от сервера
    // Нужно отправить его клиенту
    if (dport == 8080) {
       	mac[0] = 0x54; mac[1] = 0xb1; mac[2] = 0x21;
    	mac[3] = 0x84; mac[4] = 0x1e; mac[5] = 0x28;

    	memcpy(np.remote_mac, mac, ETH_ALEN);
    	send_packet(
    		htonl(ip_to_int("192.168.2.36")), // DST: Адрес клиента
    		htonl(ip_to_int("192.168.2.40")), // SRC: Адрес прокси
    		client_port, // DST: Порт клиента
    		8000,        // SRC: Порт прокси
    		data,
    		data_len
    	);
    }


	return NF_DROP;
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
