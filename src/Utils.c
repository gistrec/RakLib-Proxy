/**
 * Преобразуем строковое представление IP
 * в представление ip в раклибе
 */
void ip_to_raklib(char* string_ip, char* raklib_ip) {
	char a[4];

	sscanf(string_ip, "%hhu.%hhu.%hhu.%hhu", &a[0], &a[1], &a[2], &a[3]);
	raklib_ip[0] = (~a[0]) & 0xff;
	raklib_ip[1] = (~a[1]) & 0xff;
	raklib_ip[2] = (~a[2]) & 0xff;
	raklib_ip[3] = (~a[3]) & 0xff;
}

/**
 * Возвращает u32 представление ip в host
 * @param  strip [description]
 * @return       [description]
 */
u32 ip_to_int(char* string_ip) {
	u32 ip;
    unsigned int a[4];

    sscanf(string_ip, "%u.%u.%u.%u", &a[0], &a[1], &a[2], &a[3]);
    ip = (a[0] << 24) + (a[1] << 16) + (a[2] << 8) + a[3];

	return ip;
}

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