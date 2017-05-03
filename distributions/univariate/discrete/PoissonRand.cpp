#include "PoissonRand.h"
#include "../continuous/UniformRand.h"
#include "../continuous/ExponentialRand.h"

PoissonRand::PoissonRand(double rate)
{
    SetRate(rate);
}

std::string PoissonRand::Name() const
{
    return "Poisson(" + toStringWithPrecision(GetRate()) + ")";
}

void PoissonRand::SetRate(double rate)
{
    lambda = (rate <= 0.0) ? 1.0 : rate;

    logLambda = std::log(lambda);

    floorLambda = std::floor(lambda);
    FFloorLambda = F(floorLambda);
    PFloorLambda = P(floorLambda);
}

double PoissonRand::P(const int & k) const
{
    return (k < 0) ? 0.0 : std::exp(logP(k));
}

double PoissonRand::logP(const int & k) const
{
    if (k < 0)
        return -INFINITY;
    double y = k * logLambda - lambda;
    return y - RandMath::lfact(k);
}

double PoissonRand::F(const int & k) const
{
    return (k >= 0.0) ? RandMath::qgamma(k + 1, lambda, logLambda) : 0.0;
}

double PoissonRand::S(const int & k) const
{
    return (k >= 0.0) ? RandMath::pgamma(k + 1, lambda, logLambda) : 1.0;
}

int PoissonRand::Variate() const
{
    double U = UniformRand::StandardVariate();
    int k = floorLambda;
    double s = FFloorLambda, p = PFloorLambda;
    if (s < U)
    {
        do {
            ++k;
            p *= lambda / k;
            s += p;
        } while (s < U);
    }
    else
    {
        s -= p;
        while (s > U) {
            p /= lambda / k;
            --k;
            s -= p;
        }
    }
    return k;
}

int PoissonRand::Variate(double rate)
{
    int k = -1;
    double s = 0;
    do {
        s += ExponentialRand::StandardVariate();
        ++k;
    } while (s < rate);
    return k;
}

double PoissonRand::Mean() const
{
    return lambda;
}

double PoissonRand::Variance() const
{
    return lambda;
}

std::complex<double> PoissonRand::CFImpl(double t) const
{
    std::complex<double> y(std::cos(t) - 1.0, std::sin(t));
    return std::exp(lambda * y);
}

int PoissonRand::Median() const
{
    /// this value is approximate
    return std::floor(lambda + 1.0 / 3 - 0.02 / lambda);
}

int PoissonRand::Mode() const
{
    return floorLambda;
}

double PoissonRand::Skewness() const
{
    return 1.0 / std::sqrt(lambda);
}

double PoissonRand::ExcessKurtosis() const
{
    return 1.0 / lambda;
}

void PoissonRand::FitRateMLE(const std::vector<int> &sample)
{
    if (!allElementsAreNonNegative(sample))
        throw std::invalid_argument(fitError(WRONG_SAMPLE, NON_NEGATIVITY_VIOLATION));
    SetRate(sampleMean(sample));
}

GammaRand PoissonRand::FitRateBayes(const std::vector<int> &sample, const GammaDistribution &priorDistribution)
{
    if (!allElementsAreNonNegative(sample))
        throw std::invalid_argument(fitError(WRONG_SAMPLE, NON_NEGATIVITY_VIOLATION));
    double alpha = priorDistribution.GetShape();
    double beta = priorDistribution.GetRate();
    GammaRand posteriorDistribution(alpha + sampleSum(sample), beta + sample.size());
    SetRate(posteriorDistribution.Mean());
    return posteriorDistribution;
}
