Integer & Bitwise Arithmetic Functions
======================================

Bitwise Operations
------------------

.. function:: and(x1; x2; ...)

    Performs a bitwise logical AND on the submitted parameters (at least two). All parameters have to be real integers from the range -2\ :sup:`255` to +2\ :sup:`255`-1 (signed or unsigned 256 bit integers), non integer arguments are rounded toward zero. The result ranges from -2\ :sup:`255` to +2\ :sup:`255`-1 (signed integer).

.. function:: or(x1; x2; ...)

    Performs a bitwise logical OR on the submitted parameters (at least two). All parameters have to be integers from the range -2\ :sup:`255` to +2\ :sup:`255`-1 (signed integer), non integer arguments are rounded toward zero.

.. function:: xor(x1; x2; ...)

    Performs a bitwise logical XOR on the submitted parameters (at least two). All parameters have to be integers from the range -2\ :sup:`255` to +2\ :sup:`255`-1  (signed integer), non integer arguments are rounded toward zero.

.. function:: not(n)

    The :func:`not` function is defined by ``not(x) = -x-1``, giving the same result as the one's complement operator ``~`` in C/C++.

    .. warning:: This function does *not* simply flip the bits!

.. function:: shl(x; n)

    Performs an arithmetic left shift.

    :param x: The number (bit pattern) to shift, -2\ :sup:`255` <= ``x`` <= +2\ :sup:`256`-1.
    :param n: Number of bits to shift, -255 <= ``n`` <= 255. Must be integer.

    Note that ``n`` < 0 results in a right shift. The result ranges from -2\ :sup:`255` to +2\ :sup:`255`-1 (signed integer). ``x`` is rounded toward zero before shifting.  If ``n`` = 0, ``x`` is returned without rounding.

    Shifted out bits are always dropped. During a right shift, the most significant bit (bit 255) is copied. During a left shift, zero bits are shifted in.


.. function:: shr(x; n)

    Performs an arithmetic right shift.

    :param x: The number (bit pattern) to shift, -2\ :sup:`255` <= ``x`` <= +2\ :sup:`256`-1.
    :param n: Number of bits to shift, -255 <= ``n`` <= 255. Must be integer.

    Note that ``n`` < 0 results in a left shift. The result ranges from -2\ :sup:`255` to +2\ :sup:`255`-1 (signed integer). ``x`` is rounded toward zero before shifting.  If ``n`` = 0, ``x`` is returned without rounding.

    Shifted out bits are always dropped. During a right shift, the most significant bit (bit 255) is copied. During a left shift, zero bits are shifted in.

.. function:: mask(x; n)

    Returns the lowest ``n`` bits from ``x``. For this, ``x`` must be in the range -2\ :sup:`255` <= ``x`` <= +2\ :sup:`256`-1, and ``n`` must be an integer, 1 <= ``n`` <= 255. ``x`` is rounded toward zero.

    The result is always unsigned.

    Example: Getting the two's complement of -1 in a 16-bit system::

        hex(mask(-1; 16))
        = 0xFFFF

.. function:: unmask(x; n)

    Takes the lower ``n`` bits from ``x`` and sign-extends them to full 256 bits. This means that bit at position *n-1* is copied to all upper bits.

    The value of ``x`` must be in the range *-2* :sup:`255` *<= x <= +2* :sup:`256` *-1*, and ``n`` must be an integer, *1 <= n <= 255*. ``x`` is rounded toward zero.

    Example: Converting a number in two's complement representation to a signed number::

        unmask(0xFFFF; 16)
        = -1
        unmask(0x1FFF; 16)
        = 0x1FFF



Numeral Bases
-------------

The following functions only change the format for the current result. To change the base
that is used for displaying results, select one of the corresponding settings in :menuselection:`Settings --> Result Format`.

.. function:: bin(n)

    Format ``n`` as binary (base-2).

.. function:: oct(n)

    Format ``n`` as octal (base-8).

.. function:: dec(n)

    Format ``n`` as decimal (base-10).

.. function:: hex(n)

    Format ``n`` as hexadecimal (base-16).


Rounding
--------

.. function:: ceil(x)

    Round ``x`` to the next largest integer. Only real, dimensionless arguments are allowed.

.. function:: floor(x)

    Round ``x`` to the next smallest integer. Only real, dimensionless arguments are allowed.


.. function:: round(x [; n])

    Round ``x`` to the nearest number with ``n`` fractional digits; ``n`` may be omitted, in which case ``x`` is rounded to the closest integer. Ties are broken by rounding to the nearest *even* integer. This rounding strategy, commonly known as *Banker's rounding*, serves to avoid a bias (for instance when averaging or summing).

    Example::

        round(0.5)
        = 0

        round(1.5)
        = 2

        round(12.345; 2)
        = 12.34

        round(12345; -2)
        = 12300

    Only real, dimensionless arguments are allowed.

.. function:: trunc(x [; n])

    Truncate (rounds toward zero) ``x`` to the next number with ``n`` fractional digits; ``n`` may be omitted, in which case ``x`` is rounded to integer. Only real, dimensionless arguments are allowed.

    .. seealso::
       | :func:`int`
       | :func:`frac`


Integer Division
----------------

.. function:: idiv(a; b)

    Compute the integer part of the division ``a/b``. The result is guaranteed to be exact. While ``int(a/b)`` covers a larger range of arguments, the result is computed via floating point arithmetics and may be subject to rounding errors. This function will instead yield an error if the parameters exceed the safe bounds.

    It is possible to apply :func:`idiv` to non-integers as well, but be aware that rounding errors might be lead to off-by-one errors. If the result depends on the validity of the guard digits, *NaN* is returned.

    Only real, dimensionless arguments are allowed.

.. function:: mod(a; b)

    Compute the remainder of the integer division ``a/b``. The divisor ``b`` must be non-zero. The result takes the sign of ``a``.

    This function always returns an exact result, provided that the parameters are exact.

    You can use this function with non-integers as well, but rounding errors might lead to off-by-one errors. Evaluating :func:`mod` can be computationally expensive, so the function is internally restricted to 250 division loops.

    Only real, dimensionless arguments are allowed.

.. function:: gcd(n1; n2; ...)

    Returns the greatest common divisor of the arguments (at least two must be given). You can use this function to reduce a rational number.
    If a rational number is given as ``p/q``, its reduced form is ``(p / gcd(p; q)) / (q / gcd(p; q))``.

    A closely related function is ``lcm()`` (*least common multiple*). While ``lcm()`` is not defined by default
    in SpeedCrunch, you can define it yourself as::

        lcm(n1; n2) = n1 * n2 / gcd(n1; n2)

    Only real, integer arguments are allowed.
