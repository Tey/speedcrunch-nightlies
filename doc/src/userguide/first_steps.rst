First Steps
===========

SpeedCrunch is designed to be used via the keyboard. While an :ref:`on-screen keypad <keypad>` is available,
it is disabled by default. The powerful keyboard interface is the recommended way to
use SpeedCrunch to get the most out of it. However, the keypad can be enabled from:
:menuselection:`View --> Keypad`.

.. This note included since the code samples aren't translatable; people in comma locales
   might get confused.

.. note::

   All examples in this manual use a period (``.``) as the decimal separator. By default,
   SpeedCrunch will use your system's default decimal separator for output and will
   accept both a period and a comma (``,``) for input (:ref:`more details <radix_character>`).
   Depending on your settings, it may be necessary to replace the periods with the
   appropriate decimal separator when entering these examples into SpeedCrunch.

When starting SpeedCrunch for the first time, the interface may thus seem a little plain. Don't be fooled, it can easily be customized. At the bottom edge of the window, you see the the **input bar**, also called the **editor**, with a blinking cursor showing that it is ready to accept keystrokes. To try out SpeedCrunch, type::

    1.234 + 7.89

then hit :kbd:`Enter` to evaluate the expression. The result (along with the expression you just entered) will be displayed at the top of window::

    1.234 + 7.89
    = 9.124

This region (called the **result window**) not only shows the result of your last calculation, it is a complete browsable history. Any previous input or result can be recalled by double-clicking the corresponding line.

Let's try this out. First, perform some more calculations so that more than one entry is displayed in the result window::

    54*4.514 + 57
    = 300.756

    54*(4.514 - 5.7)
    = -64.044

Note how multiplication (``*``) takes precedence over addition (``+``) unless parentheses are used to specify a particular order of evaluation. If you now wish to continue working with one of the results, double-click e.g. the line ``= 300.756``. The input bar will be populated with this result. Similarly, you can also quickly recall previous input by double-clicking them in the result history. Alternatively, you can use the :kbd:`Up` and :kbd:`Down` arrow keys on your keyboard to recall previous expressions or use the mouse wheel for the same effect (the mouse cursor must be hovering over the editor for this to work, otherwise the result window will be scrolled).

To continue the computation directly from the previous result, you can use the special variable ``ans`` which always holds the result of the last entry in the history. Try this::

    2+3         ? By the way, you can use question marks for comments.
    = 5

    7*ans       ? 'ans' is now equal to 5
    = 35

    ans + 300
    = 38

As you can see, ``ans`` provides a quick and easy way to access the previous result. Sometimes,
SpeedCrunch can even insert ``ans`` for you: When you start a new expression with an operator (e.g. ``+``, ``-``, ``*``, ``/``, ``^``), ``ans`` will be prepended automatically. [#f1]_ To try it out, type ``7`` :kbd:`Enter` ``+3`` :kbd:`Enter`.
SpeedCrunch expands this as follows::

    7
    = 7

    ans+3
    = 10

SpeedCrunch comes with a number of :ref:`mathematical constants <constants>` like π that are available by typing their name::

    pi
    = 3.14159265358979323846

    pi*1.2^2
    = 4.52389342116930226339

Let us finish this short introduction with a word about functions. SpeedCrunch comes with an extensive list of built-in mathematical functions which can be accessed by simply typing their name in the editor followed by any parameters (also called arguments) in parentheses::

    sqrt(144)
    = 12

The function :func:`sqrt` calculates the square root of its argument. Some functions require or accept more than one argument. In that case, separate them with a semicolon (``;``).

.. warning::

   To repeat: In contrast to many programming languages, parameters in SpeedCrunch are separated by a **semicolon**!

An example is the :func:`average` function::

    average(12;34;56)
    = 34

This is already enough to use SpeedCrunch effectively. Try it out for yourself! Alternatively,
read on to learn about its more advanced features.


.. rubric:: Footnotes

.. [#f1] This requires that the :ref:`Automatic Result Reuse <automatic_result_reuse>` setting
         is enabled.
