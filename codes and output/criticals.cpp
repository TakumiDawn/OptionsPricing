/**
 * critical_price.cpp
 * Purpose: find the critical stock price
 * 
 */

#include <iostream>
#include <cmath>
#include <cfloat>

#include "main.h"
#include "binomial.h"

using namespace std;

double getNew_s_star(OptionType option, double s_star, double K, double diff)
{
    double new_s = s_star;
    if (diff < 0.005)
    {
        if (option == P)
        {
            // For puts, S*(i) is the largest s_t(difference < 0.005)
            new_s = (K > s_star) ? K : s_star;
        }
        else if (option == C)
        {
            // For calls, S*(i) is the smallest s_t(difference < 0.005)
            new_s = (K < s_star) ? K : s_star;
        }
    }
    return new_s;
}

double getCriticalPrice(paramType &p, double optionPrice)
{
    if (p.exercise == E)
    {
        cout << "ERROR: We only find critical price for American options." << endl;
        return -1;
    }

    // Get u, d
    double delta, u, d;
    delta = p.t / (double)p.n;
    if (p.ifUandD == true)
    {
        u = p.u;
        d = p.d;
    }
    else
    {
        u = exp(p.sigma * sqrt(delta));
        d = exp(-p.sigma * sqrt(delta));
    }

    double K, intrinsicValue, s_star, diff;

    if (p.option == C)
        s_star = DBL_MAX;
    else if (p.option == P)
        s_star = 0;

    // We are supposed to vary the number of time steps 'n'
    for (double i = 0; i <= p.n; i++)
    {
        for (double j = 0; j <= i; j++)
        {
            K = pow(u, i - j) * pow(d, j) * p.s0;
            intrinsicValue = getPayoff(p.option, K, p.k);
            diff = abs(optionPrice - intrinsicValue);
            s_star = getNew_s_star(p.option, s_star, K, diff);
        }
    }
    return s_star;
}
