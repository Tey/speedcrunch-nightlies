This part of the documentation lays out the general syntax of a valid SpeedCrunch input. As you will see, SpeedCrunch honors most conventions for mathematical expressions. Especially if you are already familiar with a programming language, you will find using SpeedCrunch to be very natural and intuitive.


Notation of Numbers
===================

In this manual we differentiate two kinds of symbols you may use to specify the value of a number or quantity. One is a *literal number* (or simply *literal* in general), like `5` or `3.141592`. The other is an *identifier*, i.e. a special name given to a specific value, e.g. ``pi``, ``meter``, or any of your own choosing like ``x`` or ``foobar``. In this section we shall explain which notation for *literals* you may use in SpeedCrunch.

Fixed point
-----------
When you would like to specify a non-integer value, simply enter the number as you would write it on paper, with either a dot ``.`` or a comma ``,`` as the decimal separator. Indeed, since in some countries it is customary to use a comma rather than a dot for this purpose, SpeedCrunch treats them interchangeably, both ``1.234`` and ``1,234`` will evaluate to the same result. Note that for this reason you must not use either comma or dot as a thousand's separator. For instance ``1,234.567`` is not a valid input. Use spaces or apostrophes ``'`` for emphasis of digit groups, e.g. ``1 000 000`` or ``1'000'000`` for a million.

Trailing zeros after the decimal point (like in ``12.300``), or leading zeros before it (``0012.3``) are redundant, and can be included or omitted to the user's preference. Expressions like ``.5`` as a shorthand notation for ``0.5`` are also permitted.


.. _scientific_notation:

Scientific notation (floating point)
------------------------------------
When dealing with very small or very large numbers (think size of an atom, or a galaxy) the notation above is most inconvenient. Commonly these are expressed in scientific notation, for instance instead of writing 0.000000001234, one prefers 1.234*10\ :sup:`-9`.

Naturally, in SpeedCrunch you could write this as ``1.234*10^-9``, but there exists a shorthand notation for this, namely ``1.234e-9``. Here the ``e`` stands for ``*10^``, but is considered a part of the number literal, and treated with higher precedence, e.g. ``1e2^3`` is 100\ :sup:`3` and not 10\ :sup:`8`. The scale of a number (sometimes called its exponent) always begins with the scale character ``E`` or ``e`` followed by a signed integer. So ``e+10``, ``e-4``, ``E-0`` are all valid scale expressions. If the sign is '+', you may simply omit it: ``e0``, ``E10``.

Naturally the significand (i.e. the part preceding the exponent) is required, and not more than one exponent may be specified.

Compared to most calculators, SpeedCrunch can accept very large numbers without overflowing (both ``1e+536870911`` and ``1e-536870911`` are still valid). However, only about 78 significant digits are stored at any point. Any digits beyond that are lost.

Bases 2, 8 and 16
-----------------
Sometimes you may require to work in numeral bases other than decimal. SpeedCrunch provides support for binary, octal and hexadecimal numbers.

You can enter a number in the any base by marking it with the corresponding prefix.

* ``0b`` or ``0B`` for **binary**, e.g. ``0b10010``.
* ``0o`` or ``0O`` for **octal**, e.g. ``0o1412``.
* ``0d`` or ``0D`` for **decimal**. These are always redundant, as decimal is the default base.
* ``0x``, ``0X``, ``0h``, ``0H`` or ``#`` for **hexadecimal**. The additional digits 10 to 15 are represented by upper or lower case letters ``a`` to ``f``, e.g. ``0xdeadbeef`` or ``0xDEADBEEF``.

You may even enter fractional values in either of these bases. At this time only fixed point notation is allowed for input. Examples::

    0b1.01
    = 1.25
    
    0xf.a
    = 15.625

To have SpeedCrunch output its results in a base other than decimal, you may use one of the functions :func:`bin`, :func:`oct`, :func:`dec`, :func:`hex`::

    hex(12341)
    = 0x3035

The effect of these functions is only temporary, the formatting will be reset with any following operation::

    0x2 * hex(12341)
    = 24682
    
If you would like to more permanently change the output base, change the corresponding setting under :menuselection:`Settings --> Result Format`.

SpeedCrunch stores integers with a precision of up to 256 bits. For this reason, when working with common length integers (e.g. 32 bit), negative numbers will *not* be given by the two's complement of the respective number. Instead, the sign can be thought of being stored as a separate property. Thus, while usually the signed 16-bit integers *0xFFFF* and *-1* are identical, they are distinct in SpeedCrunch. See :func:`mask` and :func:`unmask` for how to convert a negative number into the two's complement form.


    
Operators and Precedence
========================

When writing an expression like ``10+5*4``, which operation will be executed first? The common rules of operator precedence tell us that in this case multipication shall be computed first, hence the result is ``30``. We also distinguish **unary** operators (which act on a single number/operand) and **binary** operators (which link two operands).

The operators in SpeedCrunch and their precedence are listed here in decreasing order of precedence.

* Parentheses ``(...)``. Use these to mark precedence explicitly, e.g. ``(2+3)*4 = 5*4 = 20``.
* Unary operator ``!``. Computes the factorial of its argument, e.g. ``5! = 125``, see also :func:`gamma`.
* Binary operator ``^`` or ``**`` (power). Both notations (``^`` and ``**``) are equivalent. Note that the power operation is *right-associative*, i.e. evaluated from right to left, e.g. ``2^2^3 = 2^8 = 256``.
* Simplified function syntax, e.g. ``sqrt 2`` [#simplified_function]_.
* Binary operator ``\`` (integer division).
* Binary operators ``*`` (multiplication), ``/`` (division), unary operators ``+`` and ``-`` (negation), implied multiplication [#implied_mult]_, unary operator ``%`` (percent) [#percent]_.
* Binary operator ``+`` (addition), binary operator ``-`` (subtraction).
* Binary operators ``<<`` (left shift) and ``>>`` (right shift); see also :func:`shl` and :func:`shr`.
* Binary operator ``&`` (bitwise and); see also :func:`and`.
* Binary operator ``|`` (bitwise or); see also :func:`or`.
* Binary operator ``->`` or ``in`` (unit conversion). Again, both notations are equivalent. See the chapter about units for more information.


.. only:: html

    Some of these require a few details to be mentioned:

.. [#simplified_function] 
    .. deprecated :: 0.12
        *Simplified function syntax* refers to the notation where the parentheses are omitted from a function call, e.g. ``sin 123``. The use of this feature is **discouraged**, as it easily allows for errors to creep in. Although deprecated as of version .12, it is still available, but subject to be removed from a later version of SpeedCrunch.

.. [#implied_mult]
    .. versionadded :: 0.12
        *Implicit multiplication* is the syntax feature where the multiplication operator ``*`` may be omitted, for instance in ``3 sqrt(2)``. 
    
.. [#percent]
    .. deprecated :: 0.12
        The percent operator ``%`` has been removed: it was confusing and not very useful. For more details see `issue #239 <issue239_>`_, where the reasons for its removal are discussed in more detail. In versions prior to 0.12, the percent operator would simply act as a multiplication by 0.01, e.g. ``10% = 0.1``, or ``5 - 20% = 4.8``.
    
.. _issue239: https://bitbucket.org/heldercorreia/speedcrunch/issues/239/more-intuitive-and-useful-percentage

