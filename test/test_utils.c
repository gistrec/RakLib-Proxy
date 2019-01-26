TEST(funct_ip_to_int) {
	char first[] = "10.0.2.15";
	u32 first_result = 0x0a00020f;

	char second[] = "192.168.56.1";
	u32 second_result = 0xc0a83801;

	u32 third_result = 0xc0a83802;
	char third[] = "192.168.56.2";

	check_assert(ip_to_int(first) == first_result,
		      "10.0.2.15 must be 0x0a00020f");
	check_assert(ip_to_int(second) == second_result,
		      "192.168.56.1 must be 0xc0a83801");
	check_assert(ip_to_int(third) == third_result,
		      "192.168.56.2 must be 0xc0a83802");
}

TEST(funct_ip_to_raklib) {
	char* first = "10.0.2.15";
	char* second = "192.168.56.1";

	u8 result[4]; // Не char, а u8 или unsigned char

	ip_to_raklib(first, (char *) &result);
	check_assert(result[0] == 245 && result[1] == 255 && result[2] == 253 && result[3] == 240,
		         "10.0.2.15 must be 0xf5fffdf0");

	ip_to_raklib(second, (char *) &result);
	check_assert(result[0] == 0x3f && result[1] == 0x57 &&
		      result[2] == 0xc7 && result[3] == 0xfe,
		      "192.168.56.1 must be 0x3f57c7fe");
}

TEST_SUITE(test_utils) {
	RUN_TEST(funct_ip_to_int);
	RUN_TEST(funct_ip_to_raklib);
}