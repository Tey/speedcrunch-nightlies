User-Defined Variables and Functions
====================================


.. _variables:

Variables
---------

When working on more sophisticated problems, you will likely find that you frequently need to access
results from previous computations. As we have already seen, you can simply recall results from the
result window. However, SpeedCrunch also offers another more powerful way: Variables. Variables allow
you to store and recall any value, by assigning it a name. Variables are defined using the :samp:`{variable}={value}` syntax::

    a = 5.123

Now you can access this value via the name ``a`` much like you would use a built-in constant like :const:`pi`.

Naturally, when assigning a value, the right-hand-side can be an arbitrarily complex expression::

    mass = 100+20
    = 120

    g = 9.81
    = 9.81

    weight = mass*g
    = 1177.2

    somethingelse = ln(sqrt(123) + ans)
    = 7.08027102937165690787

As you see, using descriptive variable names can make the calculation history much more readable.


.. _user_functions:

User Functions
--------------
.. versionadded:: 0.12

Just as you can define your own variables, it is also possible to define your own functions. While SpeedCrunch comes with an extensive collection of built-in functions (:ref:`sc:functionindex`), defining
your own functions can be very useful when you find yourself repeating a similar computation over and over again.

Defining a custom function is similar to defining a variable::

    f(x) = 5*x+8

You can now use the new function ``f`` just like any of the built-in ones::

    f(5)
    = 33

Functions with more arguments are possible as well; simply separate the parameters with a semicolon::

    f(x; y) = x*y + x

    f(2; 3)
    = 8


.. _complex_numbers:

Complex numbers
===============
.. versionadded:: 0.12

SpeedCrunch supports calculations involving complex numbers. To use them, make sure that the menu item :menuselection:`Settings --> Behaviour --> Enable Complex Numbers` is checked. The imaginary unit is now available under the name :const:`j`::

    j^2
    = -1

    (5+3j)/(8-2j)
    = 0.5+0.5j

A note on the syntax of complex numbers: ``5j`` denotes the number ``5*j`` while ``j5`` is a variable named 'j5'. If necessary, consider writing the multiplication explicitly, i.e. ``j*5``.

Some users (especially mathematicians or physicists) may prefer to use the letter ``i`` for the imaginary unit. Although SpeedCrunch does not support this notation by default, it is added easily enough.
Simply defining ``i=j`` allows you to use ``i`` as an alias for :const:`j`.

Not every function in SpeedCrunch supports complex arguments. Refer to a function's documentation for more information.

Caution is advised when using functions like :func:`cbrt` or any fractional power operation with complex numbers.
With complex number support enabled, the power operation ``x^(1/3)`` will return the first complex cubic root of ``x`` which is usually non-real.
However, when given a real argument, :func:`cbrt` will *always* return the real cubic root, regardless of whether or not complex numbers are enabled.

When complex numbers are disabled, the constant :const:`j` is not available. However, previously stored variables may still contain complex values.
In that case, the imaginary part of these numbers is discarded when passing them as an argument to a built-in function.


.. _units:

Units
=====
.. versionadded:: 0.12

SpeedCrunch includes a powerful system for units and unit conversions. It provides an extensive list of built-in units and easily allows you to define your own.

Just as in common textbook notation, you specify a value's unit by multiplication::

    5 foot
    = 1.524 meter

Note that this is an actual multiplication internally. However, the ``*`` operator can
often be omitted (using implicit multiplication), as shown by the previous example.
By default SpeedCrunch converts the quantity into SI units::

    60 mile/hour
    = 26.8224 meter second⁻¹

This alone would not be terribly useful. However, it is possible to convert the value to a different unit using the conversion operator ``->``
(``in`` can be used as an alias)::

    50 yard + 2 foot in centi meter
    = 4632.96 centi meter

    10 knot -> kilo meter / hour
    = 18.52 (kilo meter/hour)

Note that all built-in unit names are singular and use American English spelling. This is independent of the language selected for SpeedCrunch's interface.

As seen in the example above, you can use any SI prefix like ``kilo`` or ``centi``.
They are treated like any other unit, so separate them with a space from the base unit they refer to.

.. warning::

   In SpeedCrunch (unlike in textbook notation), prefixes can be used on their own. This limitation (or feature, depending on your point of view)
   means that their use follows the same rules of precedence as any other mathematical operation. For instance, if you intend to express the unit
   'newtons per centimeter', don't simply type ``newton / centi meter`` -- this will be interpreted as ``(newton / centi) meter``! Instead, make
   the order explicit by using ``newton / (centi meter)``.

   For the same reasons, expressions like ``500 gram / 20 gram`` and ``(500 gram) / (20 gram)`` yield different results.

An important feature of SpeedCrunch's unit system is *dimensional checking*. Simply put, it prevents comparing apples and pears: If you try to convert seconds to meters, SpeedCrunch will complain, stating that the dimensions of ``second`` and ``meter`` do not match. Indeed, the dimension of ``second`` is *time*, while ``meter`` denotes a *length*, thus they cannot be compared, added, etc. When adding, multiplying, or otherwise manipulating units, SpeedCrunch will track the dimension and raise an error if it detects an invalid operation. For instance if you type ``meter^2``, the result will be a quantity with the dimension *length*\ :sup:`2` which can only be compared to other quantities with the same dimension. Currently, the available dimensions and their associated primitive units are:

* *Length*: ``meter``
* *Mass*: ``kilogram``
* *Time*: ``second``
* *Electric current*: ``ampere``
* *Amount*: ``mole``
* *Luminous intensity*: ``candela``
* *Temperature*: ``kelvin``
* *Information*: ``bit``

Since the current unit system does not support non-linear units like °C or °F, the only available unit for temperature quantities is ``kelvin``.
This might change in a future version of SpeedCrunch.

Defining a custom unit works exactly like defining a variable::

    earth_radius = 6730 kilo meter

    3.5 astronomical_unit in earth_radius
    = 77799.78416790490341753343 earth_radius

In fact, any unit is simply a user-defined variable or a built-in constant. This also means
that any variable or even expression can be used as the right-hand side of a conversion expression::

    10 meter in (1 yard + 2 foot)
    = 6.56167979002624671916 (1 yard+2 foot)

As mentioned above, the built-in units are spelled out to avoid ambiguity. However, this also means that longer
expressions can become tedious to input and hard to read. If you find yourself using a particular set of units frequently,
consider defining shorter aliases::

    m = meter
    cm = centi meter
    ft = foot

Some of the built-in functions are able to handle arguments with a dimension. Refer to the documentation of a particular function for more information.
