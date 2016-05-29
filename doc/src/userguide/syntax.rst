Syntax
======

This part of the documentation explains the syntax of valid SpeedCrunch input. As you will see, SpeedCrunch honors most conventions for mathematical expressions. You will find using SpeedCrunch to be very natural and intuitive, especially so if you are already familiar with a programming language.


Number Notation
---------------

Decimal Form
++++++++++++

When you would like to specify a non-integer value, simply enter the number as you would write it on paper,
with either a period (``.``) or a comma (``,``) as the decimal separator. By default, these can be
used interchangeably, i.e. both ``1.234`` and ``1,234`` represent the same number. However, this
behavior can be changed; see :ref:`Radix Character <radix_character>` for more information.

Trailing zeros after the decimal point (like in ``12.300``) or leading zeros before it (``0012.3``) are redundant and can be included or omitted to the user's preference. Expressions like ``.5`` as a shorthand notation for ``0.5`` are also permitted.


.. _scientific_notation:

Scientific Notation
+++++++++++++++++++

When dealing with very small or very large numbers (think the size of an atom or of a galaxy) the notation above is inconvenient. These are more commonly expressed in scientific notation; for instance, 1.234*10\ :sup:`-9` is preferable to 0.000000001234.

Naturally, in SpeedCrunch this could be written as ``1.234*10^-9``, but there's also a shorthand notation: ``1.234e-9``. Here, the ``e`` represents ``*10^``, but it is considered a part of the number literal and treated with higher precedence. For example, ``1e2^3`` is equivalent to ``(1e2)^3 = 100^3``. The scale of a number (sometimes called its exponent) always begins with the scale character ``E`` or ``e`` followed by a signed integer. So ``e+10``, ``e-4``, ``E-0`` are all valid scale expressions. If the sign is '+', you may simply omit it: ``e0``, ``E10``. The significand (i.e. the part preceding the exponent) is required; exactly one exponent must be specified.

Compared to most calculators, SpeedCrunch can accept very large numbers without overflowing (e.g. both ``1e+536870911`` and ``1e-536870911`` are still valid). However, only about 78 significant digits are stored at any point. Any digits beyond that are lost.

Non-Decimal Bases
+++++++++++++++++

In addition to decimal (base-10) numbers, SpeedCrunch provides support for binary (base-2), octal (base-8) and hexadecimal (base-16) numbers.
You can enter a number in any of these bases by marking it with the corresponding prefix:

* ``0b`` or ``0B`` for **binary**, e.g. ``0b10010``.
* ``0o`` or ``0O`` for **octal**, e.g. ``0o1412``.
* ``0d`` or ``0D`` for **decimal**. These can be omitted since decimal is the default base.
* ``0x``, ``0X``, or ``#`` for **hexadecimal**. The additional six digits are represented by the upper or lower case letters ``a`` to ``f``, e.g. ``0xdeadbeef`` or ``0xDEADBEEF``.

You may even enter fractional values in any of these bases. Note that scientific notation is not
supported for non-decimal bases, however. Examples::

    0b1.01
    = 1.25

    0xf.a
    = 15.625

To have SpeedCrunch output its results in a base other than decimal, you may use one of the functions :func:`bin`, :func:`oct`, :func:`dec`, or :func:`hex`::

    hex(12341)
    = 0x3035

The effect of these functions only applies to the immediate result and doesn't carry to future
operations::

    0x2 * hex(12341)
    = 24682

To change the base that is used for displaying results, select one of the corresponding settings in :menuselection:`Settings --> Result Format`.

SpeedCrunch stores integers with a precision of up to 256 bits. Since this would be unwieldy,
the binary representation of a negative number in SpeedCrunch is *not* its two's complement.
Instead, like with other bases, the value and the sign are represented separately::

    bin(-1)
    = -0b1

See :func:`mask` and :func:`unmask` to convert a negative number into the two's complement form.

Any integer larger than the 256-bit limit will be silently converted into a floating point number, making it susceptible to rounding errors.
To specify large integers, using the shift operators (``1 << n``) is preferable to exponentiation (``2 ^ n``) as the latter are floating point
calculations and thus susceptible to rounding errors.


Operators and Precedence
------------------------

When writing an expression like ``10+5*4``, which operation will be executed first? The common rules of operator precedence tell us that in this case multipication shall be computed first, hence the result is ``30``. We also distinguish **unary** operators (which act on a single number/operand) and **binary** operators (which link two operands).

SpeedCrunch supports the following operators, listed in order of decreasing precedence:

.. Note: When making changes to these tables, also check that they look ok with LaTeX; these big
.. tables can be problematic.

.. tabularcolumns:: |p{0.2\linewidth}|p{0.5\linewidth}|p{0.25\linewidth}|

