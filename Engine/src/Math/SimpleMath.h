#ifndef SIMPLE_MATH_H
#define SIMPLE_MATH_H

#include "Utils/SimpleStandardLibrary.h"

//TODO(bies): remove this
#include <math.h>

/*

  Vectors:
  v2 v3 v4 - by default it's f32 vn[n]

*/

// e
#define MATH_E         2.71828182845904523536028747135266250
// log2(e)
#define MATH_LOG2E     1.44269504088896340735992468100189214
// log10(e)
#define MATH_LOG10E    0.434294481903251827651128918916605082
// loge(2)
#define MATH_LN2       0.693147180559945309417232121458176568
// loge(10)
#define MATH_LN10      2.30258509299404568401799145468436421
// pi
#define MATH_PI        3.14159265358979323846264338327950288
// pi/2
#define MATH_PI_2      1.57079632679489661923132169163975144
// pi/4
#define MATH_PI_4      0.785398163397448309615660845819875721
// 1/pi
#define MATH_1_PI      0.318309886183790671537767526745028724
// 2/pi
#define MATH_2_PI      0.636619772367581343075535053490057448
// 2/sqrt(pi)
#define MATH_2_SQRTPI  1.12837916709551257389615890312154517
// sqrt(2)
#define MATH_SQRT2     1.41421356237309504880168872420969808
// 1/sqrt(2)
#define MATH_SQRT1_2   0.707106781186547524400844362104849039

#define MATH_Ef        ((f32) MATH_E)
#define MATH_LOG2Ef    ((f32) MATH_LOG2E)
#define MATH_LOG10Ef   ((f32) MATH_LOG10E)
#define MATH_LN2f      ((f32) MATH_LN2)
#define MATH_LN10f     ((f32) MATH_LN10)
#define MATH_PIf       ((f32) MATH_PI)
#define MATH_PI_2f     ((f32) MATH_PI_2f)
#define MATH_PI_4f     ((f32) MATH_PI_4f)
#define MATH_1_PIf     ((f32) MATH_1_PI)
#define MATH_2_PIf     ((f32) MATH_2_PI)
#define MATH_2_SQRTPIf ((f32) MATH_2_SQRTPI)
#define MATH_SQRT2f    ((f32) MATH_SQRT2)
#define MATH_SQRT1_2f  ((f32) MATH_SQRT1_2)

//typedef f32 v2[2];
typedef f32 v3a[3];
typedef f32 v4a[4];
typedef v3a m3a[3];
typedef v4a m4a[4];

//#define v2_(x, y) (v2) { x, y }
//#define v3_(x, y, z) (v3) { x, y, z }
//#define v4_(x, y, z, w) (v4) { x, y, z, w }

//#define v2_wo(x, y) {x, y}
#define v3_wo(x, y, z) {x, y, z}
#define v4_wo(x, y, z, w) {x, y, z, w}

#define v4_imvec4(imvec) { imvec.x, imvec.y, imvec.z, imvec.w }

#define v2_convert(x, y) (v2) {x, y}
#define v3_convert(x, y, z) (v3) {x, y, z}
#define v4_convert(x, y, z, w) (v4) {x, y, z, w}

typedef struct v2
{
    union
    {
	struct
	{
	    f32 X;
	    f32 Y;
	};
	struct
	{
	    f32 Width;
	    f32 Height;
	};

	f32 V[2];
    };
} v2;

typedef struct v3
{
    union
    {
	struct
	{
	    f32 X;
	    f32 Y;
	    f32 Z;
	};
	struct
	{
	    f32 R;
	    f32 G;
	    f32 B;
	};
	f32 V[3];
    };
} v3;

typedef struct v4
{
    union
    {
	struct
	{
	    f32 X;
	    f32 Y;
	    f32 Z;
	    f32 W;
	};
	struct
	{
	    f32 R;
	    f32 G;
	    f32 B;
	    f32 A;
	};
	f32 V[4];
    };
} v4;

typedef struct m3
{
    union
    {
	struct
	{
	    f32 M00;
	    f32 M01;
	    f32 M02;

	    f32 M10;
	    f32 M11;
	    f32 M12;

	    f32 M20;
	    f32 M21;
	    f32 M22;
	};
	v3 V[3];
	m3a M;
    };
} m3;

//#define M4_IDENTITY				\
//    {						\
//	{ 1.0f, 0.0f, 0.0f, 0.0f },		\
//	{ 0.0f, 1.0f, 0.0f, 0.0f },		\
//	{ 0.0f, 0.0f, 1.0f, 0.0f },		\
//	{ 0.0f, 0.0f, 0.0f, 1.0f }		\
//    }

typedef struct m4
{
    union
    {
	struct
	{
	    f32 M00;
	    f32 M01;
	    f32 M02;
	    f32 M03;

	    f32 M10;
	    f32 M11;
	    f32 M12;
	    f32 M13;

	    f32 M20;
	    f32 M21;
	    f32 M22;
	    f32 M23;

	    f32 M30;
	    f32 M31;
	    f32 M32;
	    f32 M33;
	};

	v4 V[4];
	m4a M;
    };
} m4;

force_inline i32
i32_pow(i32 x, i32 n)
{
    i32 result = 1;

    if (n == 0 || x == 1)
    {
	return 1;
    }

    while (n)
    {
	result *= x;
	--n;
    }

    return result;
}
force_inline f32
pow2f(f32 x)
{
    return x * x;
}

/*
  number_rank(1000) == 3;
  TODO(bies): rename to number_get_rank(i32 number)
  TODO(bies): change this to i32_rank in refactoring session
*/
force_inline i32
number_rank(i32 number)
{
    i32 rank = 0;
    for (; ;)
    {
	number /= 10;
	if (number != 0)
	{
	    ++rank;
	}
	else
	{
	    return rank;
	}
    }
}
//TODO: rename this function
force_inline i32
number_of_rank(i32 number, i32 rank)
{
    if (rank <= 0)
    {
	return number;
    }

    for (i32 i = 0; i < rank; i++)
    {
	number *= 10;
    }

    return number;
}
force_inline i32
number_of_digit(i64 number, i8 digit)
{
    i32 i;

    if (number_rank(number) < digit)
    {
	return 0;
    }

    if (number < 0)
    {
	number *= -1;
    }

    if (digit == 0)
    {
	return (number % 10);
    }

    number %= number_of_rank(1, (digit + 1));
    number /= number_of_rank(1, digit);

    return number;
}

