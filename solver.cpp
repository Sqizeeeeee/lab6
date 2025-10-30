#include "solver.h"
#include <iostream>
#include <cmath>

// Реализация функции вывода системы на экран
void printSystem(const LinearSystem& system) {
    std::cout << "Система уравнений:" << std::endl;
    for (int i = 0; i < N; i++) {
        std::cout << system.equations[i].coeffs[0] << "x + " 
                  << system.equations[i].coeffs[1] << "y + "
                  << system.equations[i].coeffs[2] << "z = "
                  << system.equations[i].rhs << std::endl;
    }
    std::cout << std::endl;
}

// Функция для вывода матрицы
void printMatrix(const LinearSystem& system) {
    std::cout << "Matrix: " << '\n';

    for (int i = 0; i < N; i ++) {
        for (int j = 0; j < N; j++) {
            std::cout << system.equations[i].coeffs[j] << '\t';
        }

        std::cout << "| " << system.equations[i].rhs << '\n';
    }
    std::cout << '\n';
}

// Функция для анализа и вывода решения (ПЕРЕМЕЩАЕМ ВВЕРХ)
void analyzeSolution(const LinearSystem& system) {
    std::cout << "\n=== АНАЛИЗ РЕШЕНИЯ ===" << std::endl;
    
    // Проверяем, есть ли противоречивые уравнения (0 = ненулевое число)
    bool hasContradiction = false;
    for (int i = 0; i < N; i++) {
        bool allZeroCoeffs = true;
        for (int j = 0; j < N; j++) {
            if (std::abs(system.equations[i].coeffs[j]) > EPS) {
                allZeroCoeffs = false;
                break;
            }
        }
        if (allZeroCoeffs && std::abs(system.equations[i].rhs) > EPS) {
            std::cout << "Обнаружено противоречие: 0 = " << system.equations[i].rhs 
                      << " -> СИСТЕМА НЕСОВМЕСТНА (нет решений)" << std::endl;
            hasContradiction = true;
        }
    }
    
    if (hasContradiction) {
        return;
    }
    
    // Проверяем количество независимых уравнений
    int rank = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (std::abs(system.equations[i].coeffs[j]) > EPS) {
                rank++;
                break;
            }
        }
    }
    
    std::cout << "Ранг системы: " << rank << " из " << N << std::endl;
    
    if (rank == N) {
        std::cout << "СИСТЕМА ИМЕЕТ ЕДИНСТВЕННОЕ РЕШЕНИЕ:" << std::endl;
        for (int i = 0; i < N; i++) {
            std::cout << char('x' + i) << " = " << system.equations[i].rhs << std::endl;
        }
    } else {
        std::cout << "СИСТЕМА ИМЕЕТ БЕСКОНЕЧНО МНОГО РЕШЕНИЙ" << std::endl;
        std::cout << "Количество свободных переменных: " << (N - rank) << std::endl;
        
        // Выводим уравнения в каноническом виде
        std::cout << "\nКанонический вид:" << std::endl;
        for (int i = 0; i < N; i++) {
            bool hasNonZero = false;
            for (int j = 0; j < N; j++) {
                if (std::abs(system.equations[i].coeffs[j]) > EPS) {
                    if (hasNonZero) std::cout << " + ";
                    std::cout << system.equations[i].coeffs[j] << char('x' + j);
                    hasNonZero = true;
                }
            }
            if (hasNonZero) {
                std::cout << " = " << system.equations[i].rhs << std::endl;
            }
        }
        
        // ДОБАВЛЯЕМ ВЫВОД ПАРАМЕТРИЧЕСКОГО РЕШЕНИЯ
        std::cout << "\n=== ПАРАМЕТРИЧЕСКОЕ РЕШЕНИЕ ===" << std::endl;
        
        // Определяем, какая переменная будет свободной (параметром)
        // Ищем столбец, где нет единичного вектора
        char freeVar = 'z'; // по умолчанию z как свободная переменная
        for (int j = 0; j < N; j++) {
            bool isBasic = false;
            for (int i = 0; i < N; i++) {
                if (std::abs(system.equations[i].coeffs[j] - 1.0) < EPS) {
                    isBasic = true;
                    break;
                }
            }
            if (!isBasic) {
                freeVar = 'x' + j;
                break;
            }
        }
        
        std::cout << "Свободная переменная (параметр): " << freeVar << " = t" << std::endl;
        std::cout << "Параметрическое решение:" << std::endl;
        
        // Выводим решение для каждой переменной
        for (int j = 0; j < N; j++) {
            char var = 'x' + j;
            if (var == freeVar) {
                std::cout << var << " = t" << std::endl;
            } else {
                // Ищем уравнение, где эта переменная базисная
                for (int i = 0; i < N; i++) {
                    if (std::abs(system.equations[i].coeffs[j] - 1.0) < EPS) {
                        std::cout << var << " = " << system.equations[i].rhs;
                        // Вычитаем вклады от свободных переменных
                        for (int k = 0; k < N; k++) {
                            if (k != j && std::abs(system.equations[i].coeffs[k]) > EPS && 
                                ('x' + k) == freeVar) {
                                double coeff = system.equations[i].coeffs[k];
                                if (coeff > 0) {
                                    std::cout << " - " << std::abs(coeff) << "t";
                                } else {
                                    std::cout << " + " << std::abs(coeff) << "t";
                                }
                            }
                        }
                        std::cout << std::endl;
                        break;
                    }
                }
            }
        }
        
        // Альтернативный вывод в векторной форме
        std::cout << "\nВ векторной форме:" << std::endl;
        std::cout << "(x, y, z) = (";
        bool first = true;
        for (int j = 0; j < N; j++) {
            char var = 'x' + j;
            if (!first) std::cout << ", ";
            if (var == freeVar) {
                std::cout << "t";
            } else {
                // Упрощенный вывод - берем правую часть из канонического вида
                for (int i = 0; i < N; i++) {
                    if (std::abs(system.equations[i].coeffs[j] - 1.0) < EPS) {
                        std::cout << system.equations[i].rhs;
                        // Добавляем коэффициент при параметре
                        for (int k = 0; k < N; k++) {
                            if (k != j && std::abs(system.equations[i].coeffs[k]) > EPS && 
                                ('x' + k) == freeVar) {
                                double coeff = -system.equations[i].coeffs[k];
                                if (coeff > 0) {
                                    std::cout << " + " << coeff << "t";
                                } else {
                                    std::cout << " - " << std::abs(coeff) << "t";
                                }
                            }
                        }
                        break;
                    }
                }
            }
            first = false;
        }
        std::cout << "), где t ∈ ℝ" << std::endl;
    }
}

