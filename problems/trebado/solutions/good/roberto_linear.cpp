#include <bits/stdc++.h>
#ifndef _LIB_MODULAR_INTEGER
#define _LIB_MODULAR_INTEGER
#ifndef _LIB_NUMBER_THEORY
#define _LIB_NUMBER_THEORY
#include <bits/stdc++.h>

namespace lib {
using namespace std;
namespace nt {
int64_t inverse(int64_t a, int64_t b) {
  long long b0 = b, t, q;
  long long x0 = 0, x1 = 1;
  if (b == 1)
    return 1;
  while (a > 1) {
    q = a / b;
    t = b, b = a % b, a = t;
    t = x0, x0 = x1 - q * x0, x1 = t;
  }
  if (x1 < 0)
    x1 += b0;
  return x1;
}
template<typename T, typename U>
T powmod (T a, U b, U p) {
    int res = 1;
    while (b)
        if (b & 1)
            res = (int) (res * 1ll * a % p),  --b;
        else
            a = (int) (a * 1ll * a % p),  b >>= 1;
    return res;
}
template<typename T>
vector<T> factors(T n) {
  vector<T> f;
  for(T i = 2; i*i <= n; i++) {
    if(n % i == 0) f.push_back(i);
    while(n % i == 0) n /= i;
  }
  if(n > 1) f.push_back(n);
  return f;
}
} // namespace nt
} // namespace lib

#endif
#include <bits/stdc++.h>

#if __cplusplus < 201300
#error required(c++14)
#endif

namespace lib {
using namespace std;
namespace {
template <typename T, T... Mods> struct ModularIntegerBase {
  typedef ModularIntegerBase<T, Mods...> type;

  T x[sizeof...(Mods)];
  friend ostream &operator<<(ostream &output, const type &var) {
    output << "(";
    for (int i = 0; i < sizeof...(Mods); i++) {
      if (i)
        output << ", ";
      output << var.x[i];
    }
    return output << ")";
  }
};

template <typename T, T Mod> struct ModularIntegerBase<T, Mod> {
  typedef ModularIntegerBase<T, Mod> type;
  constexpr static T mod = Mod;

  T x[1];

  T& data() { return this->x[0]; }
  T data() const { return this->x[0]; }
  explicit operator int() const { return this->x[0]; }
  explicit operator int64_t() const { return this->x[0]; }
  explicit operator double() const { return this->x[0]; }
  explicit operator long double() const { return this->x[0]; }
  friend ostream &operator<<(ostream &output, const type &var) {
    return output << var.x[0];
  }
};

template<typename T, typename U, T... Mods>
struct InversesTable {
  constexpr static size_t n_mods = sizeof...(Mods);
  constexpr static T mods[sizeof...(Mods)] = {Mods...};
  constexpr static int n_inverses = 1e6 + 10;

  T v[n_inverses][n_mods];
  T max_x;

  InversesTable() : v(), max_x(n_inverses) {
    for(int j = 0; j < sizeof...(Mods); j++)
      v[1][j] = 1, max_x = min(max_x, mods[j]);
    for(int i = 2; i < max_x; i++) {
      for(int j = 0; j < sizeof...(Mods); j++) {
        v[i][j] = mods[j] - (T)((U)(mods[j] / i) * v[mods[j] % i][j] % mods[j]);
      }
    }
  }
};

// Make available for linkage.
template <typename T, class U, T... Mods>
constexpr T InversesTable<T, U, Mods...>::mods[];

template <typename T, class Enable, T... Mods>
struct ModularIntegerImpl : ModularIntegerBase<T, Mods...> {
  typedef ModularIntegerImpl<T, Enable, Mods...> type;
  typedef T type_int;
  typedef uint64_t large_int;
  constexpr static size_t n_mods = sizeof...(Mods);
  constexpr static T mods[sizeof...(Mods)] = {Mods...};
  using ModularIntegerBase<T, Mods...>::x;
  using Inverses = InversesTable<T, large_int, Mods...>;

  struct Less {
    bool operator()(const type &lhs, const type &rhs) const {
      for (size_t i = 0; i < sizeof...(Mods); i++)
        if (lhs.x[i] != rhs.x[i])
          return lhs.x[i] < rhs.x[i];
      return false;
    };
  };
  typedef Less less;


  constexpr ModularIntegerImpl() {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      x[i] = T();
  }
  constexpr ModularIntegerImpl(large_int y) {
    for (size_t i = 0; i < sizeof...(Mods); i++) {
      x[i] = y % mods[i];
      if (x[i] < 0)
        x[i] += mods[i];
    }
  }
  static type with_remainders(T y[sizeof...(Mods)]) {
    type res;
    for (size_t i = 0; i < sizeof...(Mods); i++)
      res.x[i] = y[i];
    res.normalize();
    return res;
  }

  inline void normalize() {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      if ((x[i] %= mods[i]) < 0)
        x[i] += mods[i];
  }

  inline T operator[](int i) const { return x[i]; }

  inline T multiply(T a, T b, T mod) const { return (large_int)a * b % mod; }

  inline T inv(T a, T mod) const { return static_cast<T>(nt::inverse(a, mod)); }

  inline T invi(T a, int i) const {
    const static Inverses inverses = Inverses();
    if(a < inverses.max_x)
      return inverses.v[a][i];
    return inv(a, mods[i]);
  }

  type inverse() const {
    T res[sizeof...(Mods)];
    for (size_t i = 0; i < sizeof...(Mods); i++)
      res[i] = invi(x[i], i);
    return type::with_remainders(res);
  }

  template <typename U> T power_(T a, U p, T mod) {
    if (mod == 1)
      return T();
    if (p < 0) {
      if (a == 0)
        throw domain_error("0^p with negative p is invalid");
      p = -p;
      a = inv(a, mod);
    }
    if (p == 0)
      return T(1);
    if (p == 1)
      return a;
    T res = 1;
    while (p > 0) {
      if (p & 1)
        res = multiply(res, a, mod);
      p >>= 1;
      a = multiply(a, a, mod);
    }
    return res;
  }

