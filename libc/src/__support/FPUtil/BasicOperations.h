//===-- Basic operations on floating point numbers --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_FPUTIL_BASICOPERATIONS_H
#define LLVM_LIBC_SRC___SUPPORT_FPUTIL_BASICOPERATIONS_H

#include "FPBits.h"

#include "FEnvImpl.h"
#include "src/__support/CPP/type_traits.h"
#include "src/__support/common.h"

namespace LIBC_NAMESPACE {
namespace fputil {

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T abs(T x) {
  return FPBits<T>(x).abs().get_val();
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fmin(T x, T y) {
  const FPBits<T> bitx(x), bity(y);

  if (bitx.is_nan()) {
    return y;
  } else if (bity.is_nan()) {
    return x;
  } else if (bitx.sign() != bity.sign()) {
    // To make sure that fmin(+0, -0) == -0 == fmin(-0, +0), whenever x and
    // y has different signs and both are not NaNs, we return the number
    // with negative sign.
    return (bitx.is_neg()) ? x : y;
  } else {
    return (x < y ? x : y);
  }
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fmax(T x, T y) {
  FPBits<T> bitx(x), bity(y);

  if (bitx.is_nan()) {
    return y;
  } else if (bity.is_nan()) {
    return x;
  } else if (bitx.sign() != bity.sign()) {
    // To make sure that fmax(+0, -0) == +0 == fmax(-0, +0), whenever x and
    // y has different signs and both are not NaNs, we return the number
    // with positive sign.
    return (bitx.is_neg() ? y : x);
  } else {
    return (x > y ? x : y);
  }
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fmaximum(T x, T y) {
  FPBits<T> bitx(x), bity(y);

  if (bitx.is_nan())
    return x;
  if (bity.is_nan())
    return y;
  if (bitx.sign() != bity.sign())
    return (bitx.is_neg() ? y : x);
  return x > y ? x : y;
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fminimum(T x, T y) {
  const FPBits<T> bitx(x), bity(y);

  if (bitx.is_nan())
    return x;
  if (bity.is_nan())
    return y;
  if (bitx.sign() != bity.sign())
    return (bitx.is_neg()) ? x : y;
  return x < y ? x : y;
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fmaximum_num(T x, T y) {
  FPBits<T> bitx(x), bity(y);
  if (bitx.is_signaling_nan() || bity.is_signaling_nan()) {
    fputil::raise_except_if_required(FE_INVALID);
    if (bitx.is_nan() && bity.is_nan())
      return FPBits<T>::quiet_nan().get_val();
  }
  if (bitx.is_nan())
    return y;
  if (bity.is_nan())
    return x;
  if (bitx.sign() != bity.sign())
    return (bitx.is_neg() ? y : x);
  return x > y ? x : y;
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fminimum_num(T x, T y) {
  FPBits<T> bitx(x), bity(y);
  if (bitx.is_signaling_nan() || bity.is_signaling_nan()) {
    fputil::raise_except_if_required(FE_INVALID);
    if (bitx.is_nan() && bity.is_nan())
      return FPBits<T>::quiet_nan().get_val();
  }
  if (bitx.is_nan())
    return y;
  if (bity.is_nan())
    return x;
  if (bitx.sign() != bity.sign())
    return (bitx.is_neg() ? x : y);
  return x < y ? x : y;
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fmaximum_mag(T x, T y) {
  FPBits<T> bitx(x), bity(y);

  if (abs(x) > abs(y))
    return x;
  if (abs(y) > abs(x))
    return y;
  return fmaximum(x, y);
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fminimum_mag(T x, T y) {
  FPBits<T> bitx(x), bity(y);

  if (abs(x) < abs(y))
    return x;
  if (abs(y) < abs(x))
    return y;
  return fminimum(x, y);
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fmaximum_mag_num(T x, T y) {
  FPBits<T> bitx(x), bity(y);

  if (abs(x) > abs(y))
    return x;
  if (abs(y) > abs(x))
    return y;
  return fmaximum_num(x, y);
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fminimum_mag_num(T x, T y) {
  FPBits<T> bitx(x), bity(y);

  if (abs(x) < abs(y))
    return x;
  if (abs(y) < abs(x))
    return y;
  return fminimum_num(x, y);
}

template <typename T, cpp::enable_if_t<cpp::is_floating_point_v<T>, int> = 0>
LIBC_INLINE T fdim(T x, T y) {
  FPBits<T> bitx(x), bity(y);

  if (bitx.is_nan()) {
    return x;
  }

  if (bity.is_nan()) {
    return y;
  }

  return (x > y ? x - y : 0);
}

} // namespace fputil
} // namespace LIBC_NAMESPACE

#endif // LLVM_LIBC_SRC___SUPPORT_FPUTIL_BASICOPERATIONS_H
