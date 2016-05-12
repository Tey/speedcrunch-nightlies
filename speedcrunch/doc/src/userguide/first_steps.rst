First Steps
===========

SpeedCrunch has been designed to be used via the keyboard. While it is perfectly viable to use it via the mouse (and enter numbers by clicking the buttons on the on-screen keypad), it is highly recommended to get used to the keyboard interface, and harness SpeedCrunch's full power. For this reason the on-screen keypad widget is disabled by default.

.. TODO: link to how to enable the keypad widget.

When starting SpeedCrunch for the first time, the interface may thus seem a little plain. Don't be fooled, it can easily be customized. At the bottom edge of the window, you see the the **input bar**, or the **editor**, with a blinking cursor showing that it is ready to accept keystrokes. To try out SpeedCrunch, type::
   
    1.234 + 7.89
   
then hit :kbd:`ENTER` to evaluate the expression. The result (along with the expression you just entered) will be displayed in the top of window.::

    1.234 + 7.89
    = 9.124
    
This region (called the **result window**) will not only show the result of your last calculation, but a complete browsable history. Any previous input or result be may be recalled, by e.g. double clicking the corresponding line.

Let's try this out. First, input some more calculations, so that more than one entry is displayed in the result window. ::

    54*4.514 + 57
    = 300.756
    
    54*(4.514 - 5.7)
    = -64.044
    
Note how the multiplication ``*`` takes precedence over the addition ``+``, unless parentheses are used to specify a particular order of operations. If you now wish to continue working with one of the results, double-click for instance on ``= 300.756``. The input bar will automatically be populated with this result. Similarly you can also quickly recall previous input, rather than results. Alternatively, you can use the :kbd:`UP` and :kbd:`DOWN` arrow keys on your keyboard to recall previous input expressions, or use the mouse wheel for the same effect (the mouse cursor must hover over the editor for this, otherwise the result window will be scrolled).

For continuing the computation directly from the previous result, you may use the special variable ``ans``, which always holds the result of the last entry in the history. Try this::

    2+3         ? By the way, you can use question marks for comments.
    = 5
    
    7*ans       ? 'ans' is now equal to 5
    = 35
    
    ans + 300
    = 38
    
As you can see, ``ans`` provides a quick and easy way to access the previous result. Sometimes you can even omit the keyword ``ans`` completely. When you start a new expression by either ``+``, ``-``, ``*``, ``/``, ``^``, SpeedCrunch will automatically prepend ``ans``. To try it out, type ``7`` :kbd:`ENTER` ``+3`` :kbd:`ENTER`. SpeedCrunch expands this as follows::

    7
    = 7
    
    ans+3
    = 10

SpeedCrunch comes with a number of mathematical constants like π, that are available by typing their name, in this case :const:`pi`. Example::
    
    pi
    = 3.14159265358979323846

    pi*1.2^2
    = 4.52389342116930226339
    
Let us finish this short introduction with a word about functions. SpeedCrunch comes with an extensive list of built-in mathematical functions, which can be accessed by just typing their name in the editor, followed by their parameter (also called argument) between parentheses. ::

    sqrt(144)
    = 12
    
Here :func:`sqrt` denotes the square root. Some functions require or accept more than one argument. In that case, separate them with a **semicolon** ``;``.

.. warning :: This is an important point, often overlooked by people, so it is repeated here. In SpeedCrunch (unlike many programming languages), parameters are separated by a **semicolon**.

An example is the function :func:`average`::

    average(12;34;56)
    = 34
    
This should be all you need to know to make good use of SpeedCrunch already. Try it out for yourself! Or you can read on about its more advanced features...
