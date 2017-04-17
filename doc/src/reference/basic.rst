Basic Math Functions
====================

General
-------

.. function:: abs(x)

    Return the absolute value of ``x``, commonly written as \|x\|. In real mode, it flips the sign of a negative number, thus forcing it to be positive. When given a complex number as argument, it returns the modulus of the number.

    The argument can have a dimension.

    Example::

        abs(-3 meter)
        = 3 meter

        abs(4 + 3j)
        = 5

.. function:: sqrt(x)

    Return the square root of ``x``. If complex numbers are disabled, this function is only defined for *x > 0*. In *complex mode*, any complex number may be specified, yielding the complex root in the upper half plane.

    The argument may have a dimension.

.. function:: cbrt(x)

    Compute the third (cubic) root of ``x``. In *real mode*, it accepts any real number. Negative numbers will yield a negative cubic root::

        cbrt(-27)
        = -3

    In *complex mode*, this function accepts any complex input. The result will generally be the first complex root, i.e. the one with a phase between 0 and π/3. Real negative arguments however will still yield a real (negative) result, matching the function's behavior in *real mode*. Use ``x^(1/3)`` to get the first complex root.

.. function:: exp(x)

    Compute the natural exponential function.

    The argument must be dimensionless.

    .. seealso::
       | :func:`ln` (natural logarithm)
       | :func:`lg` (binary logarithm)
       | :func:`log` (arbitrary base logarithm)

.. function:: ln(x)

    Compute the natural logarithm. In *real mode*, the argument must be real, with *x > 0*.

    In *complex mode*, any non-zero number may be given. The result will be the principal value. The branch cut runs across the negative real axis. Nevertheless, in SpeedCrunch :func:`ln` is defined for negative real numbers as *ln(-x) = ln(\|x\|)) + πj*, extending the branch from the *upper* half-plane.

.. function:: lb(x)

    Compute the binary logarithm. In *complex mode* the same rules apply as for :func:`ln`.

.. function:: lg(x)

    Compute the decimal logarithm. In *complex mode* the same rules apply as for :func:`ln`.

.. function:: log(n; x)

    Compute the logarithm of base ``n`` . In *complex mode* the same rules apply as for :func:`ln`.


.. _trigonometric:

Trigonometric & Inverse Trigonometric
-------------------------------------

.. function:: sin(x)

    Returns the sine of ``x``. The behavior depends on both the angle unit setting (degrees or radians) and on whether complex numbers are enabled.

    In *degrees* mode, the argument is assumed to be expressed in degrees, such that :func:`sin` is periodic with a period of 360 degrees: *sin(x) = sin(x+360)*. Complex arguments are not allowed in *degrees* mode, regardless of the corresponding setting.

    When *radians* are set as the angle unit, :func:`sin` will be 2π-periodic. The argument may be complex.

    For real arguments beyond approx. \|x\|>10\ :sup:`77`, SpeedCrunch no longer recognizes the periodicity of the function and issues an error.

    The argument of :func:`sin` must be dimensionless.

    The inverse function is :func:`arcsin`.

    .. seealso::
        | :func:`cos`
        | :func:`tan`
        | :func:`cot`
        | :func:`arcsin`

.. function:: cos(x)

    Returns the cosine of ``x``. The behavior depends on both the angle unit setting (degrees or radians) and on whether complex numbers are enabled.

    In *degrees* mode, the argument is assumed to be expressed in degrees, such that :func:`cos` is periodic with a period of 360 degrees: *cos(x) = cos(x+360)*. Complex arguments are not allowed in *degrees* mode, regardless of the corresponding setting.

    When *radians* are set as the angle unit, :func:`cos` will be 2π-periodic. The argument may be complex.

    For real arguments beyond approx. \|x\|>10\ :sup:`77`, SpeedCrunch no longer recognizes the periodicity of the function and issues an error.

    The argument of :func:`cos` must be dimensionless.

    The inverse function is :func:`arccos`.

    .. seealso::
        | :func:`sin`
        | :func:`tan`
        | :func:`cot`
        | :func:`sec`
        | :func:`arccos`

.. function:: tan(x)

    Returns the tangent of ``x``. The behavior depends on both the angle unit setting (degrees or radians) and on whether complex numbers are enabled.

    In *degrees* mode, the argument is assumed to be expressed in degrees, such that :func:`tan` is periodic with a period of 180 degrees: *tan(x) = tan(x+180)*. Complex arguments are not allowed in *degrees* mode, regardless of the corresponding setting.

    When *radians* are set as the angle unit, :func:`tan` will be π-periodic. The argument may be complex.

    The argument of :func:`tan` must be dimensionless.

    The inverse function is :func:`arctan`.

    .. seealso::
        | :func:`cos`
        | :func:`sin`
        | :func:`cot`

.. function:: cot(x)

    Returns the cotangent of ``x``. The behavior depends on both the angle unit setting (degrees or radians) and on whether complex numbers are enabled.

    In *degrees* mode, the argument is assumed to be expressed in degrees, such that :func:`cot` is periodic with a period of 180 degrees: *cot(x) = cot(x+180)*. Complex arguments are not allowed in *degrees* mode, regardless of the corresponding setting.

    When *radians* are set as the angle unit, :func:`cot` will be π-periodic. The argument may be complex.

    The argument of :func:`cot` must be dimensionless.

    .. seealso::
        | :func:`cos`
        | :func:`sin`
        | :func:`tan`

