/**
 * bionomial.cpp
 * Purpose: implement the bionomial model
 * 
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;

#include "main.h"
#include "binomial.h"

/**
 * Calculate the price of the option and the computational time used  
 * 
 * @param params: parameters for the binomial model
 * paramType:
    OptionType option;     // C(call) or P(put)
    ExerciseType exercise; // A(MERICAN) or E(UROPEAN)
    double k;              // Strike price
    double t;              // Maturity
    double s0;             // Initial Stock price
    double sigma;          // Volatility
    double r;              // Risk-free interest rate
    double q;              // Dividend yield
    int n;                 // Time steps     
    
    bool ifUandD;          // If true, input values for u and d
    double u;              // Up movement in stock price
    double d;              // Down movement in stock price

 * @return result: optionPrice and computationalTime
 */
BinomialReturnType binomial(paramType &params)
{
    clock_t start = clock();
    BinomialReturnType result;

    // Get p_star
    double u, d, p_star;
    double delta = params.t / (double)params.n;
    if (params.ifUandD == true)
    {
        u = params.u;
        d = params.d;
    }
    else
    {
        u = exp(params.sigma * sqrt(delta));
        d = exp(-params.sigma * sqrt(delta));
    }
    p_star = (exp((params.r - params.q) * delta) - d) / (u - d);

    // Payoffs at maturity
    vector<double> payoffs;
    double st;
    for (int i = 0; i <= params.n; i++)
    {
        st = pow(u, params.n - i) * pow(d, i) * params.s0;
        double payoff = getPayoff(params.option, st, params.k);
        payoffs.push_back(payoff);
    }

    // Backward Induction
    double p_fu, p_fd, payoff1, payoff2;
    int j_limit;
    while (payoffs.size() > 1)
    {
        j_limit = payoffs.size() - 1;
        for (int j = 0; j < j_limit; j++)
        {
            p_fu = p_star * payoffs[j];
            p_fd = (1 - p_star) * payoffs[j + 1];

            if (params.exercise == E)
            {
                payoffs[j] = exp(-params.r * delta) * (p_fu + p_fd);
            }
            else if (params.exercise == A)
            {
                payoff1 = exp(-params.r * delta) * (p_fu + p_fd);
                st = pow(u, j_limit - 1 - j) * pow(d, j) * params.s0;
                payoff2 = getPayoff(params.option, st, params.k);
                payoffs[j] = (payoff1 < payoff2) ? payoff2 : payoff1;
            }
        }
        payoffs.pop_back(); // Remove last element
    }

    result.optionPrice = payoffs[0]; // f0 is the first item in payoffs vector now
    result.computationalTime = (clock() - start) / (double)CLOCKS_PER_SEC;
    return result;
}
