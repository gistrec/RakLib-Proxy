// Обязательная часть любого загружаемого модуля ядра
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Frost");
MODULE_DESCRIPTION("A simple UDP proxy for linux");
MODULE_VERSION("0.01");

// __init это подсказка ядру, что функция используется только во время инициализации модуля,
// а значит, после инициализации модуля эту функцию можно выгрузить из памяти.
// Аналогично и для __exit
static int __init init(void) {
    pr_info("Module loaded!\n");
    return 0;
}

static void __exit exit(void) {
    pr_info("Module unloaded!\n");
}

module_init(init);
module_exit(exit);
