/**
 * Массив с клиентами
 */
Client clients[MAX_CLIENTS] = { 0 };

/**
 * Функция вызывается при получении пакета от клиента
 * @param saddr - адрес клиента [Network endianness]
 * @param sport - порт  клиента [Network endianness]
 * @param data - данные UDP пакета
 * @param len  - размер данных
 */
void handle_client_packet(u32 saddr, u16 sport, char* data, int len) {

}