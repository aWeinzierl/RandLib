#include "NoncentralChiSquared.h"

NoncentralChiSquared::NoncentralChiSquared(double degree, double noncentrality)
{
    setParameters(degree, noncentrality);
}

std::string NoncentralChiSquared::name()
{
    return "Noncentral Chi-Squared(" + toStringWithPrecision(getDegree()) + ", "
            + toStringWithPrecision(getNoncentrality()) + ")";
}

void NoncentralChiSquared::setParameters(double degree, double noncentrality)
{
    k = degree;
    if (k <= 0)
        k = 1;

    lambda = std::max(noncentrality, 0.0);
    sqrtLambda = std::sqrt(lambda);

    if (k > 1)
        X.setParameters(0.5 * (k - 1), 0.5);
    else {
        X.setParameters(0.5 * k, 0.5);
        Y.setRate(0.5 * lambda);
    }
}

double NoncentralChiSquared::f(double x) const
{
    if (x < 0)
        return 0.0;
    double halfkm1 = 0.5 * k - 1;
    double y = RandMath::modifiedBesselFirstKind(std::sqrt(lambda * x), halfkm1);
    y *= std::pow(x / lambda, 0.5 * halfkm1);
    return 0.5 * y * std::exp(-0.5 * (x + lambda));
}

double NoncentralChiSquared::F(double x) const
{
    if (x <= 0)
        return 0.0;
    if (k >= 2) {
        return RandMath::integral([this] (double t)
        {
            return f(t);
        }, 0, x);
    }

    // TODO: find a min for bessel function
    static constexpr double minBound = 0.01;
    if (x < minBound) {
        return RandMath::integral([this] (double t)
        {
            return f(t);
        }, -MIN_POSITIVE, x);
    }

    // TODO: this value can be counted only once for fixed parameters
    double y1 = RandMath::integral([this] (double t)
    {
        return f(t);
    }, -MIN_POSITIVE, minBound);

    return y1 + RandMath::integral([this] (double t)
    {
        return f(t);
    }, minBound, x);
}

double NoncentralChiSquared::variateForDegreeEqualOne() const
{
    double y = sqrtLambda + NormalRand::standardVariate();
    return y * y;
}

double NoncentralChiSquared::variate() const
{
    double y = (k == 1) ? X.variate() : 0.0;
    return variateForDegreeEqualOne() + y;
}

void NoncentralChiSquared::sample(std::vector<double> &outputData) const
{
    if (k != 1)
        X.sample(outputData);
    else
        std::fill(outputData.begin(), outputData.end(), 0.0);
    if (k >= 1)
    {
        for (double & var : outputData)
            var += variateForDegreeEqualOne();
    }
    else
    {
        for (double & var : outputData)
            var += GammaRand::variate(Y.variate(), 0.5);
    }
}

double NoncentralChiSquared::Mean() const
{
    return k + lambda;
}

double NoncentralChiSquared::Variance() const
{
    return 2 * (k + 2 * lambda);
}

std::complex<double> NoncentralChiSquared::CF(double t) const
{
    if (t == 0)
        return std::complex<double>(1, 0);
    std::complex<double> aux(1, -2 * t);
    std::complex<double> y(0, lambda * t);
    y = std::exp(y / aux);
    return y / std::pow(aux, 0.5 * k);
}

double NoncentralChiSquared::Skewness() const
{
    double y = k + 2 * lambda;
    y = std::pow(2 / y, 1.5);
    return y * (k + 3 * lambda);
}

double NoncentralChiSquared::ExcessKurtosis() const
{
    double y = k + 2 * lambda;
    return 12 * (k + 4 * lambda) / (y * y);
}
