#ifndef _MAIN_H
#define _MAIN_H

enum OptionType {C, P}; // Option = C for calls and P for puts
enum ExerciseType {A, E}; // Exercise = A for American options and E for European options.

double getPayoff(OptionType option, double st, double k);

struct BinomialReturnType {
    double optionPrice;
    double computationalTime;
};

struct paramType {
    OptionType option;     // CALL or PUT
    ExerciseType exercise; // AMERICAN or EUROPEAN
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
};

double question1();
double question2();
int question3();
int question4();

void getOptionPrice(paramType &params);
double getCritical_Q3(paramType &params, double lb, double hb);
double getCritical_Q4(paramType &params, double lb, double hb);

#endif /* _MAIN_H */