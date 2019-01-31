/**
 * Структура сервера
 */
struct Server {
    bool active; // Активен ли сервер
    bool main;   // Главный ли сервер

    /* Различные формы адреса сервера */
    u32 nendian_ip;     // [Network endianness]
    u32 hendian_ip;     // [Host    endianness]

    /* Различные представления порта сервера */
    u16 nendian_port;    // Порт сервера [Network endianness]
    u16 hendian_port;    // Порт сервера [Host    endianness]
};
typedef struct Server Server;

// Заголовки функций
void send_packet(u32, u16, u32, u16, char*, int); // Network.c