  inline type &operator+=(const type &rhs) {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      if ((x[i] += rhs.x[i]) >= mods[i])
        x[i] -= mods[i];
    return *this;
  }
  inline type &operator-=(const type &rhs) {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      if ((x[i] -= rhs.x[i]) < 0)
        x[i] += mods[i];
    return *this;
  }
  inline type &operator*=(const type &rhs) {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      x[i] = multiply(x[i], rhs.x[i], mods[i]);
    return *this;
  }
  inline type &operator/=(const type &rhs) {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      x[i] = multiply(x[i], invi(rhs.x[i], i), mods[i]);
    return *this;
  }

  inline type &operator+=(T rhs) {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      if ((x[i] += rhs) >= mods[i])
        x[i] -= mods[i];
    return *this;
  }

  type &operator-=(T rhs) {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      if ((x[i] -= rhs) < 0)
        x[i] += mods[i];
    return *this;
  }

  type &operator*=(T rhs) {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      x[i] = multiply(x[i], rhs, mods[i]);
    return *this;
  }

  type &operator/=(T rhs) {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      x[i] = multiply(invi(rhs, i), x[i], mods[i]);
    return *this;
  }

  type &operator^=(large_int p) {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      x[i] = power_(x[i], p, mods[i]);
    return *this;
  }

  type &operator++() {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      if ((++x[i]) >= mods[i])
        x[i] -= mods[i];
    return *this;
  }
  type &operator--() {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      if ((--x[i]) < 0)
        x[i] += mods[i];
    return *this;
  }
  type operator++(int unused) {
    type res = *this;
    ++(*this);
    return res;
  }
  type operator--(int unused) {
    type res = *this;
    --(*this);
    return res;
  }

  friend type operator+(const type &lhs, const type &rhs) {
    type res = lhs;
    return res += rhs;
  }
  friend type operator-(const type &lhs, const type &rhs) {
    type res = lhs;
    return res -= rhs;
  }
  friend type operator*(const type &lhs, const type &rhs) {
    type res = lhs;
    return res *= rhs;
  }
  friend type operator/(const type &lhs, const type &rhs) {
    type res = lhs;
    return res /= rhs;
  }

  friend type operator+(const type &lhs, T rhs) {
    type res = lhs;
    return res += rhs;
  }

  friend type operator-(const type &lhs, T rhs) {
    type res = lhs;
    return res -= rhs;
  }

  friend type operator*(const type &lhs, T rhs) {
    type res = lhs;
    return res *= rhs;
  }

  friend type operator/(const type &lhs, T rhs) {
    type res = lhs;
    return res /= rhs;
  }

  friend type operator^(const type &lhs, large_int rhs) {
    type res = lhs;
    return res ^= rhs;
  }

  friend type power(const type &lhs, large_int rhs) { return lhs ^ rhs; }

  type operator-() const {
    type res = *this;
    for (size_t i = 0; i < sizeof...(Mods); i++)
      if (res.x[i])
        res.x[i] = mods[i] - res.x[i];
    return res;
  }

  friend bool operator==(const type &lhs, const type &rhs) {
    for (size_t i = 0; i < sizeof...(Mods); i++)
      if (lhs.x[i] != rhs.x[i])
        return false;
    return true;
  }
  friend bool operator!=(const type &lhs, const type &rhs) {
    return !(lhs == rhs);
  }

  friend istream &operator>>(istream &input, type &var) {
    T y;
    cin >> y;
    var = y;
    return input;
  }
};
} // namespace

// Explicitly make constexpr available for linkage.
template <typename T, class Enable, T... Mods>
constexpr T ModularIntegerImpl<T, Enable, Mods...>::mods[];

template <typename T, T... Mods>
using ModularInteger =
    ModularIntegerImpl<T, typename enable_if<is_integral<T>::value>::type,
                       Mods...>;

template <int32_t... Mods> using Mint32 = ModularInteger<int32_t, Mods...>;

template <int64_t... Mods> using Mint64 = ModularInteger<int64_t, Mods...>;

using MintP = Mint32<(int32_t)1e9+7>;
using MintNTT = Mint32<998244353>;
} // namespace lib

#endif
#ifndef _LIB_COMBINATORICS
#define _LIB_COMBINATORICS
#include <bits/stdc++.h>
#ifndef _LIB_BIT_TRICKS
#define _LIB_BIT_TRICKS
#include <bits/stdc++.h>

namespace lib {
long long next_power_of_two(long long n) {
  if (n <= 0) return 1;
  return 1LL << (sizeof(long long) * 8 - 1 - __builtin_clzll(n) +
                 ((n & (n - 1LL)) != 0));
}
} // namespace lib

#endif

namespace lib {
using namespace std;
template<typename T>
struct Combinatorics {
    static vector<T> fat;
    static vector<T> inv;
    static vector<T> ifat;

    static T factorial(int i) {
        ensure_fat(next_power_of_two(i));
        return fat[i];
    }

    static T inverse(int i) {
        ensure_inv(next_power_of_two(i));
        return inv[i];
    }

    static T ifactorial(int i) {
        ensure_ifat(next_power_of_two(i));
        return ifat[i];
    }

    static T nCr(int n, int K) {
        if(K > n) return 0;
        ensure_fat(next_power_of_two(n));
        ensure_ifat(next_power_of_two(n));
        return fat[n] * ifat[n-K] * ifat[K];
    }

    static T arrangement(int n, int K) {
        return nCr(n, K) * factorial(n);
    }

    static T nCr_rep(int n, int K) {
        return interpolate(n - 1, K);
    }

    static T multinomial(const vector<int>& v) {
        int n = accumulate(v.begin(), v.end(), 0);
        ensure_fat(next_power_of_two(n));
        ensure_ifat(next_power_of_two(n));
        T acc = fat[n];
        for(int x : v) acc *= ifat[x];
        return acc;
    }

    static T interpolate(int a, int b) {
        return nCr(a+b, b);
    }

    static void ensure_fat(int i) {
        int o = fat.size();
        if(i < o) return;
        fat.resize(i+1);
        for(int j = o; j <= i; j++) fat[j] = fat[j-1]*j;
    }

