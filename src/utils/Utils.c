/**
 * Устанавливаем адрес в RakLib пакете
 * @param address   - адрес, который нужно установить [Network endianness]
 * @param raklib_ip - указатель на адрес в пакете
 */
void set_raklib_address(u32 address, u8* raklib_ip) {
    // Для каждого байта, который ищем с помощью address >> 24
    // Делаем (~byte) & 0xff
    raklib_ip[3] = ~(address >> 24) & 0xff;
    raklib_ip[2] = ~(address >> 16) & 0xff;
    raklib_ip[1] = ~(address >>  8) & 0xff;
    raklib_ip[0] = ~(address >>  0) & 0xff;
}

/**
 * Устанавливаем порт в RakLib пакете
 * @param port        - порт, который нужно установить [Network endianness]
 * @param raklib_port - указатель на порт в пакете
 */
void set_raklib_port(u16 port, u8* raklib_port) {
    raklib_port[1] = (port >> 8) & 0xff;
    raklib_port[0] = (port) & 0xff;
}

/**
 * Выводим данные в HEX представлении
 * @param data - данные
 * @param len  - размер данных
 */
void dump(char* data, int len) {
    int i;
    printk(KERN_CONT "\n"); // Перенос на новую строку
    for (i = 0; i < len;) {
        printk(KERN_CONT "0x%1ph ", &data[i]);

        i++;

        if      (i % 8 == 0) printk(KERN_CONT "\n");   // Перенос на новую строку
        else if (i % 4 == 0) printk(KERN_CONT "    "); // Большой отступ
    }
}