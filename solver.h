#ifndef SOLVER_H
#define SOLVER_H

const double EPS = 1e-10;  // Точность для сравнения чисел с плавающей точкой
const int N = 3;           // Размерность системы уравнений

// Структура представляет одно линейное уравнение
// coeffs[0] - коэффициент при x, coeffs[1] - при y, coeffs[2] - при z
struct Equation {
    double coeffs[N];  // Коэффициенты при переменных
    double rhs;        // Правая часть уравнения (свободный член)
};

// Структура представляет всю систему линейных уравнений
struct LinearSystem {
    Equation equations[N];  // Массив из N уравнений
};

// Прототипы функций
void printSystem(const LinearSystem& system);      // Вывод системы на экран
void solveJordanGauss(LinearSystem& system);       // Решение системы методом Жордана-Гаусса
void analyzeSolution(const LinearSystem& system);

#endif