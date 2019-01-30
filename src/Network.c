// Структура для регистрации функции в netfilter
static struct nf_hook_ops nfho_in;

// Структура хранит адрес для отправки пакета с помощью netpoll
static struct netpoll np;


/**
 * В функцию передаются входящие пакеты
 */
static unsigned int handle_packet(void *priv,
                                  struct sk_buff *skb,
                                  const struct nf_hook_state *state)
{
    struct iphdr   *ip_header;  // Структура заголовка IP пакета
    struct udphdr  *udp_header; // Структура заголовка UDP пакета

    u16 sport, dport;   // Порт отправителя и порт получателя
    u32 saddr, daddr;   // Адрес отправителя и адрес получателя

    char* data;     // Данные в UDP пакете
    int   data_len; // Размер данных


    if (!skb) return NF_ACCEPT;

    ip_header  = ip_hdr(skb);  // Получаем заголовок IP пакета

    // Пропускаем обработку, если пакет не UDP
    if (ip_header->protocol != IPPROTO_UDP) {
        return NF_ACCEPT;
    }

    udp_header = udp_hdr(skb); // Получаем заголовок UDP пакета

    saddr = ip_header->saddr;    //
    daddr = ip_header->daddr;    // Все данные в
    sport = udp_header->source;  //   network byte order
    dport = udp_header->dest;    //

    // skb->len показывает содержимое фрейма, т.е. ipv4 пакет с заголовком
    // Чтобы получить длину, нужно отнять от этого размер ipv4 header && udp header
    data     = (char *)(skb->data + sizeof(struct iphdr) + sizeof(struct udphdr));
    data_len = skb->len - sizeof(struct iphdr) - sizeof(struct udphdr);


    if (dport > htonl(50000) && dport < htonl(60000)) {
        printk("Пришло сообщение от сервера\n");
        handle_server_packet(saddr, sport, dport, data, data_len);
        return NF_DROP;
    }else if (dport == hton(8080)) {
        printk("Пришло сообщение от клиента\n");
        handle_client_packet(saddr, sporеt, data, data_len);
        return NF_DROP;
    }


/*
    printk("0x%1pn %pI4n:%d -> %pI4n:%d\n", &data[0],
           &saddr, ntohs(sport), &daddr, ntohs(dport));
    dump(data, data_len > 80 ? 80 : data_len);
    printk(KERN_CONT "\n"); // Перенос на новую строку
*/



    // У двух пакетов есть адреса. Нужно их сменить
    // Server->Client  0x07  ID_OPEN_CONNECTION_REQUEST_2 - должен содержать адрес сервера
    // Client->Server  0x08  ID_OPEN_CONNECTION_REPLY_2   - должен содержать адрес клиента
    // if (data[0] == 0x07) openConnectionRequest2(data);
    // if (data[0] == 0x08) openConnectionReply2(data, client_port);

    // Сообщение пришло от клиента
    // Нужно отправить его серверу
    /* if (dport == 8000) {
        client_port = sport;

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
        send_packet(
            htonl(ip_to_int("192.168.2.36")), // DST: Адрес клиента
            htonl(ip_to_int("192.168.2.40")), // SRC: Адрес прокси
            client_port, // DST: Порт клиента
            8000,        // SRC: Порт прокси
            data,
            data_len
        );
    }*/


    return NF_ACCEPT;
}

/**
 * 1. Инициализируем структуру для отправки пакетов
 * 2. Создаем хук для входящих пакетов
 */
void init_network(void) {
    np.name = "LRNG";
    strlcpy(np.dev_name, "enp0s8", IFNAMSIZ);
    memset(np.remote_mac, 0xff, ETH_ALEN); // TODO: what MAC address

    nfho_in.hook = handle_packet; // Функция, в которую будут передаваться пакеты
    nfho_in.hooknum  = NF_INET_LOCAL_IN; // Входящие пакеты
    nfho_in.pf       = PF_INET;
    nfho_in.priority = NF_IP_PRI_FIRST;

    nf_register_net_hook(&init_net, &nfho_in); // Регестрируем хук
}

/**
 * Функция отправляет UDP пакет
 * @param u32 dst_addr [network byte order] - адрес назначения
 * @param u32 src_addr [network byte order] - адрес отправителя
 * @param u16 dst_port [host byte order] - порт назначения
 * @param u16 src_port [host byte order] - порт отправителя
 * @param char* msg - набор байт
 * @param int len   - количество байт
 */
void send_packet(u32 dst_addr, u32 src_addr,
                 u16 dst_port, u16 src_port,
                 char* msg, int len)
{
    np.local_ip.ip = src_addr;
    np.local_port  = src_port;

    np.remote_ip.ip = dst_addr;
    np.remote_port  = dst_port;

    netpoll_setup(&np);

    netpoll_send_udp(&np, msg, len);
    printk("SEND, 0x%1ph %pI4n:%d -> %pI4n:%d\n", &msg[0],
           &src_addr, src_port, &dst_addr, dst_port);

    dump(msg, len > 80 ? 80 : len);
    printk(KERN_CONT "\n"); // Перенос на новую строку
}