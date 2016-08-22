#ifndef EXPONENTIALNORMALRAND_H
#define EXPONENTIALNORMALRAND_H

#include "ContinuousDistribution.h"
#include "NormalRand.h"
#include "ExponentialRand.h"

/**
 * @brief The ExponentialNormalRand class
 * Exponentially modified Gaussian distribution
 *
 * Notation: X ~ EMG(μ, σ, β)
 *
 * Related distributions:
 * X = Y + Z, where Y ~ Normal(μ, σ) and Z ~ Exp(β)
 */
class RANDLIBSHARED_EXPORT ExponentialNormalRand : public ContinuousDistribution
{
    NormalRand X;
    ExponentialRand Y;

    double a, b, c, v; /// auxiliary variables

public:
    explicit ExponentialNormalRand(double location = 0, double variance = 1, double rate = 1);

    std::string Name() const override;
    SUPPORT_TYPE SupportType() const override { return INFINITE_T; }
    double MinValue() const override { return -INFINITY; }
    double MaxValue() const override { return INFINITY; }

    void SetParameters(double location, double variance, double rate);
    inline double GetLocation() const { return X.GetLocation(); }
    inline double GetScale() const { return X.GetScale(); }
    inline double GetRate() const { return Y.GetRate(); }

    double f(double x) const override;
    double F(double x) const override;
    double Variate() const override;
    static double Variate(double location, double rootVar, double rate);

    double Mean() const override;
    double Variance() const override;
    double Skewness() const override;
    double ExcessKurtosis() const override;

    std::complex<double> CF(double t) const override;
};



#endif // EXPONENTIALNORMALRAND_H
