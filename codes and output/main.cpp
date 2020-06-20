/**
 * main.cpp
 * Purpose: run the codes for 4 questions, one at each time chosen by the user.
 *
 * @author Takumi Li
 * @date 4/21/2020
 * @version 1.1
 */

#include <iostream>
#include <fstream>
#include <cfloat>
#include <string>
#include "main.h"
#include "binomial.h"
#include "black_scholes.h"
#include "criticals.h"


using namespace std;

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        cout << "Enter arugument 1, 2, 3 or 4 after './output' to choose the target question to run." << endl;
    }
    else
    {
        if (argv[1][0] == '1')
        {
            cout << "For question 1, please edit 'question1()' in main.cpp for the arguments of Binomial model." << endl;
            question1();
        }
        else if (argv[1][0] == '2')
            question2();
        else if (argv[1][0] == '3')
            question3();
        else if (argv[1][0] == '4')
            question4();
        else
        {
            cout << "Error in the arugument. Please only enter 1, 2, 3 or 4." << endl;
        }
    }

    return 0;
}

/**
 * Calculate the payoff of the option
 *
 * @param option: if the option is CALL or PUT
 * @param st: stock price at time T
 * @param k: strick price
 * @return the payoff of the option
 */
double getPayoff(OptionType option, double st, double k)
{
    if (option == C)
        return (0 < (st - k)) ? (st - k) : 0;
    else
        return (0 < (k - st)) ? (k - st) : 0;
}

/** QUESTION 1
 *
 * Implement the CRR binomial model to price European and American puts and calls
 * on a stock paying continuous dividend yield.
 *
 * @return The calculated option price with binomial model
 */
double question1()
{
    /* Edit parameters HERE */
    paramType params;
    params.option = P;
    params.k = 100;
    params.t = 1.0;
    params.s0 = 100;
    params.sigma = 0.25;
    params.r = 0.02;
    params.q = 0.01;
    params.n = 100;
    params.exercise = A;
    params.ifUandD = false;

    BinomialReturnType bionomialResult;
    bionomialResult = binomial(params);
    cout << "Option price = " << bionomialResult.optionPrice << endl;
    cout << "computational Time = " << bionomialResult.computationalTime << "s" << endl;
    return bionomialResult.optionPrice;
}

/** QUESTION 2
 *
 * Use the Black-Scholes formula to compute the price of the call option.
 * Verify that the binomial option prices converge to the Black-Scholes option price as N increases.
 *
 * @return The calculated option price with black-scholes formula
 */
double question2()
{
    // Set parameters as shown in the question
    paramType params;
    params.option = C;
    params.exercise = E;
    params.ifUandD = false;
    params.k = 100;
    params.t = 1.0;
    params.s0 = 100;
    params.sigma = 0.2;
    params.r = 0.05;
    params.q = 0.04;
    params.n = 10; // increasing

    ofstream outfile;
    outfile.open("q2_result.csv");

    double price_BlackScholes = black_scholes(params);
    cout << "The price of the European call using Black-Scholes formula: " << price_BlackScholes << endl;

    int N_lower = 100;
    int N_upper = 15000;

    cout << "Calculating the table for binomial model, N from " << N_lower << " to " << N_upper << " ..." << endl;

    // Calculating the table for binomial model with various N
    outfile << "N, Price" << endl;
    double binomialResult;
    for (int N = N_lower; N <= N_upper; N += 100)
    {
        params.n = N;
        binomialResult = binomial(params).optionPrice;
        outfile << N << "," << binomialResult << endl;
    }

    cout << "Binomial option prices are in 'q2_result.csv' now." << endl;
    outfile.close();

    return price_BlackScholes;
}

/*
 * Helper funtion for question 3 and 4
 *
 */
void getOptionPrice(paramType &params)
{
    ofstream outfile;
    if (params.option == P)
    { //For question 3, put options
        if (params.q == 0.00)
            outfile.open("q3_000_option_prices.csv");
        else if (params.q == 0.04)
            outfile.open("q3_004_option_prices.csv");
    }
    else if (params.option == C)
    { //For question 4, call options
        if (params.q == 0.04)
            outfile.open("q4_004_option_prices.csv");
        else if (params.q == 0.08)
            outfile.open("q4_008_option_prices.csv");
    }

    double binomialResult;
    outfile << "S0,Option Price" << endl;
    for (int s0 = 50; s0 <= 150; s0 += 1)
    {
        params.s0 = s0;
        binomialResult = binomial(params).optionPrice;
        outfile << s0 << "," << binomialResult << endl;
    }

    outfile.close();
}

/** QUESTION 3
 *
 * American puts:
 * Consider American puts with K = 100, σ = 0.2, r = 0.05, q = 0.
 * For time to maturity varying from 1 month to 12 months,
 * investigate the number of time steps needed to achieve the required 10^−3 accuracy.
 * Calculate and plot the price of a 12-month put as a function of S0.
 *
 * @return N: the determined number of time steps needed to achieve the required 10^−3 accuracy
 */
