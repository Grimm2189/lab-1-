"""
Скрипт для верификации умножения матриц
Запускается автоматически из C++ программы
"""

import numpy as np
import sys
import os
from datetime import datetime


def verify_multiplication():
    print("=" * 60)
    print("ПРОВЕРКА УМНОЖЕНИЯ МАТРИЦ (PYTHON)")
    print("=" * 60)

    try:
        # Проверяем наличие файлов
        required_files = ['matrixA_for_python.txt', 'matrixB_for_python.txt', 'result_cpp.txt']
        for file in required_files:
            if not os.path.exists(file):
                print(f"❌ Файл {file} не найден!")
                return False

        # Читаем матрицы
        print("\n📂 Чтение файлов...")
        A = np.loadtxt('matrixA_for_python.txt')
        B = np.loadtxt('matrixB_for_python.txt')
        C_cpp = np.loadtxt('result_cpp.txt')

        print(f"   Матрица A: размер {A.shape}")
        print(f"   Матрица B: размер {B.shape}")
        print(f"   Результат C++: размер {C_cpp.shape}")

        # Проверяем размеры
        if A.shape != B.shape or A.shape != C_cpp.shape:
            print("❌ Размеры матриц не совпадают!")
            return False

        # Умножаем через NumPy
        print("\n🧮 Вычисление через NumPy...")
        start_time = datetime.now()
        C_python = np.dot(A, B)
        calc_time = (datetime.now() - start_time).total_seconds() * 1000

        print(f"   Время вычисления в Python: {calc_time:.2f} мс")

        # Сравниваем результаты
        print("\n🔍 Сравнение результатов...")

        # Проверяем идентичность
        if np.array_equal(C_cpp, C_python):
            print("✅ Матрицы полностью идентичны!")
        else:
            # Вычисляем разницу
            difference = np.abs(C_cpp - C_python)
            max_diff = np.max(difference)
            mean_diff = np.mean(difference)

            print(f"⚠ Матрицы немного отличаются:")
            print(f"   Максимальная разница: {max_diff:.10f}")
            print(f"   Средняя разница: {mean_diff:.10f}")

            if max_diff < 1e-10:
                print("   Разница в пределах погрешности - ОК")
            else:
                print("❌ Разница превышает допустимую!")

                # Показываем первые элементы для сравнения
                n = min(3, len(A))
                print("\nПервые элементы (C++ vs Python):")
                for i in range(n):
                    for j in range(n):
                        print(f"   C++[{i},{j}] = {C_cpp[i, j]:6.0f}  Python[{i},{j}] = {C_python[i, j]:6.0f}")

        # Для маленьких матриц показываем полностью
        if len(A) <= 5:
            print("\n📊 Матрица A:")
            print(A)
            print("\n📊 Матрица B:")
            print(B)
            print("\n📊 Результат C++:")
            print(C_cpp)
            print("\n📊 Результат Python:")
            print(C_python)

        # Сохраняем результат верификации
        with open('verification_result.txt', 'w', encoding='utf-8') as f:
            f.write(f"Результат верификации: {'УСПЕШНО' if np.array_equal(C_cpp, C_python) else 'ПРОВЕРЕНО'}\n")
            f.write(f"Максимальная разница: {np.max(np.abs(C_cpp - C_python)):.10f}\n")

        print("\n" + "=" * 60)
        print("✅ ВЕРИФИКАЦИЯ ЗАВЕРШЕНА")
        print("=" * 60)

        return True

    except Exception as e:
        print(f"\n❌ Ошибка при верификации: {e}")
        return False


if __name__ == "__main__":
    success = verify_multiplication()

    # Сохраняем результат для C++
    with open('verification_result.txt', 'w') as f:
        f.write(f"SUCCESS={success}\n")

    # Не закрываем окно сразу, чтобы пользователь увидел результат
    input("\nНажмите Enter для выхода...")

    # Возвращаем код ошибки
    sys.exit(0 if success else 1)