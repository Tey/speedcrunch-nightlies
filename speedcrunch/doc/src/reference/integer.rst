Integer & Bitwise Arithmetic Functions
======================================

SpeedCrunch stores integers with a precision of up to 256 bits. For this reason, when working with common length integers (e.g. 32 bit), negative numbers will *not* be given by the two's complement of the respective number. Instead, the sign can be thought of being stored as a separate property. Thus, while usually the signed 16-bit integers *0xFFFF* and *-1* are identical, they are distinct in SpeedCrunch. See :func:`mask` and :func:`unmask` for how to convert a negative number into the two's complement form.

Any integer larger than the above limit will be silently converted into a floating point number, which will consequently make it susceptible to rounding errors.

To specify large integers, prefer the use of the shift operators (``1 << n``) to exponentiation (``2^n``). The latter will be performed as floating point, and will be prone to rounding errors. In particular, the result might not be integer.

Bitwise Operations
------------------

.. function:: and(x1; x2; ...)

    Performs a bitwise logical AND on the submitted parameters (one or more). All parameters  have to be real integers from the range --2\ :sup:`255` to +2\ :sup:`255`--1 (signed or unsigned 256 bit integers), non integer arguments are rounded toward zero. The result ranges from --2\ :sup:`255` to +2\ :sup:`255`--1 (signed integer). Note that ``and(x)`` is not the identity, because the unsigned +2\ :sup:`255` is mapped to the signed --2\ :sup:`255` for example. An error is returned if the parameters are not in the valid range.
    
.. function:: or(x1; x2; ...)

    Performs a bitwise logical OR on the submitted parameters (one or more). All parameters have to be integers from the range --2\ :sup:`255` to +2\ :sup:`255`--1 (signed integer), non integer arguments are rounded toward zero. Note that ``or(x)`` is not the identity, because the unsigned 2\ :sup:`255`     is mapped to the signed --2\ :sup:`255`, for example.
    
.. function:: xor(x1; x2; ...)

    Performs a bitwise logical XOR on the submitted parameters (one or more). All parameters have to be integers from the range --2\ :sup:`255` to +2\ :sup:`255`--1  (signed integer), non integer arguments are rounded toward zero. Note that ``xor(x)`` is not the identity, because the unsigned 2\ :sup:`255` is mapped to the signed --2\ :sup:`255`, for example.

.. function:: not(n)

    The :func:`not` function is defined by ``not(x) = -x-1``, giving the same result as the one's complement operator ``~`` in C/C++.
    
    .. warning:: The function does *not* simply flip the bits!
    
.. function:: shl(x; n)
    
    Performs an arithmetic **left** shift.
    
    :param x: The number (bit pattern) to shift, --2\ :sup:`255` <= `x` <= +2\ :sup:`256`-1.
    :param n: Number of bits to shift, --255 <= `n` <= 255. Must be integer.
    
    Note that `n` < 0 results in a right shift. The result ranges from --2\ :sup:`255` to +2\ :sup:`255`-1 (signed integer). `x` is rounded toward zero before shifting.  If `n` = 0, `x` is returned without rounding.
    
    Shifted out bits are always dropped. During a right shift, the most significant bit (bit 255) is copied. During a left shift, zero bits are shifted in.
    
    
.. function:: shr(n; bits)
    
    Performs an arithmetic **right** shift.
    
    :param x: The number (bit pattern) to shift, --2\ :sup:`255` <= `x` <= +2\ :sup:`256`-1.
    :param n: Number of bits to shift, --255 <= `n` <= 255. Must be integer.
    
    Note that `n` < 0 results in a left shift. The result ranges from --2\ :sup:`255` to +2\ :sup:`255`-1 (signed integer). `x` is rounded toward zero before shifting.  If `n` = 0, `x` is returned without rounding.
    
    Shifted out bits are always dropped. During a right shift, the most significant bit (bit 255) is copied. During a left shift, zero bits are shifted in.
                
.. function:: mask(x; n)
    
    Returns the lowest `n` bits from `x`. For this, `x` must be in the range --2\ :sup:`255` <= `x` <= +2\ :sup:`256`-1, and `n` must be an integer, 1 <= `n` <= 255. `x` is rounded toward zero.
    
    The result is always unsigned.
    
    .. admonition:: Example
    
        We would like to find the two's complement representation of -1 in a 16-bit system. ::
    
            hex(mask(-1; 16)) = 0xFFFF
        
