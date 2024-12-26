//three neurons simulating or and nand output fed into and for xor

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

float sigmoidf(float x)
{
    return 1.f / (1.f + expf(-x));
}

float rand_float(void){
    return (float) rand()/ (float) RAND_MAX;
}

typedef struct 
{
    float or_w1;
    float or_w2;
    float or_b;

    float and_w1;
    float and_w2;
    float and_b;

    float nand_w1;
    float nand_w2;
    float nand_b;

} xor ;

float forward(xor m, float x1, float x2)
{
    float a = sigmoidf(m.or_w1*x1 + m.or_w2*x2 +m.or_b);
    float b = sigmoidf(m.nand_w1*x1 + m.nand_w2*x2 +m.nand_b);
    return sigmoidf(a*m.and_w1 + b*m.and_w2 + m.and_b);
}

typedef float sample[3];
// XOR-gate
sample xor_gate[] = {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0},
};

sample *train = xor_gate;
size_t train_count = 4;

float cost(xor m){
    float result = 0.0f;

        for (size_t i = 0; i < train_count; ++i) {
            float x1 = train[i][0];
            float x2 = train[i][1];
            float y = forward(m, x1, x2);
            float d = y - train[i][2];
            result += d*d;
        }

        result /= train_count;
        return result;
}

xor rand_xor(void){

    xor m;

    m.or_w1 = rand_float();
    m.or_w2 = rand_float();
    m.or_b = rand_float();

    m.and_w1 = rand_float();
    m.and_w2 = rand_float();
    m.and_b = rand_float();

    m.nand_w1 = rand_float();
    m.nand_w2 = rand_float();
    m.nand_b = rand_float();

    return m;
}

void print_xor(xor m){
    printf("or_w1 = %f\n", m.or_w1);
    printf("or_w2 = %f\n", m.or_w2);
    printf("or_b = %f\n", m.or_b);
    printf("nand_w1 = %f\n", m.nand_w1);
    printf("nand_w2 = %f\n", m.nand_w2);
    printf("nand_b = %f\n", m.nand_b);
    printf("and_w1 = %f\n", m.and_w1);
    printf("and_w2 = %f\n", m.and_w2);
    printf("and_b = %f\n", m.and_b);
}

xor finite_diff(xor m, float eps)
{
    xor g;
    float c = cost(m);
    float saved;

    saved = m.or_w1;
    m.or_w1 += eps;
    g.or_w1 = (cost(m) - c)/eps;
    m.or_w1 = saved;

    saved = m.or_w2;
    m.or_w2 += eps;
    g.or_w2 = (cost(m) - c)/eps;
    m.or_w2 = saved;

    saved = m.or_b;
    m.or_b += eps;
    g.or_b = (cost(m) - c)/eps;
    m.or_b = saved;

    saved = m.nand_w1;
    m.nand_w1 += eps;
    g.nand_w1 = (cost(m) - c)/eps;
    m.nand_w1 = saved;

    saved = m.nand_w2;
    m.nand_w2 += eps;
    g.nand_w2 = (cost(m) - c)/eps;
    m.nand_w2 = saved;

    saved = m.nand_b;
    m.nand_b += eps;
    g.nand_b = (cost(m) - c)/eps;
    m.nand_b = saved;

    saved = m.and_w1;
    m.and_w1 += eps;
    g.and_w1 = (cost(m) - c)/eps;
    m.and_w1 = saved;

    saved = m.and_w2;
    m.and_w2 += eps;
    g.and_w2 = (cost(m) - c)/eps;
    m.and_w2 = saved;

    saved = m.and_b;
    m.and_b += eps;
    g.and_b = (cost(m) - c)/eps;
    m.and_b = saved;

    return g;

}

xor learn(xor m, xor g, float rate)
{
    m.or_w1 -= rate*g.or_w1;
    m.or_w2 -= rate*g.or_w2;
    m.or_b -= rate*g.or_b;
    m.and_w1 -= rate*g.and_w1;
    m.and_w2 -= rate*g.and_w2;
    m.and_b -= rate*g.and_b;
    m.nand_w1 -= rate*g.nand_w1;
    m.nand_w2 -= rate*g.nand_w2;
    m.nand_b -= rate*g.nand_b;

    return m;
}

int main(void){
    xor m = rand_xor();
    float eps = 1e-1;
    float rate = 1e-1;

    print_xor(m);
    printf("-------------------------------------------------\n");

    for (size_t i = 0; i < 100*1000; ++i){
        xor g = finite_diff(m, eps);
        m = learn(m, g, rate);
        printf("cost = %f\n",cost(m));
    }

    printf("-------------------------------------------------\n");
    for (size_t i = 0; i < 2; ++i){
        for (size_t j = 0; j < 2; ++j){
            printf("%zu ^ %zu = %f\n", i, j, forward(m, i, j));
        }
    }

    
}