+-------------------------------+---------------------------------------------------------------+-------------------------+
| Operator                      | Description                                                   | Examples                |
+===============================+===============================================================+=========================+
| ``(...)``                     | **Parentheses**                                               | ``(2+3)*4 = 5*4 = 20``  |
|                               |   Parentheses mark precedence                                 |                         |
|                               |   explicitly.                                                 |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``x!``                        | **Factorial**                                                 | ``5! = 120``            |
|                               |   Computes the factorial of its                               |                         |
|                               |   argument. See also :func:`gamma()`.                         |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``a ^ b``, ``a ** b``         | **Exponentiation**                                            |                         |
|                               |   Both variants are equivalent. Note                          |                         |
|                               |   that the power operation is                                 |                         |
|                               |   *right-associative*, i.e. it is                             | ``2^2^3 = 2^8 = 256``   |
|                               |   evaluated from right to left.                               |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``+x``, ``-x``                | **Unary plus and minus**                                      | ``--5 = +5``            |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``a \ b``                     | **Integer division**                                          | ``5\4 = 1``             |
|                               |   Divides the operands and truncates                          |                         |
|                               |   the result to an integer.                                   |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``a * b``, ``a b``, ``a / b`` | **Multiplication and division**                               | ``3 sqrt(2)``           |
|                               |   In many situations, *implicit                               |                         |
|                               |   multiplication* allows writing                              |                         |
|                               |   multiplications without the ``*``                           |                         |
|                               |   operator.                                                   |                         |
|                               |                                                               |                         |
|                               | .. versionadded:: 0.12                                        |                         |
|                               |    Implicit multiplication was added                          |                         |
|                               |    SpeedCrunch 0.12.                                          |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``a + b``, ``a - b``          | **Addition and subtraction**                                  |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``a << n``, ``a >> n``        | **Left/right arithmetic shifts**                              | ``0b11<<1 = 0b110``     |
|                               |   Shifts the first operand left/right                         |                         |
|                               |   by ``n`` bits. See also :func:`shl`                         | ``0b100>>2 = 0b1``      |
|                               |   and :func:`shr`.                                            |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``a & b``                     | **Bitwise AND**                                               | ``0b11 & 0b10 = 0b10``  |
|                               |   See also :func:`and`.                                       |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``a | b``                     | **Bitwise OR**                                                | ``0b10 | 0b01 = 0b11``  |
|                               |   See also :func:`or`.                                        |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``->``, ``in``                | **Unit conversion**                                           | ``1000 meter in mile``  |
|                               |   Convert the operand into the given                          |                         |
|                               |   unit. Both forms are equivalent. See                        | ``1000 meter -> mile``  |
|                               |   :ref:`units` for more information.                          |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+


.. We want to keep the following heading, paragraph and table together. By forcing a page break
.. here, we avoid LaTeX squeezing the paragraph onto the same page as the previous large table
.. and then running out of space for the next table and moving it onto the next page.

.. raw:: latex

    \pagebreak


Deprecated Operators
--------------------

The following operators used to be supported, but were either removed from recent SpeedCrunch
versions or are considered deprecated. Generally, these features were removed because of significant problems
so you may want to avoid them even if they're still supported in your version of SpeedCrunch.

.. tabularcolumns:: |p{0.2\linewidth}|p{0.5\linewidth}|p{0.25\linewidth}|

+-------------------------------+---------------------------------------------------------------+-------------------------+
| Operator                      | Description                                                   | Examples                |
+===============================+===============================================================+=========================+
| ``x%``                        | **Percent operator**                                          | ``10% = 0.1``           |
|                               |   Equivalent to multiplication with                           |                         |
|                               |   0.01.                                                       |                         |
|                               |                                                               |                         |
|                               | .. deprecated:: 0.12                                          |                         |
|                               |    This operator was **removed** in                           |                         |
|                               |    SpeedCrunch 0.12 as it was confusing and not very useful.  |                         |
|                               |    The reasons for its removal are discussed in more detail   |                         |
|                               |    in `issue #239 <issue239_>`_.                              |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+
| ``f x``                       | **Simplified function syntax**                                | ``sqrt 2 = sqrt(2)``    |
|                               |   Allows omitting the parentheses when                        |                         |
|                               |   calling a function.                                         |                         |
|                               |                                                               |                         |
|                               | .. deprecated:: 0.12                                          |                         |
|                               |    Use of this feature is                                     |                         |
|                               |    discouraged because it allows                              |                         |
|                               |    for very ambiguous expressions. It                         |                         |
|                               |    will likely be removed in a future release.                |                         |
+-------------------------------+---------------------------------------------------------------+-------------------------+

.. _issue239: https://bitbucket.org/heldercorreia/speedcrunch/issues/239/more-intuitive-and-useful-percentage
