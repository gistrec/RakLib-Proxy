/**
 * Массив с клиентами
 */
Client clients[MAX_CLIENTS] = { 0 };

/**
 * Добавляем клиента по его адресу и порту
 * @param saddr - адрес клиента [Network endianness]
 * @param sport - порт  клиента [Network endianness]
 */
Client* add_client(u32 saddr, u16 sport) {
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) continue;
        // Добавляем данные о новом клиенте
        clients[i].active = true;

        clients[i].nendian_ip = saddr;
        clients[i].hendian_ip = ntohl(saddr);

        clients[i].nendian_sport = sport;
        clients[i].hendian_sport = ntohs(sport);

        clients[i].nendian_bport = htons(MIN_BIND_PORT + i);
        clients[i].hendian_bport = MIN_BIND_PORT + i;

        clients[i].server_id = get_main_server_id();

        return &(clients[i]);
    }
    return 0;
}

/**
 * Получаем клиента по его адресу и порту
 * @param saddr - адрес клиента [Network endianness]
 * @param sport - порт  клиента [Network endianness]
 */
Client* get_client(u32 saddr, u16 sport) {
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active &&               // Проверяем активность
            clients[i].nendian_ip == saddr &&  // Сравниваем адрес
            clients[i].nendian_sport == sport) // Сравниваем порт
        {
            return &(clients[i]);
        }
    }
    return 0;
}

/**
 * Получаем клиента по порту, который ему был назначен прокси
 * @param  sport назначенный порт [Network endianness]
 */
Client* get_client_by_port(u16 sport) {
    Client* client;
    client = &(clients[ntohs(sport) - MIN_BIND_PORT]);

    if (client->active) return client;
    else return 0;
}

/**
 * Функция вызывается при получении пакета от клиента
 * @param saddr - адрес клиента [Network endianness]
 * @param sport - порт  клиента [Network endianness]
 * @param data - данные UDP пакета
 * @param len  - размер данных
 */
void handle_client_packet(u32 saddr, u16 sport, char* data, int len) {
    Client* client; // Данные клиента
    Server* server; // Данные сервера
    // 1. Ищем клиента в массиве
    client = get_client(saddr, sport);

    // Если клиент не найден - то добавляем нового клиента
    if (client == 0) {
        pr_info("Создали клиента");
        client = add_client(saddr, sport);
    }
    // Получаем сервер, на котором играет клиент
    server = get_server_by_id(client->server_id);

    // Вызываем функцию, которая заменит адрес и порт в RakLib пакете
    setAddressInPacket(client, server, data, len);

    // Отправляем пакет на сервер
    send_packet(
        server->nendian_ip,   // Адрес назначения [Network endianness]
        server->hendian_port, // Порт назначения  [Host    endianness]
        htonl(PROXY_ADDRESS),   // Адрес отправителя [Network endianness]
        client->hendian_bport,  // Порт отправителя  [Host    endianness]
        data,
        len
    );
}