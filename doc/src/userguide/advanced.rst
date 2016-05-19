User-defined Variables and Functions
====================================

Variables
---------

When working on a more sophisticated problem, you will find yourself in the situation where you frequently need to access results from previous computations. As we have already seen, you can simply recall results from the result window. SpeedCrunch offers another, much more powerful way: variables. Variables allow you to store and recall any number, by assigning it a name. For instance, for storing data, try this::

    a = 5.123
    
Now, you may access this value by the name ``a``, much the same as you would use some of the built-in constants like :const:`pi`.

Naturally, when assigning a value, the right-hand-side can be an arbitrarily complex expression::

    mass = 100+20
    = 120

    g = 9.81
    = 9.81

    weight = mass*g
    = 1177.2
    
    somethingelse = ln(sqrt(123) + ans)
    = 7.08027102937165690787
    
As you see, using descriptive variable names makes the calculation history much more readable.


User-Functions
--------------
.. versionadded :: 0.12

Just as you can define your own variables, SpeedCrunch also allows you to define your own functions. Although the list of integrated functions (:ref:`sc:functionindex`) is quite extensive, defining your own is most useful when you find yourself repeating a similar computation over and over again.

Doing so is very easy::

    f(x) = 5*x+8
    
You may now use the new function ``f`` just as any of the built-in ones::

    f(5)
    = 33
    
Functions with more arguments are allowed as well, just separate the parameters with a semicolon::
    
    f(x; y) = x*y + x
    
    f(2; 3)
    = 8


.. _complex_numbers:

Complex numbers
===============
.. versionadded :: 0.12

SpeedCrunch supports calculations involving complex numbers. To enable them, make sure that the menu item :menuselection:`Settings --> Behaviour --> Enable Complex Numbers` is checked. The imaginary unit is now available under the name :const:`j`. Try it out::

    j^2
    = -1
    
    (5+3j)/(8-2j)
    = 0.5+0.5j
    
A word on the syntax of complex numbers: ``5j`` denotes the number ``5*j``, while ``j5`` is a variable named 'j5'. Consider writing the multiplication explicitly, i.e. ``j*5``.

Some users (especially mathematicians or physicists) may prefer the letter ``i`` to stand for the imaginary unit. Although SpeedCrunch does not support this notation by default, it added is easily enough. Just define ``i=j``, then ``i`` may be used as a alias for :const:`j`.

Not every function in SpeedCrunch supports complex arguments. Refer to the reference of the corresponding function for more information.

Special caution is advised when using functions like :func:`cbrt` or any fractional power operation in general. With complex numbers enabled, the power operation ``x^(1/3)`` will return the first complex cubic root of ``x``, which in general is non-real. When given a real argument, :func:`cbrt` will on the other hand *always* return the *real* cubic root, regardless of whether or not complex numbers are enabled.

When complex numbers are disabled, the constant :const:`j` is not available. However, it may be that previously stored variables still contain complex values. In that case, the imaginary part of these numbers is discarded when passing them as an argument to any built-in function.

Units
=====
.. versionadded :: 0.12

SpeedCrunch boasts a powerful system for units and conversions. It hosts an extensive list of built-in units, and easily allows you to define your own.

Just as in common textbook notation, you specify a quantity's unit by multiplication::
    
    5 foot
    = 1.524 meter
    
Note that this really is a multiplication, the operator ``*`` is just omitted (implied)! As you can see, by default SpeedCrunch converts the quantity into SI units::
 
    60 mile/hour
    = 26.8224 meter second^-1
    
This alone would not yet be terribly useful. However, you may specify the target unit by using the conversion operator ``->`` (or ``in`` as an alias)::

    50 yard + 2 foot  in centi meter
    = 4632.96 centi meter
    
    10 knot -> kilo meter / hour
    = 18.52 (kilo meter/hour)
    
Note that all built-in units' names are **singular** and spelled out in **American English**. This is regardless of the language selected for SpeedCrunch's interface.

As you can see, you may also use any of the SI prefixes (like ``kilo`` or ``centi`` in the example above): they are treated just as any other unit, so just separate them with a space (multiplication!) from the base unit they refer to.

.. warning ::
    In SpeedCrunch (unlike in textbook notation) prefixes do have a meaning on their own. This limitation (or feature, you decide) means that the same order of operations applies to them as to any other unit. For instance, if you intend to express a spring stiffness in newtons per centimeter, be sure to type ``newton/(centi meter)``, rather than ``newton / centi meter``.
    
    Similarly, be aware that expressions like ``500 gram / 20 gram`` and ``(500 gram) / (20 gram)`` yield different results.
    
An important feature of SpeedCrunch's unit system is *dimensional checking*. Basically, it prevents comparison between apples and pears: if you try to convert seconds to meters, SpeedCrunch will complain, saying that the dimensions of ``second`` and ``meter`` do not match. Indeed, the dimension of ``second`` is *time*, while ``meter`` denotes a *length*, thus they cannot be compared, added, etc. When you add, multiply, etc. units, SpeedCrunch will track the dimension, and raise an error if you try to perform an illegal operation. For instance if you type ``meter^2`` the result will be a quantity of dimension *length*\ :sup:`2`, and can only be compared to other quantities of the same dimension. Currently the implemented dimensions, and their primitive units are

* *length*, ``meter``;
* *mass*, ``kilogram``;
* *time*, ``second``;
* *el. current*, ``ampere``;
* *amount*, ``mole``;
* *luminous intensity*, ``candela``;
* *temperature*, ``kelvin``;
* *information*, ``bit``.

Temperature is special: the current unit system does not support non-linear units like °C, °F. Consequently, ``kelvin`` is currently the only unit for temperature that is available. A future version of SpeedCrunch might introduce a workaround to this.

Defining your own units is very straightforward::

    earth_radius = 6730 kilo meter
    
    3.5 astronomical_unit in earth_radius
    = 77799.78416790490341753343 earth_radius
    
In fact, any variable or even expression may be used as the right-hand-side of the conversion operator::
    
    10 meter in (1 yard + 2 foot)
    = 6.56167979002624671916 (1 yard+2 foot)
    
As mentioned, built-in units are spelled out; although this avoids ambiguity, longer expressions become tedious to input and hard to read. If you find yourself using a particular set of units often, consider defining for instance ::
    
    m = meter
    cm = centi meter
    ft = foot
    
and save yourself some typing.


Some of the built-in functions are able to handle arguments with a dimension. Refer to the reference on the particular function for more information.