// Основная функция решения
void solveJordanGauss(LinearSystem& system) {
    std::cout << "Начало решения методом Жордана-Гаусса..." << std::endl;
    printMatrix(system);
    
    // Прямой ход метода Жордана-Гаусса
    for (int pivot = 0; pivot < N; pivot++) {
        std::cout << "Шаг " << pivot + 1 << ": работаем с переменной " 
                  << char('x' + pivot) << std::endl;
        
        // Поиск ведущего элемента (опорного) в текущем столбце
        int maxRow = pivot;
        for (int i = pivot + 1; i < N; i++) {
            if (std::abs(system.equations[i].coeffs[pivot]) > 
                std::abs(system.equations[maxRow].coeffs[pivot])) {
                maxRow = i;
            }
        }
        
        // Если ведущий элемент почти нулевой, пропускаем столбец
        if (std::abs(system.equations[maxRow].coeffs[pivot]) < EPS) {
            std::cout << "Внимание: ведущий элемент в столбце " << pivot 
                      << " близок к нулю!" << std::endl;
            continue;
        }
        
        // Перестановка строк, если необходимо
        if (maxRow != pivot) {
            std::cout << "Меняем местами строки " << pivot + 1 << " и " << maxRow + 1 << std::endl;
            Equation temp = system.equations[pivot];
            system.equations[pivot] = system.equations[maxRow];
            system.equations[maxRow] = temp;
            printMatrix(system);
        }
        
        // Нормализация ведущей строки
        double pivotValue = system.equations[pivot].coeffs[pivot];
        std::cout << "Делим строку " << pivot + 1 << " на " << pivotValue << std::endl;
        for (int j = 0; j < N; j++) {
            system.equations[pivot].coeffs[j] /= pivotValue;
        }
        system.equations[pivot].rhs /= pivotValue;
        printMatrix(system);
        
        // Исключение переменной из всех других строк
        for (int i = 0; i < N; i++) {
            if (i != pivot) {
                double factor = system.equations[i].coeffs[pivot];
                std::cout << "Вычитаем из строки " << i + 1 << " строку " << pivot + 1 
                          << ", умноженную на " << factor << std::endl;
                for (int j = 0; j < N; j++) {
                    system.equations[i].coeffs[j] -= factor * system.equations[pivot].coeffs[j];
                }
                system.equations[i].rhs -= factor * system.equations[pivot].rhs;
                printMatrix(system);
            }
        }
    }
    
    std::cout << "Метод Жордана-Гаусса завершен!" << std::endl;
    analyzeSolution(system);  // Теперь функция объявлена выше
}