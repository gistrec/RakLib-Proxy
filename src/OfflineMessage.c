// Обработка OfflineMessage

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