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
        ; f(n) = 418 / (2*n^2 + 7.3)
        finit
        mov ecx, 0
        fld1
        fld [zero]

        loop_start:
        fld ST(1)
        fmul ST(0), ST(0)
        fmul [b]
        fadd [c]
        fdivr [a]
        fadd
        fst [result + ecx * 4]

        fld1
        fadd ST(2), ST(0)
        fstp ST(0)
        inc ecx
        cmp ecx, result_size
        jne loop_start
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
    float a = 20;
    float b = 2.5;
    float c = 0.2;
    float result;

    __asm
    {
        ; f(n) = 20 / (x ^ 2 + 2.5 ^ x)
        finit
        fld1 ; initialize X to 1

        loop_start: ; at the start of the loop there should an up to date value of X in ST(0)

        fld ST(0) ; load X which is in ST(1) into ST(0)

        fld [b]
        fyl2x
        fld1
        fld st(1)
        fprem
        f2xm1
        fadd
        fscale
        fxch st(1)
        fstp st ; calculate 2.5 ^ x, the result is in ST(0), current value of X is still in ST(1)

        fld ST(1) ; load X which is in ST(1) into ST(0)

        fmul ST(0), ST(0) ; x ^ 2
        fadd ; add the two operands

        fdivr [a]

        fld [c]
        fcomip ST(0), ST(1)
        jb loop_end ; if the result, which is in ST(0) after fcomip, is less than 0.2, then we stop the loop

        fstp ST(0) ; drop the calculation since it is no longer needed
        fld1
        fadd
        jmp loop_start ; increment X which is in ST(0) after faddp and continue looping

        loop_end:
        fstp [result] ; write out the result
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
        ; 3 * arccsc(4*A + tan(40°))
        finit
        fld [a]
        fld [b]
        fmulp ST(1), ST(0) ; 4 * A

        fld [c]
        fld [d]
        fdivp ST(1), ST(0)
        fldpi
        fmulp ST(1), ST(0) ; convert 40 degrees to radians
        fptan
        fstp ST(0)

        fadd

        ; acsc(x) = atan(sqrt(1/(x*x-1)))
        fld ST(0) ; Compute x*x
        fmul
        fld1 ; Compute x*x-1
        fsub
        fld1 ; Compute 1/(x*x-1)
        fdivr
        fsqrt ; Compute sqrt(1/(x*x-1))
        fld1
        fpatan ; Compute atan of above.

        fld [f]
        fmulp ST(1), ST(0)

        fstp [result]
    }

    printf("3rd: %f\n\n", result);
}

void fourth_task()
{
    float initial_value = 0.5;
    float step = 0.2;
    float a = 7;
    const int result_size = 5;
    float result[result_size];

    __asm
    {
        ; Y = 7^x
        finit
        mov ecx, 0
        fld [initial_value]

        loop_start:

        fld ST(0)
        fld [a]
        fyl2x
        fld1
        fld ST(1)
        fprem
        f2xm1
        fadd
        fscale
        fxch ST(1)
        fstp ST(0) ; calculate 2.5 ^ x, the result is in ST(0), current value of X is still in ST(1)

        fstp [result + ecx * 4]

        inc ecx
        fld [step]
        fadd
        cmp ecx, result_size
        jne loop_start
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