#define F32_CUSTOM_EPSILON 1E-5f
force_inline i32
f32_equal(f32 a, f32 b)
{
    return (fabs(a - b) <= F32_CUSTOM_EPSILON);
}
force_inline i32
f32_equal_epsilon(f32 a, f32 b, f32 eps)
{
    return (fabs(a - b) <= eps);
}
force_inline f32
rad(f32 degrees)
{
    return (MATH_PI / 180) * degrees;
}
force_inline f32
deg(f32 radians)
{
    return (180 / MATH_PI) * radians;
}
force_inline i32
ROUND_MAX(f32 value)
{
    return (i32)(value + 1);
}
force_inline i32
ROUND_MIN(f32 value)
{
    return (i32)(value - 1);
}

/*

  ###################################
  ###################################
  V2.h
  ###################################
  ###################################

*/
force_inline v2
v2_new(f32 x, f32 y)
{
    v2 v;
    v.X = x;
    v.Y = y;
    return v;
}
force_inline v2
v2_array(f32* arr)
{
    v2 v;
    v.X = arr[0];
    v.Y = arr[1];
    return v;
}

force_inline f32
v2_dot(v2 a, v2 b)
{
    return a.X * b.X + a.Y * b.Y;
}

force_inline f32
v2_length(v2 v)
{
    return sqrtf(v.X * v.X + v.Y * v.Y);
}

force_inline f32
v2_distance2(v2 a, v2 b)
{
    f32 distance = pow2f(a.X - b.X) + pow2f(a.Y - b.Y);
    return distance;
}

force_inline f32
v2_distance(v2 a, v2 b)
{
    f32 distance = sqrtf(v2_distance2(a, b));
    return distance;
}

force_inline f32
v2_distance_pow(v2 a, v2 b)
{
    f32 distance = pow2f(a.X - b.X) + pow2f(a.Y - b.Y);
    return distance;
}

force_inline v2
v2_normalize(v2 v)
{
    f32 length = v2_length(v);
    if (length == 0.0f)
    {
	return v;
    }
    v.X /= length;
    v.Y /= length;
    return v;
}

force_inline v2
v2_copy(v2 b)
{
    v2 result;
    result.X = b.X;
    result.Y = b.Y;
    return result;
}

force_inline v2
v2_add(v2 a, v2 b)
{
    v2 result;
    result.X = a.X + b.X;
    result.Y = a.Y + b.Y;
    return result;
}

force_inline v2
v2_sub(v2 a, v2 b)
{
    v2 result;
    result.X = a.X - b.X;
    result.Y = a.Y - b.Y;
    return result;
}

force_inline v2
v2_mul(v2 a, v2 b)
{
    v2 result;
    result.X = a.X * b.X;
    result.Y = a.Y * b.Y;
    return result;
}

force_inline v2
v2_div(v2 a, v2 b)
{
    v2 result;
    result.X = a.X / b.X;
    result.Y = a.Y / b.Y;
    return result;
}

force_inline v2
v2_addv(v2 a, f32 num)
{
    v2 result;
    result.X = a.X - num;
    result.Y = a.Y - num;
    return result;
}

force_inline v2
v2_subv(v2 a, f32 num)
{
    v2 result;
    result.X = a.X - num;
    result.Y = a.Y - num;
    return result;
}

force_inline v2
v2_mulv(v2 a, f32 num)
{
    v2 result;
    result.X = a.X - num;
    result.Y = a.Y - num;
    return result;
}

force_inline v2
v2_divv(v2 a, f32 num)
{
    v2 result;
    result.X = a.X - num;
    result.Y = a.Y - num;
    return result;
}

/* Convertation */
force_inline v2
v2_v4(v4 a)
{
    v2 v;
    v.X = a.X;
    v.Y = a.Y;
    return v;
}

/*

  ###################################
  ###################################
  V3.h
  ###################################
  ###################################

*/

force_inline v3
v3_new(f32 x, f32 y, f32 z)
{
    v3 v;
    v.X = x;
    v.Y = y;
    v.Z = z;
    return v;
}
force_inline v3
v3_array(f32* arr)
{
    v3 v;
    v.X = arr[0];
    v.Y = arr[1];
    v.Z = arr[2];
    return v;
}

