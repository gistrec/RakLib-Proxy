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
    sport = udp_header->source;  //   Network byte order
    dport = udp_header->dest;    //

    // skb->len показывает содержимое фрейма, т.е. ipv4 пакет с заголовком
    // Чтобы получить длину, нужно отнять от этого размер ipv4 header && udp header
    data     = (char *)(skb->data + sizeof(struct iphdr) + sizeof(struct udphdr));
    data_len = skb->len - sizeof(struct iphdr) - sizeof(struct udphdr);

    // Если пакет пришел на порты, которые назначаются клиентам для общения с сервером
    if (ntohs(dport) >= MIN_BIND_PORT && ntohs(dport) < (MIN_BIND_PORT + MAX_CLIENTS)) {
        // Debug Info
        pr_info("Пришло сообщение от сервера\n");
        pr_info("0x%1pn %pI4n:%d -> %pI4n:%d\n", &data[0], &saddr, ntohs(sport), &daddr, ntohs(dport));
        dump(data, data_len > 80 ? 80 : data_len);

        handle_server_packet(saddr, sport, dport, data, data_len);
        return NF_DROP;
    }else if (ntohs(dport) == EXTERNAL_PORT) {
        // Debug info
        pr_info("Пришло сообщение от клиента\n");
        pr_info("0x%1pn %pI4n:%d -> %pI4n:%d\n", &data[0], &saddr, ntohs(sport), &daddr, ntohs(dport));
        dump(data, data_len > 80 ? 80 : data_len);

        handle_client_packet(saddr, sport, data, data_len);
        return NF_DROP;
    }

    // У двух пакетов есть адреса. Нужно их сменить
    // Server->Client  0x07  ID_OPEN_CONNECTION_REQUEST_2 - должен содержать адрес сервера
    // Client->Server  0x08  ID_OPEN_CONNECTION_REPLY_2   - должен содержать адрес клиента
    // if (data[0] == 0x07) openConnectionRequest2(data);
    // if (data[0] == 0x08) openConnectionReply2(data, client_port);

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

    nfho_in.hook     = handle_packet;    // Функция, в которую будут передаваться пакеты
    nfho_in.hooknum  = NF_INET_LOCAL_IN; // Входящие пакеты
    nfho_in.pf       = PF_INET;
    nfho_in.priority = NF_IP_PRI_FIRST;

    nf_register_net_hook(&init_net, &nfho_in); // Регестрируем хук
}

/**
 * Функция отправляет UDP пакет
 * @param u32 dst_addr [Network byte order] - адрес назначения
 * @param u16 dst_port [Host    byte order] - порт назначения
 * @param u32 src_addr [Network byte order] - адрес отправителя
 * @param u16 src_port [Host    byte order] - порт отправителя
 * @param char* msg - набор байт
 * @param int len   - количество байт
 */
void send_packet(u32 dst_addr, u16 dst_port,
                 u32 src_addr, u16 src_port,
                 char* msg, int len)
{
    np.local_ip.ip = src_addr;
    np.local_port  = src_port;

    np.remote_ip.ip = dst_addr;
    np.remote_port  = dst_port;

    netpoll_setup(&np);

    // TODO: переделать на netpoll_send_skb
    netpoll_send_udp(&np, msg, len);
    printk("SEND, 0x%1ph %pI4n:%d -> %pI4n:%d\n\n", &msg[0],
           &src_addr, src_port, &dst_addr, dst_port);

    dump(msg, len > 80 ? 80 : len);
    printk(KERN_CONT "\n"); // Перенос на новую строку
}