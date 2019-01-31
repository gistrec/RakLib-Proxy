/**
 * Client -> Server
 */
struct OpenConnectionRequest2 {
	char pid;         // 1 байт  - Packet ID = 0x06
	char magic[16];   // 16 байт - MAGIC

	u8  ip_version;  // 1 байт  - версия адреса
	u8  ip[4];       // 4 байта - адрес сервера
	u8  port[2];     // 2 байта - порт сервера

	u16  mtuSize;     // 2 байта - Значение MTU
	u64  serverId;    // 8 байт  - Server ID
};
typedef struct OpenConnectionRequest2 OpenConnectionRequest2;