    static void ensure_inv(int i) {
        int o = inv.size();
        if(i < o) return;
        inv.resize(i+1);
        for(int j = o; j <= i; j++) inv[j] = -(inv[T::mod%j] * (T::mod/j));
    }

    static void ensure_ifat(int i) {
        int o = ifat.size();
        if(i < o) return;
        ifat.resize(i+1);
        ensure_inv(i);
        for(int j = o; j <= i; j++) ifat[j] = ifat[j-1]*inv[j];
    }
};

template<typename T>
vector<T> Combinatorics<T>::fat = vector<T>(1, T(1));
template<typename T>
vector<T> Combinatorics<T>::inv = vector<T>(2, T(1));
template<typename T>
vector<T> Combinatorics<T>::ifat = vector<T>(1, T(1));
} // namespace lib

#endif
#ifndef _LIB_POLYNOMIAL_RING
#define _LIB_POLYNOMIAL_RING
#ifndef _LIB_EPSILON
#define _LIB_EPSILON
#include <bits/stdc++.h>

namespace lib {
using namespace std;

template <typename T = double> struct Epsilon {
  T eps;
  constexpr Epsilon(T eps = 1e-9) : eps(eps) {}

  template <typename G,
            typename enable_if<is_floating_point<G>::value>::type * = nullptr>
  int operator()(G a, G b = 0) const {
    return a + eps < b ? -1 : (b + eps < a ? 1 : 0);
  }

  template <typename G,
            typename enable_if<!is_floating_point<G>::value>::type * = nullptr>
  int operator()(G a, G b = 0) const {
    return a < b ? -1 : (a > b ? 1 : 0);
  }

  template <typename G,
            typename enable_if<is_floating_point<G>::value>::type * = nullptr>
  bool null(G a) const {
    return (*this)(a) == 0;
  }

  template <typename G,
            typename enable_if<!is_floating_point<G>::value>::type * = nullptr>
  bool null(G a) const {
    return a == 0;
  }
};
} // namespace lib

#endif
#ifndef _LIB_MATH
#define _LIB_MATH
#include <bits/stdc++.h>

namespace lib {
using namespace std;
namespace math {

/// caide keep
template <typename Type> struct DefaultPowerOp {
  Type operator()() const { return Type(1); }
  Type operator()(const Type &a) const { return a; }
  void operator()(Type &x, const Type &a, long long cur) const {
    x *= x;
    if (cur & 1)
      x *= a;
  }
};

template <typename Type, typename Op>
Type generic_power(const Type &a, long long n, Op op) {
  if (n == 0)
    return op();
  Type res = op(a);
  int hi = 63 - __builtin_clzll(n);
  for (int i = hi - 1; ~i; i--) {
    op(res, a, n >> i);
  }
  return res;
}

template <typename Type> Type generic_power(const Type &a, long long n) {
  return generic_power(a, n, DefaultPowerOp<Type>());
}
} // namespace math
} // namespace lib

#endif
#ifndef _LIB_TRAITS
#define _LIB_TRAITS
#include <bits/stdc++.h>

namespace lib {
using namespace std;
namespace traits {

template <typename...> struct make_void { using type = void; };

template <typename... T> using void_t = typename make_void<T...>::type;

/// keep caide
template <typename Iterator>
using IteratorCategory = typename iterator_traits<Iterator>::iterator_category;

/// keep caide
template <typename Container>
using IteratorCategoryOf = IteratorCategory<typename Container::iterator>;

/// keep caide
template <typename Iterator>
using IteratorValue = typename iterator_traits<Iterator>::value_type;

/// keep caide
template <typename Container>
using IteratorValueOf = IteratorValue<typename Container::iterator>;

/// keep caide
template <typename Iterator>
using IsRandomIterator =
    is_base_of<random_access_iterator_tag, IteratorCategory<Iterator>>;

/// keep caide
template <typename Iterator>
using IsInputIterator =
    is_base_of<input_iterator_tag, IteratorCategory<Iterator>>;

/// keep caide
template <typename Iterator>
using IsBidirectionalIterator =
    is_base_of<bidirectional_iterator_tag, IteratorCategory<Iterator>>;

/// keep caide
template <typename Container>
using HasRandomIterator =
    is_base_of<random_access_iterator_tag, IteratorCategoryOf<Container>>;

/// keep caide
template <typename Container>
using HasInputIterator =
    is_base_of<input_iterator_tag, IteratorCategoryOf<Container>>;

/// keep caide
template <typename Container>
using HasBidirectionalIterator =
    is_base_of<bidirectional_iterator_tag, IteratorCategoryOf<Container>>;
} // namespace traits
} // namespace lib

#endif
#ifndef _LIB_LONG_MULTIPLICATION
#define _LIB_LONG_MULTIPLICATION
#include <bits/stdc++.h>

namespace lib {
using namespace std;
namespace math {
struct NaiveMultiplication {
  template<typename T>
  using Transform = void;

  template <typename Field>
  vector<Field> operator()(const vector<Field> &a,
                           const vector<Field> &b) const {
    vector<Field> res(a.size() + b.size());
    for (size_t i = 0; i < a.size(); i++) {
      for (size_t j = 0; j < b.size(); j++) {
        res[i + j] += a[i] * b[j];
      }
    }
    return res;
  }
};

template <typename Mult, typename Field>
vector<Field> shift_conv(const vector<Field> &a, vector<Field> b) {
  if (b.empty())
    return {};
  reverse(b.begin(), b.end());
  int n = a.size();
  int m = b.size();

  auto res = Mult()(a, b);
  return vector<Field>(res.begin() + m - 1, res.end());
}
} // namespace math
} // namespace lib

#endif
#ifndef _LIB_VECTOR_N
#define _LIB_VECTOR_N
#include <bits/stdc++.h>

#define VEC_CONST_OP(op, typ) \
  type operator op(const typ rhs) const { \
    auto res = *this; \
    return res op##= rhs; \
  }

#define VEC_BIN_OP(op) \
  type& operator op##=(const type& rhs) { \
    if(rhs.size() > this->size()) \
      this->resize(rhs.size()); \
    int sz = this->size(); \
    for(int i = 0; i < (int)rhs.size(); i++) \
      (*this)[i] op##= rhs[i]; \
    for(int i = rhs.size(); i < sz; i++) \
      (*this)[i] op##= 0; \
    return *this; \
  } \
  VEC_CONST_OP(op, type)

