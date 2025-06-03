#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define EPSILON 1.0e-16

void calc(char *value1, char *op, char *value2) {
    double v1 = atof(value1);
    double v2 = atof(value2);
    double res;

    if (strcmp(op, "+") == 0)
        res = v1 + v2;
    else if (strcmp(op, "-") == 0)
        res = v1 - v2;
    else if (strcmp(op, "*") == 0)
        res = v1 * v2;
    else if (strcmp(op, "/") == 0) {
        if (fabs(v2) < EPSILON) {
            printf("Divisão por zero\n");
            return;
        }
        res = v1 / v2;
    } else if (strcmp(op, "^") == 0)
        res = pow(v1, v2);
    else {
        printf("Operador inválido\n");
        return;
    }

    printf("Resultado calc %.3f %s %.3f = %.3f\n", v1, op, v2, res);
}

void bits(char *op1, char *op, char *op2) {
    unsigned short a = (unsigned short)atoi(op1);
    unsigned short b = (unsigned short)atoi(op2);
    unsigned short res;

    if (strcmp(op, "&") == 0)
        res = a & b;
    else if (strcmp(op, "|") == 0)
        res = a | b;
    else if (strcmp(op, "^") == 0)
        res = a ^ b;
    else {
        printf("Operador inválido\n");
        return;
    }

    printf("Resultado bits %d %s %d = %d\n", a, op, b, res);
}

