#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <stdexcept>
#include <iomanip>
#include <windows.h>
#include <string>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;
using namespace chrono;

void setupConsole() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
}

class MatrixMultiplier {
private:
    vector<vector<int>> matrixA;
    vector<vector<int>> matrixB;
    vector<vector<long long>> matrixC;
    int size;

public:
    MatrixMultiplier(int n) : size(n) {
        matrixA.resize(n, vector<int>(n));
        matrixB.resize(n, vector<int>(n));
        matrixC.resize(n, vector<long long>(n, 0));
    }

    // Генерация случайных матриц
    void generateRandomMatrices() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(1, 10);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrixA[i][j] = dist(gen);
                matrixB[i][j] = dist(gen);
            }
        }
        cout << "Случайные матрицы сгенерированы." << endl;
    }

    // Чтение матриц из файлов
    void readMatricesFromFile(const string& filenameA, const string& filenameB) {
        cout << "Чтение файла: " << filenameA << endl;
        readMatrixFromFile(filenameA, matrixA);
        cout << "Чтение файла: " << filenameB << endl;
        readMatrixFromFile(filenameB, matrixB);
        cout << "Матрицы загружены из файлов." << endl;
    }

    void readMatrixFromFile(const string& filename, vector<vector<int>>& matrix) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Не могу открыть файл: " + filename);
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                file >> matrix[i][j];
                if (file.fail()) {
                    throw runtime_error("Ошибка чтения данных из файла на строке " + to_string(i + 1));
                }
            }
        }
        file.close();
    }

    // Последовательное умножение
    void multiplySequential() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrixC[i][j] = 0;
                for (int k = 0; k < size; k++) {
                    matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
                }
            }
        }
    }

    // Параллельное умножение
    void multiplyParallel(int numThreads) {
#ifdef _OPENMP
        omp_set_num_threads(numThreads);
#pragma omp parallel for collapse(2)
#endif
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                long long sum = 0;
                for (int k = 0; k < size; k++) {
                    sum += matrixA[i][k] * matrixB[k][j];
                }
                matrixC[i][j] = sum;
            }
        }
    }

    // Сохранение результата
    void saveResultToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Не могу создать файл: " + filename);
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                file << matrixC[i][j] << " ";
            }
            file << endl;
        }
        file.close();
        cout << "Результат сохранен в файл: " << filename << endl;
    }

    // Сохранение матриц для Python верификации
    void saveMatricesForPython() {
        ofstream fileA("matrixA_for_python.txt");
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                fileA << matrixA[i][j] << " ";
            }
            fileA << endl;
        }
        fileA.close();

        ofstream fileB("matrixB_for_python.txt");
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                fileB << matrixB[i][j] << " ";
            }
            fileB << endl;
        }
        fileB.close();

        ofstream fileC("result_cpp.txt");
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                fileC << matrixC[i][j] << " ";
            }
            fileC << endl;
        }
        fileC.close();

        cout << "Файлы для Python верификации сохранены" << endl;
    }

    // Верификация с помощью Python
    bool verifyWithPython() {
        cout << "\n--- Запуск Python верификации ---" << endl;

        saveMatricesForPython();

        int result = -1;

        cout << "Попытка: python verify_matrix.py" << endl;
        result = system("python verify_matrix.py");

        if (result != 0) {
            cout << "Попытка: py verify_matrix.py" << endl;
            result = system("py verify_matrix.py");
        }

        if (result != 0) {
            cout << "Попытка: python3 verify_matrix.py" << endl;
            result = system("python3 verify_matrix.py");
        }

        if (result == 0) {
            cout << "Python верификация успешно завершена!" << endl;

            ifstream verifFile("verification_result.txt");
            if (verifFile.is_open()) {
                string line;
                while (getline(verifFile, line)) {
                    cout << "   " << line << endl;
                }
                verifFile.close();
            }

            return true;
        }
        else {
            cout << "Не удалось запустить Python верификацию" << endl;
            cout << "Убедитесь, что Python установлен и файл verify_matrix.py существует" << endl;
            return false;
        }
    }

    // Вывод первых элементов
    void printFirstElements() {
        int n = min(3, size);

        cout << "\n--- Первые элементы матрицы A ---" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << matrixA[i][j] << " ";
            }
            cout << "..." << endl;
        }

        cout << "\n--- Первые элементы матрицы B ---" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << matrixB[i][j] << " ";
            }
            cout << "..." << endl;
        }

        cout << "\n--- Первые элементы результата C ---" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << matrixC[i][j] << " ";
            }
            cout << "..." << endl;
        }
    }

    int getSize() const { return size; }
};

