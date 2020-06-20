/**
 * black_scholes.cpp
 * Purpose: implement the blakc sholes model
 * 
 */
#include <iostream>
#include <cmath>

#include "main.h"
#include "black_scholes.h"

using namespace std;

/*
 * Helper funciton: Normailize input
 */
double normal(double in)
{
    double nor = 0.5 + 0.5 * erf(in / sqrt(2.0));
    return nor;
}

double black_scholes(paramType &p)
{
    // Get d1 and d2
    double d1 = log(p.s0 / p.k) + p.t * (p.r - p.q + 0.5 * pow(p.sigma, 2)) / (p.sigma * sqrt(p.t));
    double d2 = d1 - p.sigma * sqrt(p.t);

    if (p.option == C)
    { // for call
        return (p.s0 * exp(-p.q * p.t) * normal(d1)) - (p.k * exp(-p.r * p.t) * normal(d2));
    }
    else if (p.option == P)
    { // for put
        return (-p.s0 * exp(-p.q * p.t) * normal(-d1)) + (p.k * exp(-p.r * p.t) * normal(-d2));
    }

    return -999.0; //ERRORs
}
