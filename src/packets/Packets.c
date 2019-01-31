// Отключаем выравнивание данных
#pragma pack(push, 1)

#include "OpenConnectionRequest2.c"
#include "OpenConnectionReply2.c"

#pragma pack(pop)


/**
 * Проверяем RakLib пакеты на необходимость замены адреса в них
 * @param client - указатель на структуру клиента
 * @param server - указатель на структуру сервера
 * @param data   - данные
 * @param len    - размер данных
 */
void setAddressInPacket(Client* client, Server* server, char* data, int len) {
    switch (data[0]) {
        case 0x07: // OPEN_CONNECTION_REQUEST_2
            // Пакет идет Client->Proxy->Server
            // Должен содержать в себе адрес сервера
            // Но содержит адрес прокси
            set_raklib_address(
                server->nendian_ip,
                (char *)&(((OpenConnectionRequest2 *) data)->ip)
            );
            set_raklib_port(
                server->nendian_port,
                (char *)&(((OpenConnectionRequest2 *) data)->port)
            );
            break;
        case 0x08: // OPEN_CONNECTION_REPLY_2
            // Пакет идет Server->Proxy->Client
            // Должен содержать в себе адрес клиента
            // Но содержит адрес прокси
            set_raklib_address(
                client->nendian_ip,
                (char *)&(((OpenConnectionReply2 *) data)->ip)
            );
            set_raklib_port(
                client->nendian_sport,
                (char *)&(((OpenConnectionReply2 *) data)->port)
            );
            break;
    }
}

/**
 * Сообщение OPEN_CONNECTION_REQUEST_2 идет Client->Proxy->Server
 * Должен содержать в себе адрес сервера, но содержит адрес прокси
 * Нужно заменить адрес прокси на адрес сервера
 * @param data - данные пакета (без ip/udp заголовков)
 */
void openConnectionRequest2(char *data) {
    // Сначала идет 1 байт - айди пакета
    // Потом RakLib Magic - 16 байт
    // Дальше версия адреса 0x04 или 0x06
    // Нас интересует только IPv4
    if (data[17] == 0x04) {
        printk("Заменили IP в OPEN_CONNECTION_REQUEST_2");
        // Дальше 4 байта адрес
        data[18] = (~192) & 0xff;
        data[19] = (~168) & 0xff;
        data[20] = (~2)   & 0xff;
        data[21] = (~37)  & 0xff;
        // Порт
        data[22] = (8888 >> 8) & 0xff; // Так как сообщение идет от сервера
        data[23] = (8888) & 0xff;      // То ставим порт, который слушает клиентов
    }
}

/**
 * Сообщение OPEN_CONNECTION_REPLY_2 идет Server->Proxy->Client
 * Должен содержать в себе адрес клиента, но содержит адрес прокси
 * Нужно заменить адрес прокси на адрес клиента
 * @param data - данные пакета (без ip/udp заголовков)
 */
void openConnectionReply2(char* data, u16 clientPort) {
    printk("Заменили IP в OPEN_CONNECTION_REPLY_2");
    // Сначала идет 1 байт - айди пакета
    // Потом RakLub Magic - 16 байт
    // Дальше ServerId - 8 байт
    // Дальше 1 байт - версия адреса
    // 4 байта - адрес клиента
    data[26] = (~192) & 0xff;
    data[27] = (~168) & 0xff;
    data[28] = (~2)   & 0xff;
    data[29] = (~36)  & 0xff;
    // 2 байта - порт клиента
    data[30] = (clientPort >> 8) & 0xff;
    data[31] = (clientPort) & 0xff;
}
/*
void parse_encapsulated(char *data, u16 client_port) {
    u32 len;
    char* ptr;
    // Тип инкапсуляции
    if (data[3] == 0x40) {
        // Длина пакета - кол-во бит
        len = data[4] << 16 + data[5] << 8 + data[6];
        len /= 8; // Получаем кол-во байт

        prt = data[10];
        while (ptr < data + len) {
            // TODO: нужно парсить все пакеты :C
        }
    }
}
*/
void parse_packet(char *data, u16 client_port) {
    // Server->Client  0x07  ID_OPEN_CONNECTION_REQUEST_2 - должен содержать адрес сервера
    // Client->Server  0x08  ID_OPEN_CONNECTION_REPLY_2   - должен содержать адрес клиента
    if (data[0] == 0x07) openConnectionRequest2(data);
    if (data[0] == 0x08) openConnectionReply2(data, client_port);

    // Инкапсулированный пакет
    if (0x80 < data[0] && data[0] < 0x8f) {
        // Тип инкапсуляции хранится в 4 байте
        // parse_encapsulated(data, client_port);
    }
}