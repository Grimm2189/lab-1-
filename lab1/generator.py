import random


def create_matrix(filename, size=1600):
    """Создает матрицу size x size со случайными числами от 1 до 9"""
    with open(filename, 'w', encoding='utf-8') as f:
        for i in range(size):
            # Создаем строку из 100 случайных чисел
            row = [str(random.randint(1, 9)) for _ in range(size)]
            # Записываем строку в файл (числа через пробел)
            f.write(' '.join(row) + '\n')

            # Показываем прогресс
            if (i + 1) % 10 == 0:
                print(f"Создано {i + 1} строк из {size}")


print("=" * 50)
print("СОЗДАНИЕ МАТРИЦ 100x100")
print("=" * 50)

# Создаем матрицу A
print("\nСоздание матрицы A...")
create_matrix("matrixA1600.txt")

# Создаем матрицу B
print("\nСоздание матрицы B...")
create_matrix("matrixB1600.txt")

print("\n" + "=" * 50)
print("ГОТОВО!")
print("Созданы файлы:")
print("  - matrixA.txt (100x100)")

print("  - matrixB.txt (100x100)")
print("=" * 50)

# Показываем первые несколько чисел для проверки
print("\nПервые числа из matrixA.txt:")
with open('matrixA.txt', 'r') as f:
    for i in range(3):
        line = f.readline().strip()
        print(f"Строка {i + 1}: {line[:20]}...")

input("\nНажмите Enter для выхода...")
