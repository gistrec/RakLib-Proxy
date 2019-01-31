struct Client;

/**
 * Массив с серверами
 */
Server servers[MAX_SERVERS] = { 0 };

/**
 * Добавляем сервер в сободную ячейку
 * @param saddr - адрес сервера [Network endianness]
 * @param sport - порт  сервера [Network endianness]
 * @return serverId
 */
Server* add_server(u32 saddr, u16 sport, bool main) {
    int i;
    for (i = 0; i < MAX_SERVERS; i++) {
        if (servers[i].active) continue;

        servers[i].active = true;
        servers[i].main = main;

        // Добавляем данные о новом сервере
        servers[i].nendian_ip = saddr;
        servers[i].hendian_ip = ntohl(saddr);

        servers[i].nendian_port = sport;
        servers[i].hendian_port = ntohs(sport);

        return &(servers[i]);
    }
    return 0;
}

/**
 * Получаем сервер по его адресу и порту
 * @param saddr - адрес сервера [Network endianness]
 * @param sport - порт  сервера [Network endianness]
 */
Server* get_server(u32 saddr, u16 sport) {
    int i;
    for (i = 0; i < MAX_SERVERS; i++) {
        if (servers[i].active &&
            servers[i].nendian_ip   == saddr &&
            servers[i].nendian_port == sport)
        {
            return &(servers[i]);
        }
    }
    return 0;
}

/**
 * Получаем ID главного сервера
 * @return id
 */
u8 get_main_server_id(void) {
    int i;
    for (i = 0; i < MAX_SERVERS; i++) {
        if (servers[i].active && servers[i].main) {
            return i;
        }
    }
    return 0;
}

/**
 * Получаем объект сервера по его id
 * @param  id
 */
Server* get_server_by_id(u8 id) {
    return &(servers[id]);
}

/**
 * Пакет пришел с сервера
 * @param saddr - адрес сервера [Network endianness]
 * @param sport - порт  сервера [Network endianness]
 * @param dport - порт назначения [Network endianness]
 * @param data - данные в UDP пакете
 * @param len  - размер данных
 */
void handle_server_packet(u32 saddr, u16 sport, u16 dport, char* data, int len) {
    Server* server;
    Client* client;
    // Получаем сервер
    server = get_server(saddr, sport);
    if (server == 0) {
        pr_info("Сервер не найден\n");
        return; // Если сервера нет - то пакет подделан
    }

    // Получаем клиента
    client = get_client_by_port(dport);
    if (client == 0) {
        pr_info("Клиент не найден\n");
        return; // Если клиента нет - то пакет подделан
    }

    // TODO: RakLib Packets check

    // Отправляем пакет клиенту
    send_packet(
        client->nendian_ip,     // Адрес назначения [Network endianness]
        client->hendian_sport,  // Порт назначения  [Host endianness]
        htonl(PROXY_ADDRESS), // Адрес отправителя [Network endianness]
        EXTERNAL_PORT,        // Порт отправителя  [Host    endianness]
        data,
        len
    );
}