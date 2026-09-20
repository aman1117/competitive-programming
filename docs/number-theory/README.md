# Number theory and algebra

[Handbook](../README.md) | [Implementation](../../include/cp/math.hpp)

**Prerequisites:** divisibility, remainders, prime factorization, and matrix multiplication.  
**Goal:** derive the algebra behind modular and combinatorial templates rather than apply formulas outside their assumptions.

## 1. Modular arithmetic and binary exponentiation

**Use when:** values are defined modulo a number or an exponent is too large for repeated multiplication.

Normalize a remainder into `[0,mod)`. In C++, a negative dividend can produce a negative remainder, so normalization may need one addition.

For exponentiation, square the base repeatedly and multiply it into the answer only when the current exponent bit is set. For `3^13 mod 17`, exponent `13=8+4+1`; the relevant powers are `16`, `13`, and `3`, giving remainder `12`.

```cpp
assert(cp::normalize(-7, 5) == 3);
assert(cp::mod_pow(3, 13, 17) == 12);
assert(cp::mod_pow(123, 0, 17) == 1);
```

**TC/SC:** normalization is `O(1)` time/space. Exponentiation is `O(log(exponent+1))` time and `O(1)` space: the exponent loses one binary digit per iteration.

These APIs accept a positive **signed-int modulus**, so multiplying two normalized residues fits `i64`. They are not safe arbitrary-64-bit-modulus multiplication routines. Exponents must be nonnegative.

## 2. Extended Euclid, inverses, and safe LCM

**Use when:** solving linear congruences or replacing modular division with multiplication by an inverse.

Euclid repeatedly replaces `(a,b)` by `(b,a mod b)`. Extended Euclid also tracks coefficients `x,y` satisfying:

```text
a*x + b*y = gcd(a,b)
```

For `30` and `18`, one solution is `x=-1`, `y=2`, giving `-30+36=6`.

```cpp
auto bezout = cp::extended_gcd(30, 18);
assert(bezout.gcd == 6 && 30 * bezout.x + 18 * bezout.y == 6);
assert(cp::mod_inverse(3, 8) == 3);
assert(!cp::mod_inverse(2, 8));
assert(cp::checked_lcm(12, 18) == 36);
```

An inverse exists exactly when the GCD is one. Reducing the Bezout identity modulo `mod` then leaves `a*x=1`. Thus `3` has inverse `3` modulo composite modulus eight, while `2` has no inverse.

LCM is `(a/gcd(a,b))*b`. Divide first and check the multiplication bound before multiplying; `checked_lcm` returns `nullopt` on overflow.

**TC/SC:** Euclidean operations take logarithmic time in the smaller operand, with constant workspace. `mod_inverse` costs `O(log mod)`. Extended-GCD inputs are nonnegative and not both zero; intermediate coefficients must fit. LCM inputs are nonnegative.

Fermat's shortcut `a^(p-2)` requires a prime modulus and a nonzero residue. Never treat ordinary integer division followed by `%` as modular division.

## 3. Factorials and modular binomial coefficients

**Use when:** many combinations `n choose k` are queried with one prime modulus.

Precompute factorials and inverse factorials:

```text
C(n,k) = factorial[n] * inverse_factorial[k] * inverse_factorial[n-k]  (mod p)
```

Only one modular exponentiation is needed: invert the final factorial, then recover earlier inverse factorials backward.

```cpp
cp::Combinations choose(20);
assert(choose.choose(5, 2) == 10);
cp::Combinations small(6, 7);
assert(small.choose(6, 3) == 6); // 20 mod 7.
assert(choose.choose(5, 9) == 0);
```

**TC/SC:** `O(N+log p)` preprocessing time, `O(N)` storage, `O(1)` per query. The modulus must be prime and `N<p`. Once a factorial contains a factor of `p`, it is zero modulo `p` and cannot be inverted this way. Primality is a caller responsibility; Lucas-style extensions are not part of this class.

## 4. Linear sieve and prime factorization

**Use when:** many primality/factorization queries are bounded by a manageable maximum.

The linear sieve stores each number's smallest prime factor. For current value `x`, combine it only with primes up to its smallest prime factor. This makes each composite appear exactly once, paired with its smallest prime.

Factoring `72` repeatedly removes its smallest prime: `72 -> 36 -> 18 -> 9 -> 3 -> 1`, producing `2^3 * 3^2`.

