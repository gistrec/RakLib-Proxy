/**
 * Массив с серверами
 */
Server servers[MAX_SERVERS] = { 0 };

/**
 * Добавляем сервер в сободную ячейку
 * @param ip   - строковое представление ip
 * @param port - стандартное представление порта
 * @return serverId
 */
int add_server(char* string_ip, u16 port) {
	int i;
	// Ищем не активный сервер
	for (i = 0; i < MAX_SERVERS; i++) {
		if (servers[i].active) continue;
		servers[i].active = true;
		// Заполняем ip
		strcpy(servers[i].string_ip, string_ip);
		servers[i].skbuff_ip = ip_to_int(string_ip);
		servers[i].netpoll_ip = htonl(ip_to_int(string_ip));
		ip_to_raklib(string_ip, servers[i].raklib_ip);
		// Заполняем port
		servers[i].int_port = port;
		servers[i].skbuff_port = port;
		servers[i].netpool_port = ntohs(port);
		servers[i].raklib_port[0] = (port >> 8) & 0xff;
		servers[i].raklib_port[1] = (port) & 0xff;
	}
	return i;
}

/**
 * Получаем сервер по порту
 * @param port big-endian (Из skbuff)
 */
Server* get_server(u16 skbuff_port) {
	int i;
	for (i = 0; i < MAX_SERVERS; i++) {
		if (servers[i].active && servers[i].skbuff_port == skbuff_port) {
			return &(servers[i]);
		}
	}
	return 0;
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

}