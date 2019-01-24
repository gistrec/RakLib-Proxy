
static struct netpoll np;

void init_network() {
    np.name = "LRNG";
    strlcpy(np.dev_name, "enp0s8", IFNAMSIZ);
    memset(np.remote_mac, 0xff, ETH_ALEN); // TODO!
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
                 char* msg, int len) {
    // htonl(ip_to_int("192.168.2.40"));
    np.local_ip.ip = local_ip;
    np.local_port  = local_port;

    np.remote_ip.ip = dst_addr
    np.remote_port  = dst_port;

    netpoll_setup(&np);

    netpoll_send_udp(&np, msg, len);
}