#define VEC_SINGLE_OP(op, typ) \
  type& operator op##=(const typ rhs) { \
    for(auto& x : *this) \
      x op##= rhs; \
    return *this; \
  } \
  VEC_CONST_OP(op, typ)
  

namespace lib {
using namespace std;
template<typename T>
struct VectorN : vector<T> {
  using type = VectorN<T>;

  template <
      typename Container,
      typename enable_if<traits::HasInputIterator<Container>::value>::type * = nullptr>
  VectorN(const Container &container)
      : vector<T>(container.begin(), container.end()) {}

  VectorN(const initializer_list<T> &v)
      : vector<T>(v.begin(), v.end()) {}

  template<typename... Args>
  VectorN( Args&&... args ) 
      : vector<T>(std::forward<Args>(args)...) {}

  VEC_BIN_OP(+)
  VEC_BIN_OP(-)
  VEC_BIN_OP(*)

  VEC_SINGLE_OP(+, T&)
  VEC_SINGLE_OP(-, T&)
  VEC_SINGLE_OP(*, T&)
  VEC_SINGLE_OP(/, T&)
  VEC_SINGLE_OP(^, int64_t)

  type operator-() const {
    auto res = *this;
    for(auto& x : res) x = -x;
    return res;
  }

  type operator%(int n) const {
    // TODO: get rid of this
    // return *const_cast<type*>(this);
    return *this;
  }
};
} // namespace lib

#endif
#include <bits/stdc++.h>

namespace lib {
using namespace std;
namespace math {
namespace poly {

namespace {
/// keep caide
using traits::IsInputIterator;
/// keep caide
using traits::HasInputIterator;
} // namespace

namespace detail {
  template<class>
  struct sfinae_true : std::true_type{};

  template<class T, class Field, class Func>
  static auto test_transform(int)
      -> sfinae_true<decltype(
    std::declval<T>().template on_transform<Field>(std::declval<int>(), std::declval<Func&>()))>;

  template<class, class Field, class Func>
  static auto test_transform(long) -> std::false_type;
} // detail::

template<class T, class Field, class Func = std::function<VectorN<Field>(int)>>
struct has_transform : decltype(detail::test_transform<T, Field, Func>(0)){};

template <typename P> struct DefaultPowerOp {
  int mod;
  DefaultPowerOp(int mod) : mod(mod) {}
  P operator()() const { return P(1); }
  P operator()(const P &a) const { return a % mod; }
  void operator()(P &x, const P &a, long long cur) const {
    (x *= x) %= mod;
    if (cur & 1)
      (x *= a) %= mod;
  }
};

template <typename P> struct ModPowerOp {
  const P &mod;
  ModPowerOp(const P &p) : mod(p) {}
  P operator()() const { return P(1); }
  P operator()(const P &a) { return a % mod; }
  void operator()(P &x, const P &a, long long cur) const {
    (x *= x) %= mod;
    if (cur & 1)
      (x *= a) %= mod;
  }
};

template <typename P> struct ModShiftPowerOp {
  const P &mod;
  ModShiftPowerOp(const P &p) : mod(p) {}
  P operator()() const { return P(1); }
  P operator()(const P &a) { return a % mod; }
  void operator()(P &x, const P &a, long long cur) const {
    // if(cur < mod.degree())
    // x = P::kth(cur);
    if (cur & 1)
      (x *= (x << 1)) %= mod;
    else
      (x *= x) %= mod;
  }
};

struct DefaultDivmod;
struct NaiveDivmod;

template <typename Field, typename Mult, typename Divmod = DefaultDivmod>
struct Polynomial {
  constexpr static int Magic = 64;
  constexpr static bool NaiveMod = is_same<Divmod, NaiveDivmod>::value;
  constexpr static bool HasTransform = has_transform<Mult, Field>::value;
  using Transform =  typename Mult::template Transform<Field>;

  typedef Polynomial<Field, Mult, Divmod> type;
  typedef Field field;
  vector<Field> p;

  Polynomial() : p(0) {}
  explicit Polynomial(Field x) : p(1, x) {}

  template <
      typename Iterator,
      typename enable_if<IsInputIterator<Iterator>::value>::type * = nullptr>
  Polynomial(Iterator begin, Iterator end) : p(distance(begin, end)) {
    int i = 0;
    for (auto it = begin; it != end; ++it, ++i)
      p[i] = *it;
    normalize();
  }

  template <
      typename Container,
      typename enable_if<HasInputIterator<Container>::value>::type * = nullptr>
  Polynomial(const Container &container)
      : Polynomial(container.begin(), container.end()) {}

  Polynomial(const initializer_list<Field> &v)
      : Polynomial(v.begin(), v.end()) {}

  static type from_root(const Field &root) { return Polynomial({-root, 1}); }

  void normalize() const {
    type *self = const_cast<type *>(this);
    int sz = self->p.size();
    while (sz > 0 && Epsilon<>().null(self->p[sz - 1]))
      sz--;
    if (sz != (int)self->p.size())
      self->p.resize(sz);
  }

  inline int size() const { return p.size(); }
  inline int degree() const { return max((int)p.size() - 1, 0); }
  bool null() const {
    for (Field x : p)
      if (!Epsilon<>().null(x))
        return false;
    return true;
  }

  const vector<Field>& data() const {
    return p;
  }

  Field eval(Field x) const {
    Field pw = 1;
    Field res = 0;
    for(Field c : p) {
      res += pw * c;
      pw *= x;
    }
    return res;
  }

  inline Field operator[](const int i) const {
    if (i >= size())
      return 0;
    return p[i];
  }
  inline Field &operator[](const int i) {
    if (i >= size())
      p.resize(i + 1);
    return p[i];
  }

  Field operator()(const Field &x) const {
    if (null())
      return Field();
    Field acc = p.back();
    for (int i = (int)size() - 2; i >= 0; i--) {
      acc *= x;
      acc += p[i];
    }
    return acc;
  }

