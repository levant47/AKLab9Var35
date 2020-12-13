#include <stdint.h>
#include <stdio.h>

const float zero = 0;

void first_task()
{
    float a = 418;
    float b = 2;
    float c = 7.3f;
    const int result_size = 5;
    float result[result_size];

    __asm
    {
        finit ; очистка регистров FPU
        mov ecx, 0 ; инициализация счетчика цикла
        fld1 ; загружаем 1 на стек как изначальное значение N
        fld [zero] ; загружаем 0 на стек как изначальное значение суммы

        ; к началу каждой итерации цикла в ST(0) будет находиться текущее значение суммы, а в ST(1) текущее значение N
        loop_start:
        fld ST(1) ; загружаем N повторно на стек
        fmul ST(0), ST(0) ; считаем N^2
        fmul [b] ; считаем 2 * N^2
        fadd [c] ; считаем 2 * N^2 + 7.3
        fdivr [a] ; считаем 418 / (2 * N^2 + 7.3)
        fadd ; добавляем вычисленное значение функции к сумме
        fst [result + ecx * 4] ; записываем текущее значение суммы в массив с результатами

        fld1 ; загружаем 1 на стек
        fadd ST(2), ST(0) ; добавляем единицу к текущему значению N
        fstp ST(0) ; убираем 1 со стека
        inc ecx ; увеличиваем счетчик цикла
        cmp ecx, result_size
        jne loop_start ; если прошло 5 итераций, то мы прерываем цикл
    }

    printf("1st\n");
    for (int i = 0; i < result_size; i += 1)
    {
        printf("%d: %f\n", i + 1, result[i]);
    }
    printf("\n");
}

void second_task()
{
    float a = 20.f;
    float b = 2.5f;
    float c = 0.2f;
    float result;

    __asm
    {
        finit ; очистка регистров FPU
        fld1 ; загружаем на стек 1, которая будет начальным значением X

        ; в начале каждой итерации цикла в ST(0) будет храниться текущее значение X
        loop_start:
        fld ST(0) ; загружаем значение X повторно на стек
        fld [b] ; загружаем 2.5 на стек
        fyl2x ; считаем X * log_2(2.5)
        fld1
        fld ST(1) ; повторно загружаем результат X * log_2(2.5)
        fprem ; считаем дробную часть X * log_2(2.5)
        f2xm1 ; считаем 2 ^ (дробную часть от X * log_2(2.5)) - 1
        fadd ; добавляем 1 к результату
        fscale ; завершить вычисление 2.5 ^ X, результат находится в ST(0)
        fxch ST(1)
        fstp ST(0) ; по окончанию блока в ST(0) находится значение 2.5^X, а в ST(1) текущее значение X

        fld ST(1) ; повторно загрузить текущее значение X на стек
        fmul ST(0), ST(0) ; считаем X^2
        fadd ; сложить X^2 + 2.5^X
        fdivr [a] ; вычислить 20 / (X^2 + 2.5^X)

        fld [c]
        fcomip ST(0), ST(1)
        jb loop_end ; если текущее значение функции меньше 0.2, то закончить цикл

        fstp ST(0) ; убрать текущее значение функции со стека, так как оно нам больше не нужно
        fld1
        fadd ; увеличить на 1 текущее значение X
        jmp loop_start ; продолжить цикл

        loop_end:
        fstp [result] ; записать текущее значение функции в память
    }

    printf("2nd: %d\n\n", (int)result);
}

void third_task()
{
    float result;
    float a = 2;
    float b = 4;
    float c = 40;
    float d = 180;
    float e = -1;
    float f = 3;

    __asm
    {
        finit ; очистка регистров FPU

        fld [a]
        fld [b]
        fmul ; считаем 4 * A

        fld [c]
        fld [d]
        fdiv ; считаем 40 / 180
        fldpi ; загружаем π на стек
        fmul ; вычисляем π * 40 / 180, что является переводом 40° в радианы
        fptan ; вычисляем tan(40°), результат находится в ST(1)
        fstp ST(0) ; убираем ненужное значение со стека

        fadd ; считаем 4*A + tan(40°)

        ; считаем арккосеканс по формуле arccsc(x) = arctan(sqrt(1 / (x*x - 1)))
        ; далее будем обозначать текущий результат, который является аргументом арккосеканса, как X
        fld ST(0)
        fmul ; вычисляем X^2
        fld1
        fsub ; вычисляем X*X - 1
        fld1
        fdivr ; вычисляем 1 / (X*X - 1)
        fsqrt ; вычисляем sqrt(1 / (X*X - 1))
        fld1
        fpatan ; вычисляем arctan(sqrt(1 / (X*X - 1))), что равно arccsc(X), где X это предыдущий результат = 4*A + tan(40°)

        fld [f]
        fmul ; вычисляем 3 * arccsc(4*A + tan(40°))

        fstp [result] ; сохраняем результат в памяти
    }

    printf("3rd: %f\n\n", result);
}

void fourth_task()
{
    float initial_value = 0.5f;
    float step = 0.2f;
    float a = 7.f;
    const int result_size = 5;
    float result[result_size];

    __asm
    {
        finit ; очистка регистров FPU
        mov ecx, 0 ; инициализация счетчика цикла
        fld [initial_value] ; загружаем начальное значение X на стек

        ; в начале каждой итерации цикла в ST(0) хранится текущее значение X
        loop_start:
        fld ST(0) ; повторно загружаем X
        fld [a]
        fyl2x ; считаем X * log_2(7)
        fld1
        fld ST(1) ; повторно загружаем результат X * log_2(7)
        fprem ; считаем дробную часть X * log_2(7)
        f2xm1 ; считаем 2 ^ (дробную часть от X * log_2(7)) - 1
        fadd ; добавляем 1 к результату
        fscale ; завершить вычисление 7 ^ X, результат находится в ST(0)
        fxch ST(1)
        fstp ST(0) ; по окончанию блока в ST(0) находится значение 7^X, а в ST(1) текущее значение X

        fstp [result + ecx * 4] ; сохраняем результат в памяти

        fld [step]
        fadd ; увеличиваем X на 0.2
        inc ecx ; увеличиваем счетчик цикла
        cmp ecx, result_size
        jne loop_start ; если прошло 5 итераций, то заканчиваем цикл
    }

    printf("4th\n");
    for (int i = 0; i < result_size; i += 1)
    {
        printf("%d: %f\n", i + 1, result[i]);
    }
    printf("\n");
}

int main()
{
    first_task();
    second_task();
    third_task();
    fourth_task();

    return 0;
}

