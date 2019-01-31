/**
 * Server -> Client
 */
struct OpenConnectionReply2 {
	char pid;            // 1 байт  - Packet ID = 0x08
	char magic[16];      // 16 байт - MAGIC
	u16  serverId;       // 8 байт  - Server ID

	u8  ip_version;     // 1 байт  - версия ip
	u8  ip[4];          // 4 байта - IP адрес
	u8  port;           // 2 байта - порт

	u16  mtuSize;        // 2 байта - Значение MTU
	bool serverSecurity; // 1 байт  - Всегда 0
};
typedef struct OpenConnectionReply2 OpenConnectionReply2;