/**
 * Структура сервера
 * @param *_ip   - различные формы IP адреса
 * @param *_port - различные формы порта
 */
struct Server {
	bool active;

	// Строковое представление IP
	// Например, 192.168.0.100
	char   string_ip[16];
	// Формат IP в IPv4 пакетах
	// Превращаем адрес 192.168.0.100 (0xc0 0xa8 0x00 0x64) в 3232235620
	// Получить можно с помощью ip_hdr(skbuff)->saddr
	u32    skbuff_ip;
	// Формат IP в netpoll - используется для отправки пакетов
	// Получить можно с помощью ntohl(skbuff_ip)
	__be32 netpoll_ip;
	// Формат IP в RakLib пакетах
	// Для каждого байта делаем  (~u8) & 0xff
	char   raklib_ip[4];

	// Стандартное представление порта, например 19132
	u16  int_port;
	// Формат порта в IPv4
	// Тоже что и int_port
	u16  skbuff_port;
	// Формат порта,  в netpool
	// Получить можно с помощью ntohs(skbuff_port)
	u16  netpool_port;
	// Формат порта в RakLib пакетах
	// (port >> 8) & 0xff   +   (port) & 0xff
	char raklib_port[2];
};
typedef struct Server Server;