.. function:: sec(x)

    Returns the secant of ``x``, defined as the reciprocal cosine of ``x``: *sec(x) = 1/cos(x)*. The behavior depends on both the angle unit setting (degrees or radians) and on whether complex numbers are enabled.

    In *degrees* mode, the argument is assumed to be expressed in degrees, such that :func:`sec` is periodic with a period of 360 degrees: *sec(x) = sec(x+360)*. Complex arguments are not allowed in *degrees* mode, regardless of the corresponding setting.

    When *radians* are set as the angle unit, :func:`sec` will be 2π-periodic. The argument may be complex.

    For real arguments beyond approx. \|x\|>10\ :sup:`77`, SpeedCrunch no longer recognizes the periodicity of the function and issues an error.

    The argument of :func:`sec` must be dimensionless.

.. function:: csc(x)

    Returns the cosecant of ``x``, defined as the reciprocal sine of ``x``: *csc(x) = 1/sin(x)*. The behavior depends on both the angle unit setting (degrees or radians) and on whether complex numbers are enabled.

    In *degrees* mode, the argument is assumed to be expressed in degrees, such that :func:`csc` is periodic with a period of 360 degrees: *csc(x) = csc(x+360)*. Complex arguments are not allowed in *degrees* mode, regardless of the corresponding setting.

    When *radians* are set as the angle unit, :func:`csc` will be 2π-periodic. The argument may be complex.

    For real arguments beyond approx. \|x\|>10\ :sup:`77`, SpeedCrunch no longer recognizes the periodicity of the function and issues an error.

    The argument of :func:`csc` must be dimensionless.


.. function:: arccos(x)

    Returns the inverse cosine of ``x``, such that *cos(arccos(x)) = x*. The behavior of the function depends on both the angle unit setting (degrees or radians) and on whether complex numbers are enabled.

    In *degrees* mode, :func:`arccos` takes a real argument from *[-1, 1]*, and the return value is in the range *[0, 180]*. Real arguments outside *[-1, 1]* and complex numbers are not allowed in *degrees* mode.

    When *radians* are set as the angle unit, :func:`arccos` maps an element from *[-1, 1]* to a value in *[0, π]*. When complex numbers are enabled in addition, :func:`arccos` may take any argument from the complex plane. In complex mode, *arccos(-1) = π* and *arccos(1) = 0* will yield the same result as in real mode.

    The argument of :func:`arccos` must be dimensionless.

    The inverse function is :func:`cos`.

.. function:: arcsin(x)

    Returns the inverse sine of ``x``, such that *sin(arcsin(x)) = x*. The behavior of the function depends on both the angle unit setting (degrees or radians) and on whether complex numbers are enabled.

    In *degrees* mode, :func:`arcsin` takes a real argument from *[-1, 1]*, and the return value is in the range *[-90, 90]*. Real arguments outside *[-1, 1]* and complex numbers are not allowed in *degrees* mode.

    When *radians* are set as the angle unit, :func:`arcsin` maps an element from *[-1, 1]* to a value in *[-π/2, π/2]*. When complex numbers are enabled in addition, :func:`arcsin` may take any argument from the complex plane. In complex mode, *arcsin(-1) = π/2* and *arcsin(1) = π/2* will yield the same result as in real mode.

    The argument of :func:`arccos` must be dimensionless.

    The inverse function is :func:`sin`.


.. function:: arctan(x)

    Returns the inverse tangent of ``x``, such that *tan(arctan(x)) = x*. The behavior of the function depends on both the angle unit setting (degrees or radians) and on whether complex numbers are enabled.

    In *degrees* mode, :func:`arctan` takes a real argument, and the return value is in the range *[-90, 90]*. Complex arguments are not allowed in *degrees* mode.

    When *radians* are set as the angle unit, :func:`arctan` maps a real number to a value in *[-π/2, π/2]*. When complex numbers are enabled in addition, :func:`arctan` may take any argument from the complex plane, except for *+j* and *-j*.

    The argument of :func:`arctan` must be dimensionless.

    The inverse function is :func:`tan`.

.. function:: arctan2(x, y)

    Returns the angle formed by the vector *(x, y)* and the X axis. If the point *(x, y)* lies in the first quadrant (i.e. both *x > 0* and *y > 0* are true), it is given by *arctan(y/x)*. However, the function handles vectors in other quadrants as well.

    The behavior of the function depends on the angle unit setting (degrees or radians). In *degrees* mode, this function returns a value in the range *]-180, 180]*. When *radians* are set as the angle unit, the return value lies in the range *]-π, π]*.

    Unlike :func:`arctan` this function only accepts real arguments.

    The argument values must be dimensionless.


Hyperbolic & Inverse Hyperbolic
-------------------------------