int question3()
{
    // Set parameters as shown in the question
    paramType params;
    params.option = P;
    params.exercise = A;
    params.ifUandD = false;
    params.k = 100;
    params.s0 = 100;
    params.sigma = 0.2;
    params.r = 0.05;
    params.n = 1000; // Test and set

    //For q = 0 ------------------------------------------------
    params.q = 0.0;
    params.t = 1.0;
    getOptionPrice(params);

    // Get S*(i) for different time of maturity
    ofstream outfile;

    cout << "Outputting 'q3_000_critical.csv'..." << endl;
    outfile.open("q3_000_critical.csv");
    outfile << "S_i,critical_Price" << endl;

    cout << "q = " << params.q << endl;
    for (double i = 1; i <= 12; i += 1)
    {
        params.t = i / 12.0;
        double crit_px = getCritical_Q3(params, 50, 100);
        cout << "S*(" << (int)(params.t * 12) << ") = " << crit_px << endl;
        outfile << (int)(params.t * 12) << "," << crit_px << endl;
    }
    outfile.close();

    //Repeat for q = 0.04 ---------------------------------
    params.q = 0.04;
    params.t = 1.0;
    getOptionPrice(params);

    cout << "Outputting 'q3_004_critical.csv'..." << endl;
    outfile.open("q3_004_critical.csv");
    outfile << "S_i,critical_Price" << endl;
    cout << "q = " << params.q << endl;

    for (double i = 1; i <= 12; i += 1)
    {
        params.t = i / 12.0;
        double criticalPrice = getCritical_Q3(params, 50, 100);
        cout << "S*(" << (int)(params.t * 12) << ") = " << criticalPrice << endl;
        outfile << (int)(params.t * 12) << "," << criticalPrice << endl;
    }
    outfile.close();

    return params.n;
}

double getCritical_Q3(paramType &params, double lb, double hb)
{
    BinomialReturnType binomialResult;
    double prevCritical, currCritical;
    double lower = 0, higher = 0;

    for (double s0 = lb; s0 <= hb; s0 += 1)
    {
        params.s0 = s0;
        binomialResult = binomial(params);
        prevCritical = currCritical;
        currCritical = getCriticalPrice(params, binomialResult.optionPrice);

        if (currCritical <= 0.01)
        {
            lower = s0 - 1;
            higher = s0;
            break;
        }
    }

    for (double s0 = lower; s0 <= higher + 0.01; s0 += 0.01)
    {
        params.s0 = s0;
        binomialResult = binomial(params);
        prevCritical = currCritical;
        currCritical = getCriticalPrice(params, binomialResult.optionPrice);
        if (currCritical <= 0.01)
        {
            return prevCritical;
        }
    }
    return 0.0;
}

/** QUESTION 4
 *
 * American calls:
 * Consider American puts with K = 100, σ = 0.2, r = 0.05, q = 0.
 * For time to maturity varying from 1 month to 12 months,
 * investigate the number of time steps needed to achieve the required 10^−3 accuracy.
 * Calculate and plot the price of a 12-month put as a function of S0.
 *
 * @return N: the determined number of time steps needed to achieve the required 10^−3 accuracy
 */

int question4()
{
    // Set parameters as shown in the question
    paramType params;
    params.option = C;
    params.exercise = A;
    params.ifUandD = false;
    params.k = 100;
    params.s0 = 100;
    params.sigma = 0.2;
    params.r = 0.05;
    params.n = 1000; // Test and set

    //For q = 0.04 ---------------------------------
    params.q = 0.04;
    params.t = 1.0;
    getOptionPrice(params);

    cout << "Outputting 'q4_004_critical.csv'..." << endl;
    ofstream outfile;

    outfile.open("q4_004_critical.csv");
    outfile << "S_i,Crit_Price" << endl;

    cout << "q = " << params.q << endl;
    for (double i = 1.0; i <= 12; i += 1)
    {
        params.t = i / 12.0;
        double crit_px = getCritical_Q4(params, 100, 170);
        cout << "S*(" << (int)(params.t * 12) << ") = " << crit_px << endl;
        outfile << (int)(params.t * 12) << "," << crit_px << endl;
    }
    outfile.close();

    //Repeat for q = 0.08 ---------------------------------
    params.t = 1.0;
    params.q = 0.08;
    getOptionPrice(params);

    cout << "Outputting 'q4_008_critical.csv'..." << endl;
    outfile.open("q4_008_critical.csv");
    outfile << "S_i,Crit_Price" << endl;

    cout << "q = " << params.q << endl;
    for (double i = 1.0; i <= 12; i += 1)
    {
        params.t = i / 12.0;
        double criticalPrice = getCritical_Q4(params, 100, 170);
        cout << "S*(" << (int)(params.t * 12) << ") = " << criticalPrice << endl;
        outfile << (int)(params.t * 12) << "," << criticalPrice << endl;
    }
    outfile.close();

    return params.n;
}

double getCritical_Q4(paramType &params, double lb, double hb)
{
    double binomialResult;
    double prevCritical, currCritical;
    double lower = 0, higher = 0;

    for (double s0 = hb; s0 >= lb; s0 -= 1)
    {
        params.s0 = s0;
        binomialResult = binomial(params).optionPrice;
        prevCritical = currCritical;
        currCritical = getCriticalPrice(params, binomialResult);

        if (currCritical == DBL_MAX)
        {
            lower = s0;
            higher = s0 + 1;
            break;
        }
    }

    for (double s0 = higher; s0 >= lower - 0.01; s0 -= 0.01)
    {
        params.s0 = s0;
        binomialResult = binomial(params).optionPrice;
        prevCritical = currCritical;
        currCritical = getCriticalPrice(params, binomialResult);
        if (currCritical == DBL_MAX)
        {
            return prevCritical;
        }
    }
    return 0.0;
}
