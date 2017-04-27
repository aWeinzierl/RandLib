#ifndef LEVYRAND_H
#define LEVYRAND_H

#include "StableRand.h"

/**
 * @brief The LevyRand class
 * Levy distribution
 *
 * f(x | μ, σ) = ((σ exp(σ / (μ - x)) / (2 π (x - μ)^3))^(1/2)
 *
 * Notation: X ~ Levy(μ, σ)
 *
 * Related distributions:
 * If X ~ Levy(0, 1), then μ + σ * X ~ Levy(μ, σ)
 * X ~ S(0.5, 1, σ, μ)
 * If Y ~ Normal(0, 1), then 1 / X^2 ~ Levy(0, 1)
 */
class RANDLIBSHARED_EXPORT LevyRand : public StableDistribution
{
public:
    LevyRand(double location = 0, double scale = 1);
    std::string Name() const override;

public:
    double f(const double & x) const override;
    double logf(const double & x) const override;
    double F(const double & x) const override;
    double S(const double & x) const override;
    double Variate() const override;

    static double Variate(double location, double scale);
    static double StandardVariate();

private:
    double quantileImpl(double p) const override;
    double quantileImpl1m(double p) const override;

    std::complex<double> CFImpl(double t) const override;

public:
    /// Maximum likelihood estimators
    void FitScaleMLE(const std::vector<double> &sample);
};

#endif // LEVYRAND_H