.. function:: sinh(x)

    Return the hyperbolic sine of ``x``. In *complex mode*, any complex number may be used as the argument.

    The argument must be dimensionless.

    The inverse function is :func:`arsinh`.


.. function:: cosh(x)

    Return the hyperbolic cosine of ``x``. In *complex mode*, any complex number may be used as the argument.

    The argument must be dimensionless.

    The inverse function is :func:`arcosh`.


.. function:: tanh(x)

    Return the hyperbolic tangent of ``x``. In *complex mode*, any complex number may be used as the argument.

    The argument must be dimensionless.

    The inverse function is :func:`artanh`.


.. function:: arsinh(x)

    Compute the area hyperbolic sine of ``x``, the inverse function to :func:`sinh`. *arsinh(x)* is the only solution to *cosh(y) = x*.

    In complex mode, the function is defined for any complex ``z`` as *arsinh(z) = ln[z + (z* :sup:`2` *+1)* :sup:`1/2` *]*.

    The function only accepts dimensionless arguments.


.. function:: arcosh(x)

    Compute the area hyperbolic cosine of ``x``, the inverse function to :func:`cosh`. *arcosh(x)* is the positive solution to *cosh(y) = x*. Except for *x=1*, the second solution to this equation will be given by *-arcosh(x)*.

    In real mode, the parameter ``x`` must be *> 1*. In complex mode, the function is defined for any complex ``z`` as *arcosh(z) = ln[z + (z* :sup:`2` *-1)* :sup:`2` *]*.

    The function only accepts dimensionless arguments.


.. function:: artanh(x)

    Compute the area hyperbolic tangent of ``x``, the inverse function to :func:`tanh`. *artanh(x)* is the only solution to *tanh(y) = x*. In real mode, the parameter ``x`` has to fulfill *-1 < x < 1*.

    In complex mode, this function accepts any argument except for -1 and +1. In the complex plane, it is defined as *artanh(z) = 1/2 \* ln[(z+1)/(z-1)]*.

    The function only accepts dimensionless arguments.


Special
-------

.. function:: erf(x)

    Compute the error function, evaluated in ``x``. The error function is closely related to the Gaussian cumulative density function.

    Note that currently only real arguments are allowed. Furthermore, the function only accepts dimensionless arguments.

.. function:: erfc(x)

    Compute the complementary error function, evaluated in ``x``. The complementary error function is defined by ``erfc(x) = 1 - erf(x)``.

    Note that currently only real arguments are allowed. Furthermore, the function only accepts dimensionless arguments.

.. function:: gamma(x)

    Evaluates the gamma function (frequently denoted by the Greek letter Γ). The gamma function is an analytic extension to the factorial operation which is defined on real numbers as well. The relation between factorial and the gamma function is given by *Γ(n) = (n - 1)!*.

    Note that currently only real arguments are allowed. Furthermore, the function only accepts dimensionless arguments.

    The computation of the factorial operation is in fact implemented via :func:`gamma`. This means that in SpeedCrunch, factorials of non-integer numbers are allowed.

.. function:: lngamma(x)

    Computes ``ln(abs(gamma(x)))``. As the gamma function grows extremely quickly, it is sometimes easier to work with its logarithm instead. :func:`lngamma` allows much larger arguments that would otherwise overflow :func:`gamma`.

    Note that currently only real arguments are allowed. Furthermore, the function only accepts dimensionless arguments.


Complex Numbers
---------------

.. function:: real(x)

    Return the real part of a complex number ``x``.

    The argument may have a dimension.

.. function:: imag(x)

    Return the imaginary part of a complex number ``x``.

    The argument may have a dimension.

.. function:: conj(x)

    Return the complex conjugate of a complex number ``x``.

    In *complex mode*, this function accepts any real or complex input. In *real mode*, the imaginary portion of ``x`` is stripped to ensure that the value returned is purely real.

.. function:: phase(x)

    Returns the phase (angle) of a complex number ``x``. The unit of the angle corresponds to the current angle mode.

    The argument may have a dimension.

    .. seealso::
       | :func:`abs` (absolute value)

.. function:: polar(x)

    Converts the complex number ``x`` to polar form, i.e. the form *r e* :sup:`jɸ`. The angle ɸ is always given in radians.

.. function:: cart(x)

    Converts the complex number ``x`` to cartesian form, i.e. the form *a + j b*.


Various
-------

.. function:: sgn(x)

    For *x >= 0*, return +1. For *x < 0*, return -1.

.. function:: radians(x)

    Convert the angle ``x`` into radians. Independently of the angle unit setting, this function will assume that ``x`` is given in degrees and return ``pi*x/180``.

    The function only accepts real, dimensionless arguments.

.. function:: degrees(x)

    Convert the angle ``x`` into degrees. Independently of the angle unit setting, this function will assume that ``x`` is given in radians and return ``180*x/pi``.

    The function only accepts real, dimensionless arguments.

.. function:: int(x)

    Returns the integer part of ``x``, effectively rounding it towards zero.

    The function only accepts real, dimensionless arguments.

.. function:: frac(x)

    Returns the fractional (non-integer) part of ``x``, given by ``frac(x) = x - int(x)``.

    The function only accepts real, dimensionless arguments.
