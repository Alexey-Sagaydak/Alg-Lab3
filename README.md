# Лабораторная работа №3

## Разработать

**Три реализации хеш-таблицы:**

- внутреннее хеширование, линейное зондирование;
- внутреннее хеширование, двойное хеширование;
- внешнее хеширование (список элементов для каждого значения ключа).

**Ключи:** целые числа от 0 до 1 000 000.

**Функции:** добавление, удаление, поиск.

## Протестировать
Для внутреннего хеширования посчитать среднее количество проб в серии экспериментов при успешном и неуспешном поиске. Сравнить с теоретическими оценками.

Для всех методов посчитать общее время выполнения поиска в серии экспериментов. Сравнить между собой.

Входные данные: случайные числа с появляющиеся с равными вероятностями, повторения исключить. Серия экспериментов успешного поиска - поиск всех добавленных значений послезаполнения (добавления) 10%, 20%,..., 90% таблицы. Для неуспешного поиска - искать такое же количество значений, которых нет в таблице.

Удаление протестировать отдельно, только на корректность.