force_inline f32
v3_length(v3 v)
{
    return sqrtf(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}
force_inline f32
v3_length_array(f32 v[3])
{
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

force_inline f32
v3_dot(v3 a, v3 b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

force_inline v3
v3_cross(v3 a, v3 b)
{
    v3 dest;
    dest.X = a.Y * b.Z - a.Z * b.Y;
    dest.Y = a.Z * b.X - a.X * b.Z;
    dest.Z = a.X * b.Y - a.Y * b.X;
    return dest;
}

force_inline v3
v3_normalize(v3 v)
{
    f32 length = v3_length(v);
    if (length == 0.0f)
    {
	return v;
    }

    v3 dest;
    dest.X = v.X / length;
    dest.Y = v.Y / length;
    dest.Z = v.Z / length;
    return dest;
}
force_inline v3
v3_cross_normalized(v3 a, v3 b)
{
    v3 cross = v3_cross(a, b);
    v3 normalizedCross = v3_normalize(cross);
    return normalizedCross;
}

force_inline f32
v3_distance(v3 a, v3 b)
{
    f32 distance = sqrtf(pow2f(a.X - b.X) + pow2f(a.Y - b.Y) + pow2f(a.Z - b.Z));
    return distance;
}

force_inline f32
v3_distance_pow(v3 a, v3 b)
{
    f32 distance = pow2f(a.X - b.X) + pow2f(a.Y - b.Y) + pow2f(a.Z - b.Z);
    return distance;
}

force_inline v3
v3_negative(v3 v)
{
    v3 dest;
    dest.X = -v.X;
    dest.Y = -v.Y;
    dest.Z = -v.Z;
    return dest;
}

force_inline v3
v3_negative_to(v3 v)
{
    v3 dest;
    dest.X = -v.X;
    dest.Y = -v.Y;
    dest.Z = -v.Z;
    return dest;
}

force_inline v3
v3_copy(v3 b)
{
    v3 dest;
    dest.X = b.X;
    dest.Y = b.Y;
    dest.Z = b.Z;
    return dest;
}

/*
  Arithmetic's operations
*/
force_inline v3
v3_add(v3 a, v3 b)
{
    v3 v;
    v.X = a.X + b.X;
    v.Y = a.Y + b.Y;
    v.Z = a.Z + b.Z;
    return v;
}

force_inline v3
v3_sub(v3 a, v3 b)
{
    v3 v;
    v.X = a.X - b.X;
    v.Y = a.Y - b.Y;
    v.Z = a.Z - b.Z;
    return v;
}

force_inline v3
v3_mul(v3 a, v3 b)
{
    v3 v;
    v.X = a.X * b.X;
    v.Y = a.Y * b.Y;
    v.Z = a.Z * b.Z;
    return v;
}
force_inline v3
v3_scale(v3 a, f32 num)
{
    v3 v;
    v.X = a.X * num;
    v.Y = a.Y * num;
    v.Z = a.Z * num;
    return v;
}

force_inline v3
v3_div(v3 a, v3 b)
{
    v3 v;
    v.X = a.X / b.X;
    v.Y = a.Y / b.Y;
    v.Z = a.Z / b.Z;
    return v;
}

force_inline v3
v3_addv(v3 b, f32 num)
{
    v3 v;
    v.X = b.X * num;
    v.Y = b.Y * num;
    v.Z = b.Z * num;
    return v;
}

force_inline v3
v3_subv(v3 b, f32 num)
{
    v3 v;
    v.X = b.X - num;
    v.Y = b.Y - num;
    v.Z = b.Z - num;
    return v;
}

force_inline v3
v3_mulv(v3 b, f32 num)
{
    v3 v;
    v.X = b.X * num;
    v.Y = b.Y * num;
    v.Z = b.Z * num;
    return v;
}
force_inline v4
v3_mulv_as_v4(v3 b, f32 num)
{
    v4 v;
    v.X = b.X * num;
    v.Y = b.Y * num;
    v.Z = b.Z * num;
    v.W = 0.0f;
    return v;
}
force_inline void
v3_mulv_ref(f32 dest[3], v3 b, f32 num)
{
    dest[0] = b.X * num;
    dest[1] = b.Y * num;
    dest[2] = b.Z * num;
}

force_inline v3
v3_divv(v3 b, f32 num)
{
    v3 v;
    v.X = b.X - num;
    v.Y = b.Y - num;
    v.Z = b.Z - num;
    return v;
}

/* Convertation */
force_inline v3
v3_v4(v4 a)
{
    v3 v;
    v.X = a.X;
    v.Y = a.Y;
    v.Z = a.Z;
    return v;
}

force_inline v3
v3_rotate(v3 v, f32 angle, v3 axis)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    v3 normalizedAxis = v3_normalize(axis);

    /* Right Hand, Rodrigues' rotation formula:
       v = v*cos(t) + (normalizedAxisxv)sin(t) + normalizedAxis*(normalizedAxis.v)(1 - cos(t))
    */
    v3 firstVector = v3_scale(v, c);
    v3 secondVector = v3_cross(normalizedAxis, v);
    secondVector = v3_scale(secondVector, s);

    firstVector = v3_add(firstVector, secondVector);

    secondVector = v3_scale(normalizedAxis, v3_dot(normalizedAxis, v) * (1.0f - c));
    v = v3_add(firstVector, secondVector);
    return v;
}

/*

  ###################################
  ###################################
  V4.h
  ###################################
  ###################################

*/

force_inline v4
v4_new(f32 x, f32 y, f32 z, f32 w)
{
    v4 v;
    v.X = x;
    v.Y = y;
    v.Z = z;
    v.W = w;
    return v;
}
force_inline v4
v4_v3(v3 src, f32 w)
{
    return v4_new(src.X, src.Y, src.Z, w);
}
force_inline v4
v4_array(f32* array)
{
    v4 v;
    v.X = array[0];
    v.Y = array[1];
    v.Z = array[2];
    v.W = array[3];
    return v;
}

force_inline i32
v4_is_direction(v4 v)
{
    return (v.W == 0) ? 1 : 0;
}

force_inline i32
v4_is_position(v4 v)
{
    return (v.W == 1) ? 1 : 0;
}

force_inline v4
v4_copy(v4 src)
{
    v4 dest;
    dest.X = src.X;
    dest.Y = src.Y;
    dest.Z = src.Z;
    dest.W = src.W;
    return dest;
}
force_inline void
v4a_copy(v4a dest, v4a src)
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
}

force_inline void
v4_assignv(v4 a, f32 x, f32 y, f32 z, f32 w)
{
    a.X = x;
    a.Y = y;
    a.Z = z;
    a.W = w;
}

force_inline f32
v4_length(v4 v)
{
    return sqrtf(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}

force_inline f32
v4_distance(v4 a, v4 b)
{
    f32 distance = sqrt(pow2f(a.X - b.X) + pow2f(a.Y - b.Y) + pow2f(a.Z - b.Z) + pow2f(a.W - b.W));
    return distance;
}

force_inline v4
v4_normalize(v4 v)
{
    v4 dest;
    f32 length = v4_length(v);
    dest.X = v.X / length;
    dest.Y = v.Y / length;
    dest.Z = v.Z / length;
    dest.W = v.W / length;
    return dest;
}

force_inline v4
v4_add(v4 a, v4 b)
{
    v4 v;
    v.X = a.X + b.X;
    v.Y = a.Y + b.Y;
    v.Z = a.Z + b.Z;
    v.W = a.W + b.W;
    return v;
}

force_inline v4
v4_addv(v4 a, f32 num)
{
    v4 v;
    v.X = a.X + num;
    v.Y = a.Y + num;
    v.Z = a.Z + num;
    v.W = a.W + num;
    return v;
}

force_inline v4
v4_sub(v4 a, v4 b)
{
    v4 v;
    v.X = a.X - b.X;
    v.Y = a.Y - b.Y;
    v.Z = a.Z - b.Z;
    v.W = a.W - b.W;
    return v;
}

force_inline v4
v4_subv(v4 a, f32 num)
{
    v4 v;
    v.X = a.X - num;
    v.Y = a.Y - num;
    v.Z = a.Z - num;
    v.W = a.W - num;
    return v;
}

force_inline v4
v4_mul(v4 a, v4 b)
{
    v4 v;
    v.X = a.X * b.X;
    v.Y = a.Y * b.Y;
    v.Z = a.Z * b.Z;
    v.W = a.W * b.W;
    return v;
}

force_inline v4
v4_mulv(v4 a, f32 num)
{
    v4 v;
    v.X = a.X * num;
    v.Y = a.Y * num;
    v.Z = a.Z * num;
    v.W = a.W * num;
    return v;
}

force_inline void
v4a_scale(f32 a[4], f32 num, f32 dest[4])
{
    dest[0] = a[0] * num;
    dest[1] = a[1] * num;
    dest[2] = a[2] * num;
    dest[3] = a[3] * num;
}
force_inline v4
v4_scale_array(f32 v[4], f32 num)
{
    v4 r;
    r.X = v[0] * num;
    r.Y = v[1] * num;
    r.Z = v[2] * num;
    r.W = v[3] * num;
    return r;
}

force_inline v4
v4_div(v4 a, v4 b)
{
    v4 v;
    v.X = a.X / b.X;
    v.Y = a.Y / b.Y;
    v.Z = a.Z / b.Z;
    v.W = a.W / b.W;
    return v;
}

force_inline f32
v4_dot(v4 a, v4 b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
}

/*

  ###################################
  ###################################
  M3.h
  ###################################
  ###################################

*/
force_inline m3
m3_array(f32* array)
{
    m3 m;
    m.M00 = array[0]; m.M01 = array[1]; m.M02 = array[2];
    m.M10 = array[4]; m.M11 = array[5]; m.M12 = array[6];
    m.M20 = array[8]; m.M21 = array[9]; m.M22 = array[10];
    return m;
}
force_inline m3
m3_double_array(m3a array)
{
    m3 m;
    m.M00 = array[0][0]; m.M01 = array[0][1]; m.M02 = array[0][2];
    m.M10 = array[1][0]; m.M11 = array[1][1]; m.M12 = array[1][2];
    m.M20 = array[2][0]; m.M21 = array[2][1]; m.M22 = array[2][2];
    return m;
}
force_inline m3
m3_mul(m3 m1, m3 m2)
{
    m3 r;
    f32 l00 = m1.M00, l01 = m1.M01, l02 = m1.M02,
	l10 = m1.M10, l11 = m1.M11, l12 = m1.M12,
	l20 = m1.M20, l21 = m1.M21, l22 = m1.M22,
	r00 = m2.M00, r01 = m2.M01, r02 = m2.M02,
	r10 = m2.M10, r11 = m2.M11, r12 = m2.M12,
	r20 = m2.M20, r21 = m2.M21, r22 = m2.M22;

    r.M00 = l00 * r00 + l10 * r01 + l20 * r02;
    r.M01 = l01 * r00 + l11 * r01 + l21 * r02;
    r.M02 = l02 * r00 + l12 * r01 + l22 * r02;
    r.M10 = l00 * r10 + l10 * r11 + l20 * r12;
    r.M11 = l01 * r10 + l11 * r11 + l21 * r12;
    r.M12 = l02 * r10 + l12 * r11 + l22 * r12;
    r.M20 = l00 * r20 + l10 * r21 + l20 * r22;
    r.M21 = l01 * r20 + l11 * r21 + l21 * r22;
    r.M22 = l02 * r20 + l12 * r21 + l22 * r22;
    return r;
}

force_inline m3
m3_copy(m3 a)
{
    m3 b;
    b.M00 = a.M00; b.M01 = a.M01; b.M02 = a.M02;
    b.M10 = a.M10; b.M11 = a.M11; b.M12 = a.M12;
    b.M20 = a.M20; b.M21 = a.M21; b.M22 = a.M22;
    return b;
}


/*

  ###################################
  ###################################
  M4.h
  ###################################
  ###################################

*/
force_inline m4
m4_array(f32* array)
{
    m4 m;
    m.M00 = array[0]; m.M01 = array[1]; m.M02 = array[2]; m.M03 = array[3];
    m.M10 = array[4]; m.M11 = array[5]; m.M12 = array[6]; m.M13 = array[7];
    m.M20 = array[8]; m.M21 = array[9]; m.M22 = array[10]; m.M23 = array[11];
    m.M30 = array[12]; m.M31 = array[13]; m.M32 = array[14]; m.M33 = array[15];
    return m;
}
force_inline m4
m4_double_array(m4a array)
{
    m4 m;
    m.M00 = array[0][0]; m.M01 = array[0][1]; m.M02 = array[0][2]; m.M03 = array[0][3];
    m.M10 = array[1][0]; m.M11 = array[1][1]; m.M12 = array[1][2]; m.M13 = array[1][3];
    m.M20 = array[2][0]; m.M21 = array[2][1]; m.M22 = array[2][2]; m.M23 = array[2][3];
    m.M30 = array[3][0]; m.M31 = array[3][1]; m.M32 = array[3][2]; m.M33 = array[3][3];
    return m;
}

force_inline m4
m4_identity()
{
    m4 m;
    m.M00 = 1.0f; m.M01 = 0.0f; m.M02 = 0.0f; m.M03 = 0.0f;
    m.M10 = 0.0f; m.M11 = 1.0f; m.M12 = 0.0f; m.M13 = 0.0f;
    m.M20 = 0.0f; m.M21 = 0.0f; m.M22 = 1.0f; m.M23 = 0.0f;
    m.M30 = 0.0f; m.M31 = 0.0f; m.M32 = 0.0f; m.M33 = 1.0f;
    return m;
}

force_inline m4
m4_copy(m4 a)
{
    m4 b;
    b.M00 = a.M00; b.M01 = a.M01; b.M02 = a.M02; b.M03 = a.M03;
    b.M10 = a.M10; b.M11 = a.M11; b.M12 = a.M12; b.M13 = a.M13;
    b.M20 = a.M20; b.M21 = a.M21; b.M22 = a.M22; b.M23 = a.M23;
    b.M30 = a.M30; b.M31 = a.M31; b.M32 = a.M32; b.M33 = a.M33;
    return b;
}
force_inline m4
m4_copy_array(f32 array[16])
{
    m4 b;
    b.M00 = array[0]; b.M01 = array[1]; b.M02 = array[2]; b.M03 = array[3];
    b.M10 = array[4]; b.M11 = array[5]; b.M12 = array[6]; b.M13 = array[7];
    b.M20 = array[8]; b.M21 = array[9]; b.M22 = array[10]; b.M23 = array[11];
    b.M30 = array[12]; b.M31 = array[13]; b.M32 = array[14]; b.M33 = array[15];
    return b;
}

// NOTE(bies): don't think we ever need m4_set_value function
force_inline m4
m4_zero()
{
    m4 m;
    m.M00 = 0; m.M10 = 0; m.M20 = 0; m.M30 = 0;
    m.M01 = 0; m.M11 = 0; m.M21 = 0; m.M31 = 0;
    m.M02 = 0; m.M12 = 0; m.M22 = 0; m.M32 = 0;
    m.M03 = 0; m.M13 = 0; m.M23 = 0; m.M33 = 0;
    return m;
}

#if 0
CGLM_INLINE
void
glm_rotate_make(mat4 m, float angle, vec3 axis) {
    CGLM_ALIGN(8) vec3 axisn, v, vs;
    float c = cosf(angle);

    glm_vec3_normalize_to(axis, axisn);
    glm_vec3_scale(axisn, 1.0f - c, v);
    glm_vec3_scale(axisn, sinf(angle), vs);

    glm_vec3_scale(axisn, v[0], m[0]);
    glm_vec3_scale(axisn, v[1], m[1]);
    glm_vec3_scale(axisn, v[2], m[2]);

    m[0][0] += c;     m[0][1] += vs[2]; m[0][2] -= vs[1];
    m[1][0] -= vs[2]; m[1][1] += c;     m[1][2] += vs[0];
    m[2][0] += vs[1]; m[2][1] -= vs[0]; m[2][2] += c;

    m[0][3] = m[1][3] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}
#endif

force_inline m4
m4_rotation_matrix(m4 m, v3 axis, f32 angle)
{
    f32 angleCos = cosf(angle);
    f32 angleSin = sinf(angle);
    v3 axisNormalized = v3_normalize(axis);

    v3 vCos = v3_mulv(axisNormalized, 1.0f - angleCos);
    m.V[0] = v3_mulv_as_v4(axisNormalized, vCos.X);
    m.V[1] = v3_mulv_as_v4(axisNormalized, vCos.Y);
    m.V[2] = v3_mulv_as_v4(axisNormalized, vCos.Z);

    v3 vSin = v3_mulv(axisNormalized, angleSin);
    m.M00 += angleCos; m.M01 += vSin.Z  ; m.M02 -= vSin.Y;
    m.M10 -= vSin.Z  ; m.M11 += angleCos; m.M12 += vSin.X;
    m.M20 += vSin.Y  ; m.M21 -= vSin.X  ; m.M22 += angleCos;

    m.M03 = m.M13 = m.M23 = m.M30 = m.M31 = m.M32 = 0.0f;
    m.M33 = 1.0f;
    return m;
}

force_inline m4
m4_mul_f32(m4 m, f32 v)
{
    m4 r;
    r.M00 = v * m.M00;
    r.M01 = v * m.M01;
    r.M02 = v * m.M02;
    r.M03 = v * m.M03;

    r.M10 = v * m.M10;
    r.M11 = v * m.M11;
    r.M12 = v * m.M12;
    r.M13 = v * m.M13;

    r.M20 = v * m.M20;
    r.M21 = v * m.M21;
    r.M22 = v * m.M22;
    r.M23 = v * m.M23;

    r.M30 = v * m.M30;
    r.M31 = v * m.M31;
    r.M32 = v * m.M32;
    r.M33 = v * m.M33;
    return r;
}

force_inline void
m4_mul_v4a(m4 m, v4 v, v4a r)
{
    r[0] = m.M00 * v.X + m.M10 * v.Y + m.M20 * v.Z + m.M30 * v.W;
    r[1] = m.M01 * v.X + m.M11 * v.Y + m.M21 * v.Z + m.M31 * v.W;
    r[2] = m.M02 * v.X + m.M12 * v.Y + m.M22 * v.Z + m.M32 * v.W;
    r[3] = m.M03 * v.X + m.M13 * v.Y + m.M23 * v.Z + m.M33 * v.W;
}
force_inline v4
m4_mul_v4(m4 m, v4 v)
{
    v4 r;
    r.X = m.M00 * v.X + m.M10 * v.Y + m.M20 * v.Z + m.M30 * v.W;
    r.Y = m.M01 * v.X + m.M11 * v.Y + m.M21 * v.Z + m.M31 * v.W;
    r.Z = m.M02 * v.X + m.M12 * v.Y + m.M22 * v.Z + m.M32 * v.W;
    r.W = m.M03 * v.X + m.M13 * v.Y + m.M23 * v.Z + m.M33 * v.W;
    return r;
}

force_inline void
m4_mul_v3a(m4 m, v3 v, f32 r[3])
{
    v4 xyzw = v4_new(v.X, v.Y, v.Z, 1.0f);
    v4 rv = m4_mul_v4(m, xyzw);

    r[0] = rv.X;
    r[1] = rv.Y;
    r[2] = rv.Z;
}
force_inline v3
m4_mul_v3(m4 m, v3 v)
{
    v3 r;
    v4 xyzw = v4_new(v.X, v.Y, v.Z, 1.0f);
    v4 rv = m4_mul_v4(m, xyzw);

    r.X = rv.X;
    r.Y = rv.Y;
    r.Z = rv.Z;
    return r;
}

force_inline m4
m4_mul(m4 m1, m4 m2)
{
    m4 r;
    /*
      Column major order for OpenGL

      1 2 3 4     1 2 3 4
      2 2 3 3  *  2 2 3 3
      4 4 2 1     4 4 2 1
      3 3 1 4     3 3 1 4

    */

    f32 l00 = m1.M00, l01 = m1.M01, l02 = m1.M02, l03 = m1.M03,
	l10 = m1.M10, l11 = m1.M11, l12 = m1.M12, l13 = m1.M13,
	l20 = m1.M20, l21 = m1.M21, l22 = m1.M22, l23 = m1.M23,
	l30 = m1.M30, l31 = m1.M31, l32 = m1.M32, l33 = m1.M33,

	r00 = m2.M00, r01 = m2.M01, r02 = m2.M02, r03 = m2.M03,
	r10 = m2.M10, r11 = m2.M11, r12 = m2.M12, r13 = m2.M13,
	r20 = m2.M20, r21 = m2.M21, r22 = m2.M22, r23 = m2.M23,
	r30 = m2.M30, r31 = m2.M31, r32 = m2.M32, r33 = m2.M33;

    // column
    r.M00 = l00 * r00 + l10 * r01 + l20 * r02 + l30 * r03;
    r.M01 = l01 * r00 + l11 * r01 + l21 * r02 + l31 * r03;
    r.M02 = l02 * r00 + l12 * r01 + l22 * r02 + l32 * r03;
    r.M03 = l03 * r00 + l13 * r01 + l23 * r02 + l33 * r03;

    r.M10 = l00 * r10 + l10 * r11 + l20 * r12 + l30 * r13;
    r.M11 = l01 * r10 + l11 * r11 + l21 * r12 + l31 * r13;
    r.M12 = l02 * r10 + l12 * r11 + l22 * r12 + l32 * r13;
    r.M13 = l03 * r10 + l13 * r11 + l23 * r12 + l33 * r13;

    r.M20 = l00 * r20 + l10 * r21 + l20 * r22 + l30 * r23;
    r.M21 = l01 * r20 + l11 * r21 + l21 * r22 + l31 * r23;
    r.M22 = l02 * r20 + l12 * r21 + l22 * r22 + l32 * r23;
    r.M23 = l03 * r20 + l13 * r21 + l23 * r22 + l33 * r23;

    r.M30 = l00 * r30 + l10 * r31 + l20 * r32 + l30 * r33;
    r.M31 = l01 * r30 + l11 * r31 + l21 * r32 + l31 * r33;
    r.M32 = l02 * r30 + l12 * r31 + l22 * r32 + l32 * r33;
    r.M33 = l03 * r30 + l13 * r31 + l23 * r32 + l33 * r33;

    return r;
}

force_inline m4
m4_mul_rot(m4 a, m4 b)
{
    m4 dest;
    f32 a00 = a.M00, a01 = a.M01, a02 = a.M02, a03 = a.M03,
	a10 = a.M10, a11 = a.M11, a12 = a.M12, a13 = a.M13,
	a20 = a.M20, a21 = a.M21, a22 = a.M22, a23 = a.M23,
	a30 = a.M30, a31 = a.M31, a32 = a.M32, a33 = a.M33,

	b00 = b.M00, b01 = b.M01, b02 = b.M02,
	b10 = b.M10, b11 = b.M11, b12 = b.M12,
	b20 = b.M20, b21 = b.M21, b22 = b.M22;

    dest.M00 = a00 * b00 + a10 * b01 + a20 * b02;
    dest.M01 = a01 * b00 + a11 * b01 + a21 * b02;
    dest.M02 = a02 * b00 + a12 * b01 + a22 * b02;
    dest.M03 = a03 * b00 + a13 * b01 + a23 * b02;

    dest.M10 = a00 * b10 + a10 * b11 + a20 * b12;
    dest.M11 = a01 * b10 + a11 * b11 + a21 * b12;
    dest.M12 = a02 * b10 + a12 * b11 + a22 * b12;
    dest.M13 = a03 * b10 + a13 * b11 + a23 * b12;

    dest.M20 = a00 * b20 + a10 * b21 + a20 * b22;
    dest.M21 = a01 * b20 + a11 * b21 + a21 * b22;
    dest.M22 = a02 * b20 + a12 * b21 + a22 * b22;
    dest.M23 = a03 * b20 + a13 * b21 + a23 * b22;

    dest.M30 = a30;
    dest.M31 = a31;
    dest.M32 = a32;
    dest.M33 = a33;

    return dest;
}

/* Object transformation operation */

force_inline m4
m4_translate(m4 m, v3 translation)
{
    v4 v1, v2, v3;
    v4a_scale(m.M[0], translation.X, v1.V);
    v4a_scale(m.M[1], translation.Y, v2.V);
    v4a_scale(m.M[2], translation.Z, v3.V);

    m.V[3] = v4_add(v1, m.V[3]);
    m.V[3] = v4_add(v2, m.V[3]);
    m.V[3] = v4_add(v3, m.V[3]);

    return m;
}
force_inline m4
m4_translate_identity(v3 translation)
{
    m4 m = m4_translate(m4_identity(), translation);
    return m;
}

force_inline m4
m4_scale_v3(m4 m, v3 v)
{
    m4 dest;
    dest.V[0] = v4_mulv(m.V[0], v.X);
    dest.V[1] = v4_mulv(m.V[1], v.Y);
    dest.V[2] = v4_mulv(m.V[2], v.Z);
    dest.V[3] = v4_copy(m.V[3]);
    return dest;
}

force_inline m4
m4_rotate_x(m4 m, f32 rad)
{
    f32 cosV = cos(rad);
    f32 sinV = sin(rad);
    m4 dest  = m4_identity();
    dest.M11 =  cosV;
    dest.M12 =  sinV;
    dest.M21 = -sinV;
    dest.M22 =  cosV;
    dest = m4_mul_rot(m, dest);
    return dest;
}

force_inline m4
m4_rotate_y(m4 m, f32 rad)
{
    f32 cosV = cos(rad);
    f32 sinV = sin(rad);
    m4 dest = m4_identity();
    dest.M00 =  cosV;
    dest.M02 = -sinV;
    dest.M20 =  sinV;
    dest.M22 =  cosV;
    dest = m4_mul_rot(m, dest);
    return dest;
}

force_inline m4
m4_rotate_z(m4 m, f32 rad)
{
    f32 cosV = cos(rad);
    f32 sinV = sin(rad);
    m4 dest  = m4_identity();
    dest.M00 =  cosV;
    dest.M01 =  sinV;
    dest.M10 = -sinV;
    dest.M11 =  cosV;
    dest = m4_mul_rot(m, dest);
    return dest;
}

//NOTE(bies): m4_rotation_matrix -> m4_rotate
force_inline m4
m4_rotate(m4 m, v3 axis, f32 angle)
{
    m4 rotationMatrix = m4_rotation_matrix(m, axis, angle);
    m4 dest = m4_mul(m, rotationMatrix);
    return dest;
}

force_inline m4
m4_rotate_at(m4 m, v3 axis, v3 pivot, f32 angle)
{
    v3 pivotNegative = v3_negative_to(pivot);
    m4 dest = m4_translate(m, pivot);
    dest = m4_rotate(m, axis, angle);
    dest = m4_translate(m, pivotNegative);
    return dest;
}

force_inline m4
m4_transform(v3 position, v3 scale, v3 rotation)
{
    m4 translationMat = m4_translate_identity(position);
    m4 rotationMat = m4_rotate_z(translationMat, rad(rotation.Z));
    m4 transform = m4_scale_v3(rotationMat, scale);

    return transform;
}
force_inline m4
m4_transform_wo_rotation(v3 position, v3 scale)
{
    m4 translationMat = m4_translate_identity(position);
    m4 transform = m4_scale_v3(translationMat, scale);

    return transform;
}

force_inline i32
m4_transform_decompose(m4 transform, v3* translation, v3* rotation, v3* scale)
{
    f32 zero = 0.0f;
    m4 localMatrix;
    localMatrix = m4_copy(transform);

    // Normalize the matrix.
    if (f32_equal(localMatrix.M33, zero))
    {
	return 0;
    }

    // First, isolate perspective. This is the messiest.
    i32 shouldIsolatePerspective = (!f32_equal(localMatrix.M03, zero) ||	!f32_equal(localMatrix.M13, zero) || !f32_equal(localMatrix.M23, zero));
    if (shouldIsolatePerspective)
    {
	// Clear the perspective partition
	localMatrix.M03 = (f32) 0;
	localMatrix.M13 = (f32) 0;
	localMatrix.M23 = (f32) 0;
	localMatrix.M33 = (f32) 1;
    }

    // Next take care of translation (easy).
    translation->V[0] = localMatrix.M30;
    translation->V[1] = localMatrix.M31;
    translation->V[2] = localMatrix.M32;
    translation->V[3] = localMatrix.M33;

    v3 row[3];
    // Now get scale and shear.
    for (i32 i = 0; i < 3; ++i)
	for (i32 j = 0; j < 3; ++j)
	    row[i].V[j] = localMatrix.M[i][j];


    // TODO(bies): fix it
    // Compute X scale factor and normalize first row.
    scale->X = v3_length(row[0]);
    row[0] = v3_scale(row[0], 1.0f);
    scale->Y = v3_length(row[1]);
    row[1] = v3_scale(row[1], 1.0f);
    scale->Z = v3_length(row[2]);
    row[2] = v3_scale(row[2], 1.0f);

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
#if 0
    v3 Pdum3 = cross(row[1], row[2]); // v3Cross(row[1], row[2], Pdum3);
    if (dot(row[0], Pdum3) < 0)
    {
	for (i32 i = 0; i < 3; i++)
	{
	    scale.v[i] *= f32(-1);
	    row[i]   *= f32(-1);
	}
    }
#endif

    rotation->Y = asin(-row[0].Z);
    if (cos(rotation->Y) != 0)
    {
	rotation->X = atan2(row[1].Z, row[2].Z);
	rotation->Z = atan2(row[0].Y, row[0].X);
    }
    else
    {
	rotation->X = atan2(-row[2].X, row[1].Y);
	rotation->Z = 0;
    }

    return 1;
}

force_inline m4
m4_inverse(m4 m)
{
    m4 r;

    f32 det;
    f32 t[6];
    f32 m00 = m.M00, m01 = m.M01, m02 = m.M02, m03 = m.M03,
	m10 = m.M10, m11 = m.M11, m12 = m.M12, m13 = m.M13,
	m20 = m.M20, m21 = m.M21, m22 = m.M22, m23 = m.M23,
	m30 = m.M30, m31 = m.M31, m32 = m.M32, m33 = m.M33;

    t[0] = m22 * m33 - m32 * m23;
    t[1] = m21 * m33 - m31 * m23;
    t[2] = m21 * m32 - m31 * m22;
    t[3] = m20 * m33 - m30 * m23;
    t[4] = m20 * m32 - m30 * m22;
    t[5] = m20 * m31 - m30 * m21;

    r.M00 =  m11 * t[0] - m12 * t[1] + m13 * t[2];
    r.M10 =-(m10 * t[0] - m12 * t[3] + m13 * t[4]);
    r.M20 =  m10 * t[1] - m11 * t[3] + m13 * t[5];
    r.M30 =-(m10 * t[2] - m11 * t[4] + m12 * t[5]);

    r.M01 =-(m01 * t[0] - m02 * t[1] + m03 * t[2]);
    r.M11 =  m00 * t[0] - m02 * t[3] + m03 * t[4];
    r.M21 =-(m00 * t[1] - m01 * t[3] + m03 * t[5]);
    r.M31 =  m00 * t[2] - m01 * t[4] + m02 * t[5];

    t[0] = m12 * m33 - m32 * m13;
    t[1] = m11 * m33 - m31 * m13;
    t[2] = m11 * m32 - m31 * m12;
    t[3] = m10 * m33 - m30 * m13;
    t[4] = m10 * m32 - m30 * m12;
    t[5] = m10 * m31 - m30 * m11;

    r.M02 =  m01 * t[0] - m02 * t[1] + m03 * t[2];
    r.M12 =-(m00 * t[0] - m02 * t[3] + m03 * t[4]);
    r.M22 =  m00 * t[1] - m01 * t[3] + m03 * t[5];
    r.M32 =-(m00 * t[2] - m01 * t[4] + m02 * t[5]);

    t[0] = m12 * m23 - m22 * m13;
    t[1] = m11 * m23 - m21 * m13;
    t[2] = m11 * m22 - m21 * m12;
    t[3] = m10 * m23 - m20 * m13;
    t[4] = m10 * m22 - m20 * m12;
    t[5] = m10 * m21 - m20 * m11;

    r.M03 =-(m01 * t[0] - m02 * t[1] + m03 * t[2]);
    r.M13 =  m00 * t[0] - m02 * t[3] + m03 * t[4];
    r.M23 =-(m00 * t[1] - m01 * t[3] + m03 * t[5]);
    r.M33 =  m00 * t[2] - m01 * t[4] + m02 * t[5];

    det = 1.0f / (m00 * r.M00 + m01 * r.M10 + m02 * r.M20 + m03 * r.M30);

    //glm_mat4_scale_p(dest, det);
    r = m4_mul_f32(r, det);

    return r;
}

force_inline f32
m4_determinant(m4 m)
{
    f32 det = m.M00 * m.M11 * m.M22
	+ m.M01 * m.M12 * m.M20
	+ m.M02 * m.M10 * m.M21
	- m.M02 * m.M11 * m.M20
	- m.M01 * m.M10 * m.M22
	- m.M00 * m.M12 * m.M21;

    return det;
}

force_inline m4
m4_t(m4 c)
{
    m4 r;

    r.M00 = c.M00;
    r.M10 = c.M01;
    r.M20 = c.M02;
    r.M30 = c.M03;

    r.M01 = c.M10;
    r.M11 = c.M11;
    r.M21 = c.M12;
    r.M31 = c.M13;

    r.M02 = c.M20;
    r.M12 = c.M21;
    r.M22 = c.M22;
    r.M32 = c.M23;

    r.M03 = c.M30;
    r.M13 = c.M31;
    r.M23 = c.M32;
    r.M33 = c.M33;

    return r;
}

force_inline m4
orthographic(f32 left, f32 right, f32 bot, f32 top, f32 nearVal, f32 farVal)
{
    m4 r;

    f32 rightleft = right - left;
    f32 topBot    = top - bot;
    f32 farNear   = farVal - nearVal;

    r.M01 = 0.0f; r.M02 = 0.0f; r.M03 = 0.0f;
    r.M10 = 0.0f; r.M12 = 0.0f; r.M13 = 0.0f;
    r.M20 = 0.0f; r.M21 = 0.0f; r.M23 = 0.0f;
    r.M30 = 0.0f; r.M31 = 0.0f; r.M32 = 0.0f;

    r.M00 =  2.0f / rightleft;
    r.M11 =  2.0f / topBot;
    r.M22 =  -2.0f / farNear;
    r.M30 = -(right  + left)    / rightleft;
    r.M31 = -(top    + bot)  / topBot;
    r.M32 = -(farVal + nearVal) / farNear;
    r.M33 =  1.0f;

    return r;
}

force_inline m4
perspective(f32 near, f32 far, f32 aspect, f32 fov)
{
    m4 r;

    f32 temp0 = 1.0f / tanf(0.5f * fov);
    f32 temp1 = 1.0f / (near - far);

    r.M01 = 0.0f; r.M02 = 0.0f; r.M03 = 0.0f;
    r.M10 = 0.0f; r.M12 = 0.0f; r.M13 = 0.0f;
    r.M20 = 0.0f; r.M21 = 0.0f;
    r.M30 = 0.0f; r.M31 = 0.0f; r.M33 = 0.0f;

    r.M00 = temp0 / aspect;
    r.M11 = temp0;
    r.M22 = (near + far) * temp1;
    r.M23 = -1.0f;
    r.M32 = 2.0f * near * far * temp1;

    return r;
}

force_inline m4
m4_look_at(v3 eye, v3 center, v3 up)
{
    m4 dest;

    v3 direction = v3_sub(center, eye);
    direction = v3_normalize(direction);

    v3 right = v3_cross_normalized(direction, up);
    v3 upPos = v3_cross(right, direction);

    dest.M00 =  right.X;
    dest.M01 =  upPos.X;
    dest.M02 = -direction.X;
    dest.M10 =  right.Y;
    dest.M11 =  upPos.Y;
    dest.M12 = -direction.Y;
    dest.M20 =  right.Z;
    dest.M21 =  upPos.Z;
    dest.M22 = -direction.Z;
    dest.M30 = -v3_dot(right, eye);
    dest.M31 = -v3_dot(upPos, eye);
    dest.M32 =  v3_dot(direction, eye);
    dest.M03 =  dest.M13 = dest.M23 = 0.0f;
    dest.M33 =  1.0f;

    return dest;
}

/*

  ###################################
  ###################################
  M4PrintIO.h
  ###################################
  ###################################

*/
force_inline void
m4_print(m4 m, const char* name)
{
    char sb[511];
    sprintf(sb, "%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n",
	    m.M00, m.M01, m.M02, m.M03,
	    m.M10, m.M11, m.M12, m.M13,
	    m.M20, m.M21, m.M22, m.M23,
	    m.M30, m.M31, m.M32, m.M33);

    printf("M4 ("GREEN("%s")"): \n%s", name, sb);
}

#define M4_PRINT(m) m4_print(m, #m)

/*

  ###################################
  ###################################
  Quaternion.h
  ###################################
  ###################################

*/

typedef struct Quaternion
{
    union
    {
	struct
	{
	    f32 X;
	    f32 Y;
	    f32 Z;
	    f32 W;
	};

	f32 V[4];
    };
} Quaternion;

force_inline Quaternion
quat(f32 x, f32 y, f32 z, f32 w)
{
    Quaternion quaternion;
    quaternion.X = x;
    quaternion.Y = y;
    quaternion.Z = z;
    quaternion.W = w;
    return quaternion;
}

force_inline Quaternion
quat_new(f32 angle, v3 axis)
{
    Quaternion quaternion;

    v3 normalizedAxis = v3_normalize(axis);
    f32 halfAngle = 0.5f * angle;
    f32 cosAngle = cosf(halfAngle);
    f32 sinAngle = sinf(halfAngle);

    quaternion.X = sinAngle * normalizedAxis.X;
    quaternion.Y = sinAngle * normalizedAxis.Y;
    quaternion.Z = sinAngle * normalizedAxis.Z;
    quaternion.W = cosAngle;

    return quaternion;
}


#endif
