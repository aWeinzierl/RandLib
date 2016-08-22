#ifndef IRWINHALLRAND_H
#define IRWINHALLRAND_H

#include "UniformRand.h"

/**
 * @brief The IrwinHallRand class
 * Irwin-Hall distribution
 *
 * f(x | n) = 0.5 / (n - 1)! * sum_{k=0}^n (-1)^k * C(n,k) * (x - k) ^ (n - 1) * sgn(x - k)
 *
 * Notation: X ~ IH(n)
 *
 * Related distributions:
 * X ~ Y_1 + Y_2 + ... + Y_n, where Y_i ~ U(0,1)
 */
class RANDLIBSHARED_EXPORT IrwinHallRand : public ContinuousDistribution
{
    int n;
    UniformRand U;
    double pdfCoef, cdfCoef;
public:
    explicit IrwinHallRand(int number);

    std::string Name() const override;
    SUPPORT_TYPE SupportType() const override { return FINITE_T; }
    double MinValue() const override { return 0; }
    double MaxValue() const override { return n; }

    void SetNumber(int number);
    inline int GetNumber() const { return n; }

    double f(double x) const override;
    double F(double x) const override;
    double Variate() const override;

    double Mean() const override;
    double Variance() const override;
    double Median() const override;
    double Mode() const override;
    double Skewness() const override;
    double ExcessKurtosis() const override;

    std::complex<double> CF(double t) const override;
};

#endif // IRWINHALLRAND_H
