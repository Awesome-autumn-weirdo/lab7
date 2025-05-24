# lab7
Ход выполнения
Установка и подготовка среды
Работа выполнялась в среде Ubuntu 18.04. Установлены следующие инструменты:

clang — компилятор языка C/C++

llvm — инструменты анализа и оптимизации кода

opt — инструмент для работы с LLVM IR и применения оптимизаций

Graphviz — инструмент для визуализации кода

Команда установки:

sudo apt install clang llvm graphviz
![изображение](https://github.com/user-attachments/assets/e1b44a29-b645-43f7-8784-b133c2fef34a)
Исходный код
Программа на языке C:
#include <stdio.h>
int square(int x) {
    return x * x;
}
int main() {
int a = 5;
int b = square(a);

printf("%d\n", b);
return 0;
}
Сохранена в файл main.c.
![изображение](https://github.com/user-attachments/assets/696a8331-166d-4e3a-87e1-6f1d3f306a48)
Получение AST
Команда: clang -Xclang -ast-dump -fsyntax-only main.c
![изображение](https://github.com/user-attachments/assets/cc5a39d2-b768-4c1e-b5ac-d3c6cd3f51ba)
Функция square принята, содержит параметр x и возвращает x * x.
Генерация LLVM IR
Команда: clang -S -emit-llvm main.c -o main.ll
![изображение](https://github.com/user-attachments/assets/c75acd95-5c81-4e7d-8031-807d254df318)
Оптимизация IR
Команда: clang -O0 -S -emit-llvm main.c -o main_O0.ll
Стоит отметить, что в файле с IR до оптимизации:
Все переменные (a, b, x.addr) размещены в памяти через alloca;
Множество операций load и store;
square вызывается как отдельная функция.
![изображение](https://github.com/user-attachments/assets/3d564c36-ed14-44a3-ad68-53517989e784)
Команда: clang -O2 -S -emit-llvm main.c -o main_O2.ll
Команда -O2 – комплексная оптимизация среднего уровня. Она приме-няет более 30 различных оптимизаций:
● -inline – встраивание небольших функций (встраивает square в
main, если она вызывается один раз);
● -constprop – подставит значение square(5) → 25, если функция
встроена и всё известно на этапе компиляции;
● -mem2reg – перевод переменных из памяти в регистры (SSA);
● -instcombine – объединение и упрощение инструкций
(упростит арифметику, например x * x может быть преобразовано в shl при x = 2^n);
● -simplifycfg – оптимизирует структуру блоков (Упростит граф
управления, если после inlining останутся лишние блоки);
● -reassociate, -gvn, -sroa, -dce и другие.
В файле с IR после оптимизации:
Вся функция square исчезла – она была встроена (-inline) и затем вычис-лена (оптимизация -constprop);
Никаких переменных, alloca, store, load – всё удалено (оптимизации -mem2reg, -dce);
Остался только вызов printf(25).
![изображение](https://github.com/user-attachments/assets/d4701187-c30f-4145-8fd4-511923855b4c)
Команда: diff main_O0.ll main_O2.ll
Сравнение двух файлов:
![изображение](https://github.com/user-attachments/assets/017bf43c-cbdf-49e4-84ad-82cc2783233e)
Стоит отметить, что после оптимизации произошли следующие измене-ния:
● Переменные типа alloca были удалены;
● Код переведён в SSA-форму;
● Оптимизация улучшила читаемость и упростила поток управления.

Граф потока управления программы
Команда для генерации оптимизированного LLVM IR: 
clang -O2 -S -emit-llvm main.c -o main.ll
Команда для генерации .dot-файлов CFG для функций: 
opt -dot-cfg -disable-output main.ll
![изображение](https://github.com/user-attachments/assets/41ab8d2f-9bb4-4355-87d8-eda8a70b2cf8)
Команда для установки библиотеки Graphviz: 
sudo apt install graphviz
Команды для преобразования файлов с расширением .dot в .png с по-мощью Graphviz:
dot -Tpng cfg.square.dot -o cfg.square.png
dot -Tpng cfg.main.dot -o cfg.main.png
Команды для просмотра файлов с CGF:
xdg-open cfg.main.png
![изображение](https://github.com/user-attachments/assets/73dd97ba-2878-4626-9f6b-c532870148d4)
![изображение](https://github.com/user-attachments/assets/b2fa42b9-bb8b-4394-8e70-8f9820f2285d)

Стоит отметить, что в LLVM каждый граф потока управления (CFG) строится на уровне функции, поскольку структура управления всегда локаль-на для тела функции. Для получения полного представления о программе, нужно построить CFG для всех функций и анализировать их совокупность. Автоматическое объединение всех CFG в один граф не предусмотрено в LLVM по умолчанию.
Выводы
● С помощью Clang можно получить полную структуру AST и IR, а также CGF;
● LLVM предоставляет гибкие инструменты анализа и оптимизации;
● Промежуточное представление кода удобно для написания компиля-торных трансформаций.


Доп. Задание В1
1. Код для оптимизации
#include <iostream>
#include <complex>
#include <cmath>
int main() {
    std::complex<double> z(3.0, 4.0);
    std::cout << "Модуль: " << std::abs(z) << std::endl;
    return 0;
          }

2. Построение AST
clang++ -Xclang -ast-dump -fsyntax-only main.cpp
![изображение](https://github.com/user-attachments/assets/e1eafbbe-2493-45bd-a425-842aeefe524c)
3. Построение LLVM IR
clang++ -O0 -emit-llvm -S main.cpp -o main.ll
![изображение](https://github.com/user-attachments/assets/f7529829-41da-4f2a-9ae0-0c24a039ce7b)
Анализ
Вызов конструктора:
1.	Выделяет память под std::complex.
2.	Передаёт real и imag как аргументы.
3.	Конструктор сохраняет значения в структуру.
Модуль комплексного числа:
1.	Вызывает шаблон std::abs.
2.	Извлекает Re и Im.
3.	Вызывает вспомогательную функцию, которая вызывает cabs(re, im).
Доп. Задание В2
Код:
// main.c
#include <stdio.h>
struct Point {
    int x;
    int y;
};
void printPoint(struct Point p) {
    printf("Point: (%d, %d)\n", p.x, p.y);
}
int main() {
    struct Point p = {2, 3};
    printPoint(p);
    return 0;
}
Скомпилируем с помощью clang:
clang -S -emit-llvm -o main.ll main.c
![изображение](https://github.com/user-attachments/assets/da33f16b-da2b-4416-a255-d786e3cdf02a)
Анализ
Структура: %struct.Point = type { i32, i32 }
В LLVM IR структура Point передаётся как агрегатный тип.
При передаче в функцию printPoint, структура передаётся по значению, т.е. создаётся её копия. Поскольку структура маленькая, она встраивается напрямую в регистры или стек.
Доп. Задание В3
Код:
// main.cpp
#include <iostream>
#include <map>
#include <string>
int main() {
    std::map<std::string, int> myMap = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };
    for (const auto& [key, value] : myMap) {
        std::cout << key << ": " << value << "\n";
    }
    return 0;
}

clang++ -Xclang -ast-dump -fsyntax-only main.cpp
![изображение](https://github.com/user-attachments/assets/b3bf5975-ef0b-4903-a566-141dbad16a50)
clang++ -std=c++17 -S -emit-llvm main.cpp -o main.ll
![изображение](https://github.com/user-attachments/assets/ffa7eec8-834a-4738-ade8-25e7d356768f)
Анализ
Инициализация через {}:
•	В AST используется std::initializer_list.
•	В IR вызывается конструктор std::map, принимаю-щий initializer_list.
Вставка через insert:
•	В AST вызов метода insert.
•	В IR создаётся временный pair, затем вызывается insert.
Доп. Задание В4
Код:
// Прототип функции
int sum(int a, int b);
// Функция, использующая прототип
int main() {
    int result = sum(3, 4);
    return result;
}
// Определение функции
int sum(int a, int b) {
    return a + b;
}

clang -Xclang -ast-dump -fsyntax-only main.cpp
![изображение](https://github.com/user-attachments/assets/50712806-c73a-4cf5-8c61-426886ce22a8)
clang -S -emit-llvm -o main.ll main.cpp
![изображение](https://github.com/user-attachments/assets/87c042dc-73e4-4d06-9b87-422f25d58704)
Анализ
AST отражает синтаксическую структуру, включая прототипы, типы и семантику C++.
IR опускает "синтаксический сахар" и фокусируется на инструкциях, близких к ассемблеру. Прототип в IR либо объявляется через declare, либо заменяется на define, если есть тело функции.

Доп. Задание В5
Код:
// main.cpp
const int LIMIT = 100;
int check(int x) {
    if (x < LIMIT) {
        return 1;
    } else {
        return 0;
    }
}

clang -O2 -S -emit-llvm main.cpp -o main.ll
![изображение](https://github.com/user-attachments/assets/7b0ae5b1-a411-4d81-a8ba-78488bdb396f)
Анализ
Да, компилятор Clang подставляет const int LIMIT = 100 при оптимиза-ции -O2 благодаря constant propagation.

Доп. Задание В6
Код:
// main.cpp
const double PI = 3.1415;
double circle_area(double r) {
    return PI * r * r;
}

clang -O2 -S -emit-llvm main.cpp -o main.ll
![изображение](https://github.com/user-attachments/assets/a6a92022-ba83-4cb7-9c3f-350c1b18c056)
Анализ
Значение PI встроено напрямую как литерал 3.141500e+00.
Нет обращения к глобальной переменной PI — значит, произошла под-становка константы (constant propagation).
Доп. Задание В7
Код:
// main.cpp
#include <stdio.h>

int main() {
    char msg[] = "Hello";
    putchar(msg[1]); // Ожидается 'e'
    return 0;
}

clang -S -emit-llvm main.cpp -o main.ll
![изображение](https://github.com/user-attachments/assets/89d2007c-ef92-4b56-b5ec-7613691a06a1)
Анализ
Clang  хранит строковый литерал в глобальной секции как constant [N x i8], копирует его в локальный массив msg[], обращается к элементу через GEP + load.

Доп. Задание В8
Код:
// main.cpp
enum Color { Red, Green, Blue };

int getColorValue() {
    Color c = Green;
    return c;
}

clang -S -emit-llvm main.cpp -o main.ll
![изображение](https://github.com/user-attachments/assets/58733ef9-d89d-4fee-902e-1d7d3c61fded)
Анализ
В LLVM IR enum заменяется на целочисленное значение (i32).

Доп. Задание В9
Код:
// main.cpp
enum Color { Red, Green, Blue };

int getColorValue() {
    Color c = Green;
    return c;
}

clang++ -emit-llvm -S -O0 main.cpp -o main.ll
![изображение](https://github.com/user-attachments/assets/e5856725-6979-4a64-9fcb-1c46503cf581)
opt -dot-cfg main.ll
dot -Tpng cfg._Z7averageii.dot -o cfg._Z7averageii.png
dot -Tpng cfg.main.dot -o cfg.main.png
![изображение](https://github.com/user-attachments/assets/bfccd881-bc8f-4b44-8399-c969290a60e9)
![изображение](https://github.com/user-attachments/assets/55a433a1-71e3-4204-93a3-a4912b426468)
Анализ
alloca + store + load — можно заменить на прямую работу с аргумента-ми:
%7 = add nsw i32 %0, %1
Это делается при оптимизации SSA (Static Single Assignment) и называ-ется mem2reg.
fdiv double %x, 2.0 может быть заменена на: fmul double %x, 0.5

Доп. Задание В10
Код:
int main() {
    auto f = [](int x) { return x * x; };
    int result = f(3);
    return result;
}

clang++ -O1 -S -emit-llvm lambda.cpp -o lambda.ll
![изображение](https://github.com/user-attachments/assets/3bd42a0c-266b-46f0-8434-ce5796bcf6b5)
opt -dot-cfg lambda.ll
dot -Tpng cfg.main.dot -o cfg.main.png
![изображение](https://github.com/user-attachments/assets/d1b4ab6e-3938-407a-8fd2-c52e0ab1f51a)
Анализ
Лямбда не захватывает никаких переменных из внешнего контекста, значит замыкания нет.
Функция main содержит единственный базовый блок, который возвра-щает значение 9 (3*3). 
Доп. Задание В11
Код:
#include <stdio.h>
int sum_array(int* arr, int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i)
        sum += arr[i];
    return sum;
}
int main() {
    int data[5] = {1, 2, 3, 4, 5};
    int total = sum_array(data, 5);
    printf("Sum = %d\n", total);
    return 0;
}

clang -S -emit-llvm test.cpp -o test.ll
![изображение](https://github.com/user-attachments/assets/2efa2886-8bbc-43d7-b5f5-03fbc87c7e98)
opt -loop-unroll -licm -sroa test.ll -o test_opt.bc
llvm-dis test_opt.bc -o test_opt.ll
![изображение](https://github.com/user-attachments/assets/1b71136f-aec5-4e19-a9ac-17ca97cddc10)
dot -Tpng cfg._Z9sum_arrayPii.dot -o cfg._Z9sum_arrayPii.png
dot -Tpng cfg.main.dot -o cfg.main.png
![изображение](https://github.com/user-attachments/assets/314d677a-15a5-4c90-992a-b1922f0fae9a)
![изображение](https://github.com/user-attachments/assets/3fb9ddc9-d1d6-4295-b1db-dd0d30eb3ddc)
Анализ
Нет признаков распаковки цикла (-loop-unroll не сработал).
Нет изменения в структуре кода, что говорит, что LLVM не применил оптимизации -licm и -sroa в данном случае.
-loop-unroll по умолчанию может требовать настроек, чтобы распако-вать маленькие циклы.
-licm и -sroa эффективнее при сложных или больших функциях с более выраженными инвариантами и агрегатами.
CFG структурирован одинаково до и после.