.. function:: unmask(x; n)
    
    Takes the lower `n` bits from `x` and sign-extends them to full 256 bit. This means that bit at position `n` - 1 is copied to all upper bits.
    
    `x` must be in the range --2\ :sup:`255` <= `x` <= +2\ :sup:`256`-1, and `n` must be an integer, 1 <= `n` <= 255. `x` is rounded toward zero.
    
    .. admonition:: Example
    
        We would like to convert a number in two's complement representation to a signed number. ::
    
            unmask(0xFFFF; 16) = -1
            unmask(0x1FFF; 16) = 0x1FFF
        
    

Radix Change
------------

.. function:: bin(n)
    
    Displays the number `n` in binary.
    
    Note that the change of radix only affects the current result; after applying any operation to the result of :func:`bin`, the output will again be displayed in the default radix.
    
    To permanently change the radix, you need to change the according setting.
    
    .. warning:: TODO: Link to how to do that.

.. function:: oct(n)
    
    Displays the number `n` in octal.
    
    Note that the change of radix only affects the current result; after applying any operation to the result of :func:`oct`, the output will again be displayed in the default radix.
    
    To permanently change the radix, you need to change the according setting.
    
    .. warning:: TODO: Link to how to do that.
    
    
.. function:: dec(n)
    
    Displays the number `n` in decimal.
    
    Note that the change of radix only affects the current result; after applying any operation to the result of :func:`dec`, the output will again be displayed in the default radix.
    
    To permanently change the radix, you need to change the according setting.
    
    .. warning:: TODO: Link to how to do that.
    
.. function:: hex(n)
    
    Displays the number `n` in hexadecimal.
    
    Note that the change of radix only affects the current result; after applying any operation to the result of :func:`hex`, the output will again be displayed in the default radix.
    
    To permanently change the radix, you need to change the according setting.
    
    .. warning:: TODO: Link to how to do that.

    
Rounding
--------

Beware of rounding errors! Although SpeedCrunch performs all computations internally with ~78 significant digits (which is *much* more than regular calculators), any arguments beyond that limit are subject to rounding errors.

.. function:: ceil(x)
    
    Rounds `x` to the next largest integer.
    
    Only real, dimensionless arguments are allowed.
    
.. function:: floor(x)
    
    Rounds `x` to the next smallest integer.
    
    Only real, dimensionless arguments are allowed.
    
    
.. function:: round(x\[; n\])
    
    Rounds `x` to the nearest `n`-digit number. `n` may be omitted, in which case `x` is rounded to the closest integer.
        
    Only real, dimensionless arguments are allowed.
    
.. function:: trunc(x\[; n\])
    
    Truncates (rounds toward zero) `x` to the next `n`-digit number. `n` may be omitted, in which case `x` is rounded to integer.
    
    Only real, dimensionless arguments are allowed.
    
.. seealso:: 
    * :func:`int`
    * :func:`frac`
          

Integer Division
----------------

.. function:: idiv(a; b)
    
    Computes the integer part of the division `a/b`. The result of :func:`idiv` is guaranteed to be exact. While ``int(a/b)`` covers a larger range of arguments, the result is computed via floating point arithmetics, and may be subject to rounding errors. :func:`idiv` will instead yield and error if the parameters exceed the safe bounds.
    
    It is possible to apply the idiv function to non-integers as well, but be aware that rounding errors might be lead to off-by-one errors. If idiv detects, that a result depends on the validity of the guard digits, it returns a NaN as a warning.
    
    Only real, dimensionless arguments are allowed.
    
.. function:: mod(a; n)

    Computes the remainder of the integer division `a/n`. The divisor `n` must be non-zero, and the result takes the sign of `a`.
    
    This function always returns an exact result, provided that the parameters are exact.
    
    You can use this function with non-integers as well, but rounding errors might lead to off-by-one errors. Evaluating :func:`mod` can potentially be computationally expensive, so the function is internally restricted to 250 division loops.
    
    Only real, dimensionless arguments are allowed.

.. function:: gcd(n1; n2; ...)

    Returns the greatest common divisor of the integers `n1`, `n2`, ... (2 or more). You can use this function to reduce a rational number. If a rational number is given as `p/q`, its reduced form is `(p/gcd(p;q))/(q/gcd(p;q))`. Closely related to :func:`gcd` is the ``lcm()`` function (least common multiple). While ``lcm()`` is not defined by default in SpeedCrunch, you may want to define it yourself by::
     
        lcm(n1; n2) = n1*n2/gcd(n1; n2)

    Only real, integer arguments are allowed by :func:`gcd`.
                
                