```cpp
cp::LinearSieve sieve(100);
auto factors = sieve.factorize(72);
assert((factors == std::vector<std::pair<int, int>>{{2, 3}, {3, 2}}));
assert(sieve.smallest_prime[97] == 97); // Prime within the sieve.
auto trial = cp::trial_factorize(84);
assert((trial == std::vector<std::pair<cp::i64, int>>{{2, 2}, {3, 1}, {7, 1}}));
```

**TC/SC:** sieve construction takes `O(N)` time/storage. A factorization within its range takes `O(log x)` divisions and `O(1)` auxiliary space excluding output. `trial_factorize` needs no precomputation but takes `O(sqrt x)` time in the worst case and constant workspace excluding output.

Trial division is unsuitable for many huge primes. In tests such as `p*p<=x`, multiplication can overflow; the implementation uses `p<=x/p`.

## 5. Divisors and Euler's totient

**Use when:** enumerating factor-based possibilities or counting numbers coprime to `n`.

Divisors come in pairs `d` and `n/d`. Scan only to the square root, collecting small divisors forward and large divisors in reverse order. A square root must be included only once.

Euler's product formula removes multiples of each distinct prime factor:

```text
phi(n) = n * product_over_distinct_primes_p(1 - 1/p)
```

For `36=2^2*3^2`, `phi(36)=36*(1/2)*(2/3)=12`.

```cpp
assert((cp::divisors(36) == std::vector<cp::i64>{1, 2, 3, 4, 6, 9, 12, 18, 36}));
assert(cp::totient(36) == 12);
assert(cp::totient(1) == 1);
```

**TC/SC:** divisors take `O(sqrt n)` time and `O(number_of_divisors)` temporary storage plus output. Totient here uses trial factorization: `O(sqrt n)` time and `O(log n)` factor-list space. Positive `n` is required.

## 6. Generalized Chinese remainder theorem

**Use when:** a number must satisfy two congruences whose moduli may share factors.

For `x=a (mod m)` and `x=b (mod n)`, write `x=a+m*k`. Then:

```text
m*k = b-a (mod n)
```

Let `g=gcd(m,n)`. A solution exists only if `g` divides `b-a`. Divide by `g`, solve the reduced coprime congruence using an inverse, and normalize the result modulo `lcm(m,n)`.

For `x=2 (mod 6)` and `x=5 (mod 9)`, reduce `6k=3 (mod 9)` to `2k=1 (mod 3)`. Thus `k=2 (mod 3)` and `x=14 (mod 18)`.

```cpp
auto result = cp::crt_pair(2, 6, 5, 9);
assert(result && result->first == 14 && result->second == 18);
assert(!cp::crt_pair(1, 2, 0, 4));
```

**TC/SC:** logarithmic Euclidean time and `O(1)` space. The result is `{smallest_nonnegative_solution,combined_modulus}`; `nullopt` means inconsistent constraints. Both input moduli must fit positive signed `int`. Do not feed an arbitrarily large combined modulus into this two-int-modulus API.

## 7. Matrix multiplication and exponentiation

**Use when:** a fixed-dimensional linear recurrence must be advanced by many steps.

Encode one transition as matrix `M`; after `k` steps the transition is `M^k`. Binary exponentiation works because matrix multiplication is associative, even though it is not generally commutative.

For Fibonacci:

```text
[F(k+1)]   [1 1] [F(k)  ]
[F(k)  ] = [1 0] [F(k-1)]
```

```cpp
cp::Matrix transition{{1, 1}, {1, 0}};
assert((cp::matrix_multiply(transition, transition) == cp::Matrix{{2, 1}, {1, 1}}));
auto power = cp::matrix_power(transition, 10);
assert(power[0][1] == 55);
assert((cp::matrix_power(transition, 0) == cp::Matrix{{1, 0}, {0, 1}}));
```

**TC/SC:** multiplying `d x d` matrices costs `O(d^3)` time and `O(d^2)` result space. Exponentiation costs `O(d^3 log(k+1))` time and `O(d^2)` workspace including copied/intermediate matrices.

`matrix_multiply` requires already-normalized residues. `matrix_power` normalizes its input first. Dimension order matters: be consistent about whether your state is a column or row vector.
