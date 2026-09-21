#pragma once
#include "common.hpp"

namespace cp {
/// Modulus throughout this file is a positive signed int; products of normalized
/// residues therefore fit i64. This is NOT a 64-bit-modulus multiplication library.
/// Normalize negative residues: TC/SC O(1).
inline i64 normalize(i64 value, int mod) {
    assert(mod >= 1);
    value %= mod;
    return value < 0 ? value + mod : value;
}

/// Binary exponentiation: TC O(log(exponent+1)), SC O(1).
inline i64 mod_pow(i64 base, i64 exponent, int mod = MOD) {
    assert(exponent >= 0 && mod >= 1);
    base = normalize(base, mod);
    i64 result = 1 % mod;
    while (exponent) {
        if (exponent & 1) result = result * base % mod;
        base = base * base % mod;
        exponent >>= 1;
    }
    return result;
}

struct Bezout { i64 gcd, x, y; };
/// a,b >= 0, not both 0. Returns a*x+b*y=gcd(a,b).
/// TC O(log(min(a,b)+1)), SC O(1); intermediate coefficients must fit i64.
inline Bezout extended_gcd(i64 a, i64 b) {
    assert(a >= 0 && b >= 0 && (a || b));
    i64 x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    while (b) {
        i64 q = a / b, rem = a % b;
        a = b; b = rem;
        i64 x2 = x0 - q * x1, y2 = y0 - q * y1;
        x0 = x1; x1 = x2; y0 = y1; y1 = y2;
    }
    return {a, x0, y0};
}

/// Works for composite moduli too. No inverse when gcd(a,mod) != 1.
/// Require mod >= 2. TC O(log mod), SC O(1).
inline std::optional<i64> mod_inverse(i64 a, int mod = MOD) {
    assert(mod >= 2);
    auto result = extended_gcd(normalize(a, mod), mod);
    if (result.gcd != 1) return std::nullopt;
    return normalize(result.x, mod);
}

/// @brief Precomputed binomial coefficients modulo a caller-supplied prime.
/// Requires limit < prime. Build TC O(limit+log prime), space O(limit); query O(1).
class Combinations {
    int mod;
    std::vector<i64> factorial, inverse_factorial;
public:
    /// REQUIRES a PRIME modulus and 0 <= limit < prime; primality is caller's
    /// responsibility. Build TC O(limit+log prime), space O(limit).
    Combinations(int limit, int prime = MOD) : mod(prime) {
        assert(prime >= 2 && 0 <= limit && limit < prime);
        factorial.assign(limit + 1, 1);
        inverse_factorial.assign(limit + 1, 1);
        for (int i = 1; i <= limit; ++i) factorial[i] = factorial[i - 1] * i % mod;
        inverse_factorial[limit] = mod_pow(factorial[limit], mod - 2, mod);
        for (int i = limit; i > 0; --i) inverse_factorial[i - 1] = inverse_factorial[i] * i % mod;
    }
    /// TC/SC O(1); outside 0<=k<=n gives 0. n must be precomputed.
    i64 choose(int n, int k) const {
        assert(0 <= n && n < static_cast<int>(factorial.size()));
        if (k < 0 || k > n) return 0;
        return factorial[n] * inverse_factorial[k] % mod * inverse_factorial[n - k] % mod;
    }
};

/// @brief Linear-time smallest-prime-factor sieve for bounded factorization.
/// Build TC/space O(limit); factorize(x) takes O(log x) divisions.
class LinearSieve {
public:
    std::vector<int> smallest_prime, primes;
    /// TC/space O(n). Each composite is generated once by its smallest prime.
    explicit LinearSieve(int n) : smallest_prime(n + 1) {
        for (int x = 2; x <= n; ++x) {
            if (!smallest_prime[x]) { smallest_prime[x] = x; primes.push_back(x); }
            for (int p : primes) {
                if (p > smallest_prime[x] || 1LL * p * x > n) break;
                smallest_prime[p * x] = p;
            }
        }
    }
    /// 1 <= x <= sieve limit. TC O(log x), SC O(1) excluding output.
    std::vector<std::pair<int, int>> factorize(int x) const {
        assert(1 <= x && x < static_cast<int>(smallest_prime.size()));
        std::vector<std::pair<int, int>> answer;
        while (x > 1) {
            int p = smallest_prime[x], count = 0;
            do { x /= p; ++count; } while (x > 1 && smallest_prime[x] == p);
            answer.emplace_back(p, count);
        }
        return answer;
    }
};

/// Trial division, x >= 1. TC O(sqrt(x)), SC O(1) excluding output.
/// Use the sieve for many small queries; this is not suitable for huge primes.
inline std::vector<std::pair<i64, int>> trial_factorize(i64 x) {
    assert(x >= 1);
    std::vector<std::pair<i64, int>> answer;
    for (i64 p = 2; p <= x / p; ++p) if (x % p == 0) {
        int count = 0;
        do { x /= p; ++count; } while (x % p == 0);
        answer.emplace_back(p, count);
    }
    if (x > 1) answer.emplace_back(x, 1);
    return answer;
}

/// Euler phi(n), n >= 1: number of integers in [1,n] coprime to n.
/// TC O(sqrt(n)), SC O(log n) factor list.
inline i64 totient(i64 n) {
    assert(n >= 1);
    i64 result = n;
    for (auto [prime, exponent] : trial_factorize(n)) {
        (void)exponent;
        result = result / prime * (prime - 1);
    }
    return result;
}

/// Sorted divisors of positive n. TC O(sqrt(n)), SC O(d) temporary upper half,
/// plus O(d) output, where d is number of divisors. No sorting needed.
inline std::vector<i64> divisors(i64 n) {
    assert(n > 0);
    std::vector<i64> low, high;
    for (i64 d = 1; d <= n / d; ++d) if (n % d == 0) {
        low.push_back(d);
        if (d != n / d) high.push_back(n / d);
    }
    low.insert(low.end(), high.rbegin(), high.rend());
    return low;
}

/// Safe nonnegative LCM: nullopt if it exceeds i64.
/// TC O(log(min(a,b)+1)), SC O(1).
inline std::optional<i64> checked_lcm(i64 a, i64 b) {
    assert(a >= 0 && b >= 0);
    if (!a || !b) return 0;
    a /= std::gcd(a, b);
    if (a > std::numeric_limits<i64>::max() / b) return std::nullopt;
    return a * b;
}

/// Generalized CRT for TWO congruences x=a (mod m), x=b (mod n).
/// Moduli are positive int, so their LCM and intermediate products fit i64.
/// Result = {smallest nonnegative solution, lcm}; nullopt if inconsistent.
/// TC O(log(min(m,n)+1)), SC O(1).
inline std::optional<std::pair<i64, i64>> crt_pair(i64 a, int m, i64 b, int n) {
    assert(m > 0 && n > 0);
    a = normalize(a, m); b = normalize(b, n);
    auto [g, x, y] = extended_gcd(m, n);
    (void)y;
    i64 difference = b - a;
    if (difference % g) return std::nullopt;
    i64 reduced_n = n / g;
    i64 k = ((difference / g) * x) % reduced_n;
    if (k < 0) k += reduced_n;
    i64 lcm = (m / g) * n;
    return std::pair<i64, i64>{(a + m * k) % lcm, lcm};
}

using Matrix = std::vector<std::vector<i64>>;
/// Square matrices, same dimension, entries already in [0,mod).
/// TC O(d^3), SC O(1) excluding O(d^2) result.
inline Matrix matrix_multiply(const Matrix& a, const Matrix& b, int mod = MOD) {
    assert(mod >= 1 && a.size() == b.size());
    int n = static_cast<int>(a.size());
    for (int i = 0; i < n; ++i)
        assert(static_cast<int>(a[i].size()) == n && static_cast<int>(b[i].size()) == n);
    Matrix c(n, std::vector<i64>(n));
    for (int i = 0; i < n; ++i) for (int k = 0; k < n; ++k) {
        assert(0 <= a[i][k] && a[i][k] < mod);
        for (int j = 0; j < n; ++j) {
            assert(0 <= b[k][j] && b[k][j] < mod);
            c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % mod;
        }
    }
    return c;
}

/// Matrix exponentiation for linear recurrences. TC O(d^3 log(exponent+1)),
/// SC O(d^2) including by-value base and intermediate matrices.
inline Matrix matrix_power(Matrix base, i64 exponent, int mod = MOD) {
    assert(exponent >= 0 && mod >= 1);
    int n = static_cast<int>(base.size());
    Matrix result(n, std::vector<i64>(n));
    for (int i = 0; i < n; ++i) {
        assert(static_cast<int>(base[i].size()) == n);
        for (i64& x : base[i]) x = normalize(x, mod);
        result[i][i] = 1 % mod;
    }
    while (exponent) {
        if (exponent & 1) result = matrix_multiply(result, base, mod);
        base = matrix_multiply(base, base, mod);
        exponent >>= 1;
    }
    return result;
}
} // namespace cp