  type substr(int i, int sz) const {
    int j = min(sz + i, size());
    i = min(i, size());
    if(i >= j) return type();
    return type(begin(p)+i, begin(p)+j);
  }

  type &operator+=(const type &rhs) {
    if (rhs.size() > size())
      p.resize(rhs.size());
    int sz = size();
    for (int i = 0; i < sz; i++)
      p[i] += rhs[i];
    normalize();
    return *this;
  }

  type &operator-=(const type &rhs) {
    if (rhs.size() > size())
      p.resize(rhs.size());
    int sz = size();
    for (int i = 0; i < sz; i++)
      p[i] -= rhs[i];
    normalize();
    return *this;
  }

  static vector<Field> multiply(const vector<Field>& a, const vector<Field>& b) {
    if(min(a.size(), b.size()) < Magic)
      return NaiveMultiplication()(a, b);
    return Mult()(a, b);
  }

  type &operator*=(const type &rhs) {
    p = multiply(p, rhs.p);
    normalize();
    return *this;
  }

  type &operator*=(const Field &rhs) {
    int sz = size();
    for (int i = 0; i < sz; i++)
      p[i] *= rhs;
    normalize();
    return *this;
  }

  type &operator/=(const Field &rhs) {
    int sz = size();
    for (int i = 0; i < sz; i++)
      p[i] /= rhs;
    normalize();
    return *this;
  }

  type &operator<<=(const int rhs) {
    if (rhs < 0)
      return *this >>= rhs;
    if (rhs == 0)
      return *this;
    int sz = size();
    p.resize(sz + rhs);
    for (int i = sz - 1; i >= 0; i--)
      p[i + rhs] = p[i];
    fill_n(p.begin(), rhs, 0);
    return *this;
  }

  type &operator>>=(const int rhs) {
    if (rhs < 0)
      return *this <<= rhs;
    if (rhs == 0)
      return *this;
    int sz = size();
    if (rhs >= sz) {
      p.clear();
      return *this;
    }
    for (int i = rhs; i < sz; i++)
      p[i - rhs] = p[i];
    p.resize(sz - rhs);
    return *this;
  }

  type &operator%=(const int rhs) {
    if (rhs < size())
      p.resize(rhs);
    normalize();
    return *this;
  }

  type &operator/=(const type &rhs) { return *this = *this / rhs; }

  type operator%=(const type &rhs) { return *this = *this % rhs; }

  type operator+(const type &rhs) const {
    type res = *this;
    return res += rhs;
  }

  type operator-(const type &rhs) const {
    type res = *this;
    return res -= rhs;
  }

  type operator*(const type &rhs) const { return type(multiply(p, rhs.p)); }

  type operator*(const Field &rhs) const {
    type res = *this;
    return res *= rhs;
  }

  type operator/(const Field &rhs) const {
    type res = *this;
    return res /= rhs;
  }

  type operator<<(const int rhs) const {
    type res = *this;
    return res <<= rhs;
  }

  type operator>>(const int rhs) const {
    type res = *this;
    return res >>= rhs;
  }

  type operator%(const int rhs) const {
    return Polynomial(p.begin(), p.begin() + min(rhs, size()));
  }

  type operator/(const type &rhs) const {
    return type::divmod(*this, rhs).first;
  }

  type operator%(const type &rhs) const {
    return type::divmod(*this, rhs).second;
  }

  bool operator==(const type &rhs) const {
    normalize();
    rhs.normalize();
    return p == rhs.p;
  }

  template <// Used in SFINAE.
            typename U = Field,
            enable_if_t<has_transform<Mult, U>::value>* = nullptr>
  inline VectorN<U> transform(int n) {
    return Mult().template transform<U>(n, p);
  }

  template <// Used in SFINAE.
            typename U = Field,
            enable_if_t<has_transform<Mult, U>::value>* = nullptr>
  inline static type itransform(int n, const vector<U>& v) {
    return Mult().template itransform<U>(n, v);
  }

  template <typename Functor,
            // Used in SFINAE.
            typename U = Field,
            enable_if_t<has_transform<Mult, U>::value>* = nullptr,
            typename ...Ts>
  inline static type on_transform(
    int n,
    Functor f,        
    const Ts&... vs) {
    if(n < Magic)
      return f(n, vs...);
    return Mult().template on_transform<U>(n, f, vs.p...);
  }

  template <typename Functor,
            // Used in SFINAE.
            typename U = Field,
            enable_if_t<!has_transform<Mult, U>::value>* = nullptr,
            typename ...Ts>
  inline static type on_transform(
    int n,
    Functor f,        
    const Ts&... vs) {
    return f(n, vs...);
  }

  template <
    // Used in SFINAE.
    typename U = Field,
    enable_if_t<has_transform<Mult, U>::value>* = nullptr>
  type inverse(int m) const {
    if(null()) return *this;
    type r = {Field(1) / p[0]};
    r.p.reserve(m);
    for(int i = 1; i < m; i *= 2) {
      int n = 2 * i;
      vector<U> f = (*this % n).p; f.resize(n);
      vector<U> g = r.p; g.resize(n);
      Transform::dft(f, n);
      Transform::dft(g, n);
      for(int j = 0; j < n; j++) f[j] *= g[j];
      Transform::idft(f, n);
      for(int j = 0; j < i; j++) f[j] = 0;
      Transform::dft(f, n);
      for(int j = 0; j < n; j++) f[j] *= g[j];
      Transform::idft(f, n);
      for(int j = i; j < min(n, m); j++)
        r[j] = -f[j];
    }
    return r;
  }

  type inverse_slow(int m) const {
    if(null()) return *this;
    type b = {Field(1) / p[0]};
    b.p.reserve(2 * m);
    for(int i = 1; i < m; i *= 2) {
      int n = min(2 * i, m);
      auto bb = b * b % n;
      b += b;
      b -= *this % n * bb;
      b %= n;
    }
    return b % m;
  }

  template <
    // Used in SFINAE.
    typename U = Field,
    enable_if_t<!has_transform<Mult, U>::value>* = nullptr>
  type inverse(int m) const {
    return inverse_slow(m);
  }

  type inverse() const {
    return inverse(size());
  }

