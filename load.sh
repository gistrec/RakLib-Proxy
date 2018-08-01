#!/bin/bash

MODULE_NAME="main"

MODULE=`lsmod | grep $MODULE_NAME`

# Проверяем загружен ли уже модуль или нет
if [ -n "$MODULE" ]; then
    echo "Error: Module is already loaded!"
    exit;
fi

# Загружаем модуль
sudo insmod "$MODULE_NAME.ko"

# Проверяем успешно ли загрузился модуль
MODULE=`lsmod | grep $MODULE_NAME`

if [ -n "$MODULE" ]; then
    echo "Success: Module loaded!"
else
    echo "Error: Module not loaded!"
fi
