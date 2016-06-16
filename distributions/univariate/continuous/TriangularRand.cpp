#include "TriangularRand.h"
#include "UniformRand.h"

TriangularRand::TriangularRand(double lowerLimit, double mode, double upperLimit)
{
    setParameters(lowerLimit, mode, upperLimit);
}

std::string TriangularRand::name() const
{
    return "Triangular("
            + toStringWithPrecision(getLowerLimit()) + ", "
            + toStringWithPrecision(getMode()) + ", "
            + toStringWithPrecision(getUpperLimit()) + ")";
}

void TriangularRand::setParameters(double lowerLimit, double mode, double upperLimit)
{
    a = lowerLimit;
    if (mode > a)
        c = mode;
    else
        c = a + 1.0;
    if (upperLimit > c)
        b = upperLimit;
    else
        b = c + 1.0;
    setConstantsForGenerator();
}

void TriangularRand::setConstantsForGenerator()
{
    constForGenerator = (c - a) / (b - a);
    coefGenerator1 = (b - a) * (c - a);
    coefGenerator2 = (b - a) * (b - c);
}

double TriangularRand::f(double x) const
{
    if (x <= a)
        return 0;
    if (x < c)
        return 2.0 * (x - a) / ((b - a) * (c - a));
    if (x == c)
        return 2.0 / (b - a);
    if (x < b)
        return 2.0 * (b - x) / ((b - a) * (b - c));
    return 0;
}

double TriangularRand::F(double x) const
{
    if (x <= a)
        return 0.0;
    if (x <= c)
        return (x - a) * (x - a) / ((b - a) * (c - a));
    if (x < b)
        return 1.0 - (b - x) * (b - x) / ((b - a) * (b - c));
    return 1.0;
}

double TriangularRand::variate() const
{
    double u = UniformRand::standardVariate();
    if (u < constForGenerator)
        return a + std::sqrt(u * coefGenerator1);
    return b - std::sqrt((1 - u) * coefGenerator2);
}

double TriangularRand::Mean() const
{
    return (a + b + c) / 3.0;
}

double TriangularRand::Variance() const
{
    return (a * (a - b) + b * (b - c) + c * (c - a)) / 18.0;
}

std::complex<double> TriangularRand::CF(double t) const
{
    if (t == 0)
        return std::complex<double>(1, 0);
    double bmc = b - c, bma = b - a, cma = c - a;
    double at = a * t, bt = b * t, ct = c * t;
    std::complex<double> x(bmc * std::cos(at), bmc * std::sin(at));
    std::complex<double> y(bma * std::cos(ct), bma * std::sin(ct));
    std::complex<double> z(cma * std::cos(bt), cma * std::sin(bt));
    std::complex<double> numerator = x - y + z;
    /// in order to avoid numerical errors
    if (std::fabs(t) < 1e-10 && std::fabs(numerator.real()) < 1e-10)
        return std::complex<double>(1, 0);
    double denominator = bma * cma * bmc * t * t;
    std::complex<double> frac = -numerator / denominator;
    return frac + frac;
}

double TriangularRand::Median() const
{
    if (c + c > a + b)
        return a + std::sqrt(0.5 * (b - a) * (c - a));
    return b - std::sqrt(0.5 * (b - a) * (b - c));
}

double TriangularRand::Mode() const
{
    return c;
}

double TriangularRand::Skewness() const
{
    double numerator = M_SQRT2;
    numerator *= (a + b - c - c);
    numerator *= (a + a - b - c);
    numerator *= (a - b - b + c);
    double denominator = a * (a - b);
    denominator += b * (b - c);
    denominator += c * (c - a);
    denominator *= std::sqrt(denominator);
    return 0.2 * numerator / denominator;
}

double TriangularRand::ExcessKurtosis() const
{
    return -0.6;
}