  type reciprocal() const {
    normalize();
    return type(p.rbegin(), p.rend());
  }

  type integral() const {
    int sz = size();
    if (sz == 0)
      return {};
    type res = *this;
    for (int i = sz; i; i--) {
      res[i] = res[i - 1] / i;
    }
    res[0] = 0;
    res.normalize();
    return res;
  }

  type derivative() const {
    int sz = size();
    if (sz == 0)
      return {};
    type res = *this;
    for (int i = 0; i + 1 < sz; i++) {
      res[i] = res[i + 1] * (i + 1);
    }
    res.p.back() = 0;
    res.normalize();
    return res;
  }

  type mulx(field x) const { // component-wise multiplication with x^k
    field cur = 1;
    type res(*this);
    for(auto& c : res.p)
      c *= cur, cur *= x;
    return res;
  }
  type mulx_sq(field x) const { // component-wise multiplication with x^{k^2}
    field cur = x;
    field total = 1;
    field xx = x * x;
    type res(*this);
    for(auto& c : res.p)
      c *= total, total *= cur, cur *= xx;
    return res;
  }
  static pair<type, type> divmod(const type &a, const type &b) {
    if (NaiveMod || min(a.size(), b.size()) < Magic)
      return naive_divmod(a, b);
    a.normalize();
    b.normalize();
    int m = a.size();
    int n = b.size();
    if (m < n)
      return {Polynomial(), a};
    int sz = m - n + 1;
    type ar = a.reciprocal() % sz;
    type br = b.reciprocal() % sz;
    type q = (ar * br.inverse(sz) % sz).reciprocal();
    type r = a - b * q;

    return {q, r % (n-1)};
  }

  static pair<type, type> naive_divmod(const type &a, const type &b) {
    type res = a;
    int a_deg = a.degree();
    int b_deg = b.degree();
    Field normalizer = Field(1) / b[b_deg];
    for (int i = 0; i < a_deg - b_deg + 1; i++) {
      Field coef = (res[a_deg - i] *= normalizer);
      if (coef != 0) {
        for (int j = 1; j <= b_deg; j++) {
          res[a_deg - i - j] += -b[b_deg - j] * coef;
        }
      }
    }
    return {res >> b_deg, res % b_deg};
  }
  vector<Field> czt_even(Field z, int n) const { // P(1), P(z^2), P(z^4), ..., P(z^2(n-1))
    int m = degree();
    if(null()) {
      return vector<Field>(n);
    }
    vector<Field> vv(m + n);
    Field zi = Field(1) / z;
    Field zz = zi * zi;
    Field cur = zi;
    Field total = 1;
    for(int i = 0; i <= max(n - 1, m); i++) {
      if(i <= m) {vv[m - i] = total;}
      if(i < n) {vv[m + i] = total;}
      total *= cur;
      cur *= zz;
    }
    type w = (mulx_sq(z) * vv).substr(m, n).mulx_sq(z);
    vector<Field> res(n);
    for(int i = 0; i < n; i++) {
      res[i] = w[i];
    }
    return res;
  }
  vector<Field> czt(Field z, int n) const {
    auto even = czt_even(z, (n+1)/2);
    auto odd = mulx(z).czt_even(z, n/2);
    vector<Field> ans(n);
    for(int i = 0; i < n/2; i++) {
      ans[2*i] = even[i];
      ans[2*i+1] = odd[i];
    }
    if(n&1) {
      ans.back() = even.back();
    }
    return ans;
  }
  friend type kmul(const vector<type>& polys, int l, int r) {
    if(l == r) return polys[l];
    int mid = (l+r)/2;
    return kmul(polys, l, mid) * kmul(polys, mid+1, r);
  }
  friend type kmul(const vector<type>& polys) {
    if(polys.empty()) return type();
    return kmul(polys, 0, (int)polys.size() - 1);
  }
  static type power(const type &a, long long n, const int mod) {
    return math::generic_power<type>(a, n, DefaultPowerOp<type>(mod));
  }

  static type power(const type &a, long long n, const type &mod) {
    return math::generic_power<type>(a, n, ModPowerOp<type>(mod));
  }

  static type kth(int K) { return type(1) << K; }

  static type kth(long long K, const type &mod) {
    return math::generic_power<type>(type(1) << 1, K,
                                     ModShiftPowerOp<type>(mod));
  }

  friend ostream &operator<<(ostream &output, const type &var) {
    output << "[";
    int sz = var.size();
    for (int i = sz - 1; i >= 0; i--) {
      output << var[i];
      if (i)
        output << " ";
    }
    return output << "]";
  }
};
} // namespace poly
/// keep caide
using poly::Polynomial;
} // namespace math
} // namespace lib

#endif
#ifndef _LIB_FFT
#define _LIB_FFT
#ifndef _LIB_DFT
#define _LIB_DFT
#include <bits/stdc++.h>

namespace lib {
using namespace std;
namespace linalg {
template <typename Ring, typename Provider>
struct DFT {
  static vector<int> rev;
  static vector<Ring> fa;

  // function used to precompute rev for fixed size fft (n is a power of two)
  static void dft_rev(int n) {
    Provider()(n);
    int lbn = __builtin_ctz(n);
    if ((int)rev.size() < (1 << lbn))
      rev.resize(1 << lbn);
    int h = -1;
    for (int i = 1; i < n; i++) {
      if ((i & (i - 1)) == 0)
        h++;
      rev[i] = rev[i ^ (1 << h)] | (1 << (lbn - h - 1));
    }
  }

  static void dft_iter(Ring *p, int n) {
    Provider w;
    for (int L = 2; L <= n; L <<= 1) {
      for (int i = 0; i < n; i += L) {
        for (int j = 0; j < L / 2; j++) {
          Ring z = p[i + j + L / 2] * w[j + L / 2];
          p[i + j + L / 2] = p[i + j] - z;
          p[i + j] += z;
        }
      }
    }
  }

