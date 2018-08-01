#!/bin/bash

MODULE_NAME="main"

MODULE=`lsmod | grep $MODULE_NAME`

# Проверяем загружен ли модуль
if [ -z "$MODULE" ]; then
    echo "Error: Module is not loaded!"
    exit;
fi

# Выгружаем модуль
sudo rmmod $MODULE_NAME

# Проверяем успешно ли загрузился модуль
MODULE=`lsmod | grep $MODULE_NAME`

if [ -z "$MODULE" ]; then
    echo "Success: Module unloaded!"
else
    echo "Error: Module not unloaded!"
fi
