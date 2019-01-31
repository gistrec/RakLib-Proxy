/**
 * Структура клиента
 */
struct Client {
    bool active;

    /* Различные формы адреса клиента*/
    u32  nendian_ip;     // [Network endianness]
    u32  hendian_ip;     // [Host    endianness]

    /* Различные представления порта клиента */
    u16  nendian_sport;    // Исходный порт клиента [Network endianness]
    u16  hendian_sport;    // Исходный порт клиента [Host    endianness]

    /* Различные представления порта, которое прокси выделило клиенту */
    u16  nendian_bport; // Назначенный порт [Network endianness]
    u16  hendian_bport; // Назначенный порт [Host    endianness]

    u8 server_id; // ID сервера, на котором играет клиент
};
typedef struct Client Client;

// Заголовки функций
Client* get_client_by_port(u16);                  // client/ClientManager.c
void send_packet(u32, u16, u32, u16, char*, int); // Network.c