  static void swap(vector<Ring> &buf) { std::swap(fa, buf); }
  static void _dft(Ring *p, int n) {
    dft_rev(n);
    for (int i = 0; i < n; i++)
      if (i < rev[i])
        std::swap(p[i], p[rev[i]]);
    dft_iter(p, n);
  }
  static void _idft(Ring *p, int n) {
    _dft(p, n);
    reverse(p + 1, p + n);
    Ring inv = Provider().inverse(n);
    for (int i = 0; i < n; i++)
      p[i] *= inv;
  }

  static void dft(int n) { _dft(fa.data(), n); }

  static void idft(int n) { _idft(fa.data(), n); }

  static void dft(vector<Ring> &v, int n) {
    swap(v);
    dft(n);
    swap(v);
  }
  static void idft(vector<Ring> &v, int n) {
    swap(v);
    idft(n);
    swap(v);
  }

  static int ensure(int a, int b = 0) {
    int n = a+b;
    n = next_power_of_two(n);
    if ((int)fa.size() < n)
      fa.resize(n);
    return n;
  }

  static void clear(int n) { fill(fa.begin(), fa.begin() + n, 0); }

  template<typename Iterator>
  static void fill(Iterator begin, Iterator end) {
    int n = ensure(distance(begin, end));
    int i = 0;
    for(auto it = begin; it != end; ++it) {
      fa[i++] = *it;
    }
    for(;i < n; i++) fa[i] = Ring();
  }
};

template<typename DF, typename U>
static vector<U> retrieve(int n) {
  assert(n <= DF::fa.size());
  vector<U> res(n);
  for(int i = 0; i < n; i++) res[i] = (U)DF::fa[i];
  return res;
}

template<typename Ring, typename Provider>
vector<int> DFT<Ring, Provider>::rev = vector<int>();

template<typename Ring, typename Provider>
vector<Ring> DFT<Ring, Provider>::fa = vector<Ring>();
}
} // namespace lib

#endif
#ifndef _LIB_COMPLEX
#define _LIB_COMPLEX
#include <bits/stdc++.h>

namespace lib {
using namespace std;
template <typename T> struct Complex {
  T re, im;
  Complex(T a = T(), T b = T()) : re(a), im(b) {}
  T real() const { return re; }
  T imag() const { return im; }
  explicit operator T() const { return re; }
  template<typename G>
  operator Complex<G>() const { return Complex<G>(re, im); }
  Complex conj() const { return Complex(re, -im); }
  void operator+=(const Complex<T> &rhs) { re += rhs.re, im += rhs.im; }
  void operator-=(const Complex<T> &rhs) { re -= rhs.re, im -= rhs.im; }
  void operator*=(const Complex<T> &rhs) {
    tie(re, im) =
        make_pair(re * rhs.re - im * rhs.im, re * rhs.im + im * rhs.re);
  }
  Complex<T> operator+(const Complex<T> &rhs) {
    Complex<T> res = *this;
    res += rhs;
    return res;
  }
  Complex<T> operator-(const Complex<T> &rhs) {
    Complex<T> res = *this;
    res -= rhs;
    return res;
  }
  Complex<T> operator*(const Complex<T> &rhs) {
    Complex<T> res = *this;
    res *= rhs;
    return res;
  }
  Complex<T> operator-() const {
    return {-re, -im};
  }
  void operator/=(const T x) { re /= x, im /= x; }
};
} // namespace lib

#endif
#ifndef _LIB_TRIGONOMETRY
#define _LIB_TRIGONOMETRY
#include <bits/stdc++.h>

namespace lib {
using namespace std;
namespace geo {
namespace trig {
constexpr static long double PI = 3.141592653589793238462643383279502884197169399375105820974944l;
double cos(double x) { return ::cos(x); }
double sin(double x) { return ::sin(x); }
double asin(double x) { return ::asin(x); }
double acos(double x) { return ::acos(x); }
double atan2(double y, double x) { return ::atan2(y, x); }
long double cos(long double x) { return ::cosl(x); }
long double sin(long double x) { return ::sinl(x); }
long double asin(long double x) { return ::asinl(x); }
long double acos(long double x) { return ::acosl(x); }
long double atan2(long double y, long double x) { return ::atan2l(y, x); }
} // namespace trig
} // namespace geo
} // namespace lib

#endif
#include <bits/stdc++.h>

namespace lib {
using namespace std;
namespace linalg {

template<typename T>
struct ComplexRootProvider {
  typedef Complex<T> cd;
  typedef Complex<long double> cld;
  static vector<cd> w;
  static vector<cld> wl;

  static cld root(long double ang) {
    return cld(geo::trig::cos(ang), geo::trig::sin(ang));
  }

  cd operator()(int n, int k) {
    long double ang = 2.0l * geo::trig::PI / (n / k);
    return root(ang);
  }
  void operator()(int n) {
    n = max(n, 2);
    int k = max((int)w.size(), 2);
    if ((int)w.size() < n)
      w.resize(n), wl.resize(n);
    else
      return;
    w[0] = w[1] = cd(1.0, 0.0);
    wl[0] = wl[1] = cld(1.0, 0.0);
    for (; k < n; k *= 2) {
      long double ang = 2.0l * geo::trig::PI / (2*k);
      cld step = root(ang);
      for(int i = k; i < 2*k; i++)
        w[i] = wl[i] = (i&1) ? wl[i/2] * step : wl[i/2];
    }
  }
  cd operator[](int i) {
    return w[i];
  }
  cd inverse(int n) {
    return cd(1.0 / n, 0.0);
  }
};

template<typename T>
vector<Complex<T>> ComplexRootProvider<T>::w = vector<Complex<T>>();
template<typename T>
vector<Complex<long double>> ComplexRootProvider<T>::wl = vector<Complex<long double>>();

template<typename T = double>
struct FFT : public DFT<Complex<T>, ComplexRootProvider<T>> {
  using Parent = DFT<Complex<T>, ComplexRootProvider<T>>;
  using Parent::fa;

  template <typename U>
  static void _convolve(const vector<U> &a, const vector<U> &b) {
    typedef Complex<T> cd;
    int n = Parent::ensure(a.size(), b.size());
    for (size_t i = 0; i < (size_t)n; i++)
      fa[i] = cd(i < a.size() ? (T)a[i] : T(),
                 i < b.size() ? (T)b[i] : T());
    Parent::dft(n);
    for (int i = 0; i < n; i++)
      fa[i] *= fa[i];
    Parent::idft(n);
    for (int i = 0; i < n; i++)
      fa[i] = cd(fa[i].imag() / 2, T());
  }