int main(int argc, char* argv[]) {
    setupConsole();

    try {
        // Параметры по умолчанию - 200x200 из ваших файлов!
        int matrixSize = 3;  // Размер 200
        int numThreads = 1;
        string mode = "file";  // Режим чтения из файлов

        // Чтение аргументов командной строки (если есть)
        if (argc > 1) matrixSize = atoi(argv[1]);
        if (argc > 2) numThreads = atoi(argv[2]);
        if (argc > 3) mode = argv[3];

        cout << "===================================" << endl;
        cout << "    УМНОЖЕНИЕ КВАДРАТНЫХ МАТРИЦ    " << endl;
        cout << "===================================" << endl;
        cout << "Размер матрицы: " << matrixSize << "x" << matrixSize << endl;
        cout << "Количество потоков: " << numThreads << endl;

#ifdef _OPENMP
        cout << "OpenMP: ПОДДЕРЖИВАЕТСЯ" << endl;
#else
        cout << "OpenMP: НЕ ПОДДЕРЖИВАЕТСЯ" << endl;
#endif
        cout << "Режим: " << (mode == "generate" ? "Генерация" : "Из файлов") << endl;
        cout << "===================================" << endl;

        MatrixMultiplier multiplier(matrixSize);

        cout << "\n--- ИНИЦИАЛИЗАЦИЯ ---" << endl;
        if (mode == "generate") {
            cout << "Генерация случайных матриц..." << endl;
            multiplier.generateRandomMatrices();
        }
        else if (mode == "file") {
            cout << "Чтение матриц из файлов..." << endl;
            // Здесь указываем ВАШИ файлы matrixA200.txt и matrixB200.txt
            multiplier.readMatricesFromFile("matrixA.txt", "matrixB.txt");
        }
        else {
            cout << "Неизвестный режим. Используется генерация." << endl;
            multiplier.generateRandomMatrices();
        }

        cout << "\n--- ВЫЧИСЛЕНИЯ ---" << endl;
        auto start = high_resolution_clock::now();

        if (numThreads == 1) {
            cout << "Режим: последовательное умножение" << endl;
            multiplier.multiplySequential();
        }
        else {
            cout << "Режим: параллельное умножение (" << numThreads << " потоков)" << endl;
            multiplier.multiplyParallel(numThreads);
        }

        auto end = high_resolution_clock::now();

        // Время в секундах
        duration<double> duration_sec = end - start;

        cout << "\n--- СОХРАНЕНИЕ ---" << endl;
        multiplier.saveResultToFile("result_matrix.txt");

        cout << "\n--- РЕЗУЛЬТАТЫ ---" << endl;
        // Вывод с 5 знаками после запятой в секундах
        cout << fixed << setprecision(5);
        cout << "Время выполнения: " << duration_sec.count() << " с" << endl;
        cout << "Размер задачи: " << matrixSize << "x" << matrixSize << endl;
        cout << "Количество операций: " << (long long)matrixSize * matrixSize * matrixSize << endl;

        multiplier.printFirstElements();

        // Python верификация (опционально)
        cout << "\n--- ВЕРИФИКАЦИЯ ---" << endl;
        bool verified = multiplier.verifyWithPython();
        if (verified) {
            cout << "Результаты проверены Python" << endl;
        }
        else {
            cout << "Верификация не выполнена" << endl;
        }

        // Сохранение времени в секундах в CSV
        ofstream resultsFile("experiment_results.csv", ios::app);
        resultsFile << fixed << setprecision(5);
        resultsFile << matrixSize << "," << numThreads << "," << duration_sec.count() << "," << (verified ? "OK" : "FAIL") << endl;
        resultsFile.close();

        cout << "\n===================================" << endl;
        cout << "ПРОГРАММА ЗАВЕРШЕНА" << endl;
        cout << "Сохраненные файлы:" << endl;
        cout << "  - result_matrix.txt" << endl;
        cout << "  - matrixA_for_python.txt" << endl;
        cout << "  - matrixB_for_python.txt" << endl;
        cout << "  - result_cpp.txt" << endl;
        cout << "  - experiment_results.csv" << endl;
        cout << "===================================" << endl;

    }
    catch (const exception& e) {
        cerr << "\nОШИБКА: " << e.what() << endl;
        return 1;
    }

    cout << "\nНажмите Enter для выхода...";
    cin.get();
    return 0;
}