  template<typename U>
  static vector<U> convolve(const vector<U>& a, const vector<U>& b) {
    int sz = (int)a.size() + b.size() - 1;
    _convolve(a, b);
    return retrieve<Parent, U>(sz);
  }

  template<typename U>
  static vector<U> convolve_rounded(const vector<U>& a, const vector<U>& b) {
    int sz = (int)a.size() + b.size() - 1;
    _convolve(a, b);
    vector<U> res(sz);
    for(int i = 0; i < sz; i++) res[i] = (U)(long long)(fa[i].real() + 0.5);
    return res;
  }

  // TODO: use separate static buffers for this function
  template <typename M>
  static vector<M> convolve_mod(const vector<M> &a, const vector<M> &b) {
    typedef typename M::type_int type_int;
    typedef typename M::large_int large_int;
    typedef Complex<T> cd;
    typedef vector<cd> vcd;

    static_assert(sizeof(M::mods) / sizeof(type_int) == 1,
                  "cant multiply with multiple mods");
    type_int base = sqrtl(M::mods[0]) + 0.5;
    M base_m = base;
    int sza = a.size();
    int szb = b.size();
    int sz = sza+szb-1;
    int n = next_power_of_two(sz);
    Parent::dft_rev(n);

    // establish buffers
    vcd fa(n), fb(n), C1(n), C2(n);

    for (int i = 0; i < n; i++)
      fa[i] = i < sza ? cd((type_int)a[i] / base, (type_int)a[i] % base) : cd();
    for (int i = 0; i < n; i++)
      fb[i] = i < szb ? cd((type_int)b[i] / base, (type_int)b[i] % base) : cd();
    Parent::dft(fa, n);
    Parent::dft(fb, n);

    for (int i = 0; i < n; i++) {
      int j = i ? n - i : 0;
      cd a1 = (fa[i] + fa[j].conj()) * cd(0.5, 0.0);
      cd a2 = (fa[i] - fa[j].conj()) * cd(0.0, -0.5);
      cd b1 = (fb[i] + fb[j].conj()) * cd(0.5, 0.0);
      cd b2 = (fb[i] - fb[j].conj()) * cd(0.0, -0.5);
      cd c11 = a1 * b1, c12 = a1 * b2;
      cd c21 = a2 * b1, c22 = a2 * b2;
      C1[j] = c11 + c12 * cd(0.0, 1.0);
      C2[j] = c21 + c22 * cd(0.0, 1.0);
    }
    Parent::idft(C1, n), Parent::idft(C2, n);

    vector<M> res(sz);
    for (int i = 0; i < sz; i++) {
      int j = i ? n - i : 0;
      M x = large_int(C1[j].real() + 0.5);
      M y1 = large_int(C1[j].imag() + 0.5);
      M y2 = large_int(C2[j].real() + 0.5);
      M z = large_int(C2[j].imag() + 0.5);
      res[i] = x * base_m * base_m + (y1 + y2) * base_m + z;
    }

    return res;
  }
};
} // namespace linalg

namespace math {
struct FastMultiplication {
  template<typename T>
  using Transform = linalg::FFT<T>;
  template <typename Field, typename U = double>
  vector<Field> operator()(const vector<Field> &a,
                           const vector<Field> &b) const {
    return linalg::FFT<U>::convolve_rounded(a, b);
  }
};

struct FFTMultiplication {
  template<typename T>
  using Transform = linalg::FFT<T>;
  template <typename Field, typename U = double>
  vector<Field> operator()(const vector<Field> &a,
                           const vector<Field> &b) const {
    return linalg::FFT<U>::convolve(a, b);
  }
};

struct SafeMultiplication {
  template<typename T>
  using Transform = linalg::FFT<T>;
  template <typename Field, typename U = double>
  vector<Field> operator()(const vector<Field> &a,
                           const vector<Field> &b) const {
    return linalg::FFT<U>::convolve_mod(a, b);
  };
};
} // namespace math
} // namespace lib

#endif
#define int long long
using namespace std;
 
#define mp make_pair
#define mt make_tuple
#define pb push_back
#define ms(v, x) memset((v), (x), sizeof(v))
#define all(v) (v).begin(), (v).end()
#define ff first
#define ss second
#define iopt ios::sync_with_stdio(false); cin.tie(0)
 
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a%b); }
int power(int x, int p, int MOD) {
    if(p == 0) return 1%MOD;
    if(p == 1) return x%MOD;
    int res = power(x, p/2, MOD);
    res = (long long)res*res%MOD;
    if(p&1) res = (long long)res*x%MOD;
    return res;
}
 
typedef pair<int, int> ii;
typedef long double LD;
typedef vector<int> vi;

using mint = lib::MintP;
using C = lib::Combinatorics<mint>;
using poly = lib::math::Polynomial<mint, lib::math::SafeMultiplication>;

poly a, b;
poly invs, invs2;

void cdq(int l, int r) {
  if (l == r) {
    if (l) {
      a[l] = b[l] - C::factorial(l) * a[l];
      a[l] *= C::ifactorial(l);
    }
    return;
  }

  int mid = (l+r)/2;
  cdq(l, mid);
  poly c = a.substr(l, mid - l + 1) * invs.substr(0, r - l + 1);
  for(int i = mid+1; i <= r; i++) {
    a[i] += c[i-l];
  }
  cdq(mid+1, r);
}

int32_t main(){
    iopt;
    int K; cin >> K;

    for(int i = 0; i <= K; i++) {
      invs[i] = C::ifactorial(i);
      if (i % 2 == 0)
        invs2[i] = C::ifactorial(i/2)^2;
    }

    b = (invs * invs2) % (K+1);
    for(int i = 0; i < b.size(); i++) {
      b[i] *= C::factorial(i);
      b[i] *= b[i];
    }

    a[0] = 1;
    cdq(0, K);
    cout << a[K]*C::factorial(K) << endl;
}
