User Interface
==============

Widgets
-------

Apart from the main display, SpeedCrunch offers a number of extra panels, referred to as *widgets* here.
They can be enabled and disabled via the :menuselection:`View` menu.

* Formula Book
    The formula book provides access to commonly used formulas and calculations. Simply insert
    a formula into the expression editor by clicking on it.

    You can help expanding the formula book by posting your requests to the `issue tracker <tracker_>`_.

* Constants
    The constants widget shows a list of over 150 scientific constants. Just double-click on an entry
    to paste it into the editor.

    .. note::
       As of version 0.12, the constants have not yet been adapted to make use of the new unit system.
       All the inserted values thus lack a unit. This is likely to change in a future version.

* Variables
    The variables widget lists all :ref:`user-defined variables <variables>`. Any of them can be inserted into the editor by double-clicking it.
    Additionally, it is possible to delete a variable by selecting it and pressing the :kbd:`Delete` key on your keyboard.

* Functions and User Functions
    Similar to the variables widget, these show built-in and :ref:`user-defined functions <user_functions>` respectively.

* Bit Field
    The bit field widget is designed to make working with binary numbers easier. It shows a field of 64 squares,
    each representing a bit in the current result. Any bit can be toggled by clicking its square; the resulting
    number is automatically inserted into the editor. Additional buttons allow you to shift, invert and reset all the bits at once.
    While the mouse cursor is hovering over the bit field, scrolling the mouse wheel will also shift the bits.

  .. _keypad:
* Keypad
    The on-screen keypad allows inputting numbers without using the keyboard. However, it is very limited and doesn't provide
    access to many of SpeedCrunch's more advanced features. For that reason, using SpeedCrunch's keyboard interface is recommended
    in place of using the keypad.

    .. versionchanged:: 0.11
       The keypad was removed in SpeedCrunch 0.11; however, it was added back in 0.12.

* History
    The history widget lists all previous inputs. Double-click a line to recall it. Note that the main result display also provides this functionality.

.. _tracker: https://bitbucket.org/heldercorreia/speedcrunch/issues


Expression Editor Features
--------------------------

The expression editor provides some advanced features:

* Autocompletion
    If you start typing a name (e.g. of a variable, function, or unit), a pop-up with matching names will appear. Pressing :kbd:`Tab` or :kbd:`Enter`
    will automatically insert the first suggestion. Alternatively, you can use the arrow keys or the mouse to select a different suggestion, or continue
    typing to refine the list.

* Quick constant insertion
    Press :kbd:`Control+Space` to open a list of constants that allows quick access to the same constants as the constants widget (see above).
    Use the arrow keys to navigate the list.

  .. _context-help:
* Context help
    Pressing :kbd:`F1` will show the manual page for the function under the cursor, providing quick access to detailed
    usage information for a function. Pressing :kbd:`Escape` will dismiss the manual window again.

* Selection results
    If :menuselection:`Settings --> Behavior --> Partial Results` is enabled, selecting a partial expression in the expression editor will show
    you the result of the selected expression.


Import/Export
-------------

SpeedCrunch can save/export your session in a number of ways. The :menuselection:`Session --> Save` and :menuselection:`Session --> Load` menu entries
allow you to easily save and restore your entire SpeedCrunch session. The data is stored in a SpeedCrunch-specific file format. [#f1]_
While the session files are human-readable, they are designed for use by SpeedCrunch. If you want to export your
calculations to work on them in another program or hand them to a colleague, the other export options are preferable.

You can save the session as HTML (:menuselection:`Session --> Export --> HTML`). The resulting file will consist of the contents of the result
display and can be viewed in any web browser. This feature can also be used to print a SpeedCrunch session by printing the exported
HTML document. Since the syntax highlighting and color scheme are maintained in the HTML output, it is recommended to select a color scheme
with a white background (e.g. *Standard*) prior to exporting if you intend to print the document.

The final, most basic option is to export your session as a plain text file (:menuselection:`Session --> Export --> Plain text`).
In contrast to the HTML export option, the syntax highlighting will be lost.

SpeedCrunch also offers capabilities to *import* a session from a text file (:menuselection:`Session --> Import`).
Select any plain text file and SpeedCrunch will try to evaluate each line of the file as if the user entered it directly.


Settings
--------

SpeedCrunch's behavior can be customized to a large degree using the configuration options in the
:menuselection:`Settings` menu. This section explains the settings that are available.

.. _result_format:

Result Format
+++++++++++++

This section allows selecting the result format to use. You can select one of the following
formats:

* :menuselection:`Decimal --> General`
    Use fixed decimal form for most results; for very large (more than six integer places) or very small results (less than 0.0001),
    scientific notation will be used.
* :menuselection:`Decimal --> Fixed Decimal`
    Display results in fixed decimal form. For excessively
    large or small numbers, this format may still fall back to scientific notation.
* :menuselection:`Decimal --> Engineering`
    Display results in engineering notation. This is a variant of :ref:`scientific notation <scientific_notation>` in which
    the exponent is divisible by three.
* :menuselection:`Decimal --> Scientific`
    Display results in :ref:`normalized scientific notation <scientific_notation>`.
* :menuselection:`Binary`
    Display results as binary numbers, i.e. in base-2.
* :menuselection:`Octal`
    Display all results as octal numbers, i.e. in base-8.
* :menuselection:`Hexadecimal`
    Display all results as hexadecimal numbers, i.e. in base-16.

In addition to the main result format, some further options are available to customize
the result output:

* :menuselection:`Precision`
    Select the number of fractional digits to display.
    **Automatic** always displays as many digits as are necessary to represent the number
    precisely. The other settings explicitly specify a certain number of digits and will
    append additional zeroes to the fraction to reach that number of digits, if necessary.

Input Format
+++++++++++++

This section allows selecting the input format to use.

.. _radix_character:

* :menuselection:`Radix Character`
    Select the decimal separator to use in inputs and results. This can either be explicitly set
    to dot (``.``), or comma (``,``), or both, or system default. When both dot and comma are used,
    the decimal separator is detected automatically in inputs and the system default is used
    in results. With that latter mode, mixing both dot and comma in a same number to express the
    decimal separator and digit group separators is supported, but might lead to unexpected results.

Angle Unit
++++++++++

Select the angular unit to be used in calculations. For functions that operate on angles, notably the
:ref:`trigonometric functions <trigonometric>` like :func:`sin` or :func:`cos`, this setting
determines the angle format of the arguments.

* :menuselection:`Radian`
    Use radians for angles. A full circle corresponds to an angle of 2π radians.
* :menuselection:`Degree`
    Use degrees for angles. A full circle corresponds to an angle of 360°.


Behavior
++++++++

This section contains several settings that allow you to further configure SpeedCrunch's behavior.

* :menuselection:`Save History on Exit`
    Controls if the calculation history is automatically saved when SpeedCrunch exits
    and restored when it is launched again.
* :menuselection:`Save Window Position on Exit`
    Controls if the window position is saved and restored.
* :menuselection:`Partial Results`
    If set, SpeedCrunch will display partial results as you type your expression as well
    as results when selecting a partial expression in the editor.

  .. _automatic_result_reuse:
* :menuselection:`Automatic Result Reuse`
    This setting allows you to quickly continue typing after evaluating an expression
    by inserting `ans` into the editor if necessary.
* :menuselection:`Automatic Completion`
    Completely enables or disables autocompletion.
* :menuselection:`Syntax Highlighting`
    Enables or disables syntax highlighting.
* :menuselection:`Digit Grouping`
    Visually group digits in long numbers. Requires :menuselection:`Settings --> Behavior --> Syntax Highlighting` to be
    enabled.
* :menuselection:`Leave Last Expression`
    If selected, the entered expression remains in the editor after evaluating it.
* :menuselection:`Enable Complex Numbers`
    Enable or disable support for :ref:`complex numbers <complex_numbers>`. If disabled, :const:`j` will be undefined
    and expressions that would return complex results (like ``sqrt(-1)``) will fail.
* :menuselection:`Always on Top`
    Keep the SpeedCrunch window on top of other windows.
* :menuselection:`Automatic Result to Clipboard`
    Automatically copy the result to the clipboard whenever an expression is evaluated.


User Interface Settings
+++++++++++++++++++++++

* :menuselection:`Settings --> Display --> Color Scheme`
    Select a color scheme. See :ref:`color_schemes` for information on how to install
    additional color schemes so they are displayed in this menu.
* :menuselection:`Settings --> Display --> Font`
    Select the font to use for the expression editor and result display.
* :menuselection:`Settings --> Language`
    Select the user interface language.


Keyboard Shortcuts
------------------

Editing
+++++++
* :kbd:`Control+L`
    Load session.
* :kbd:`Control+S`
    Save session.
* :kbd:`Control+Q`
    Quit SpeedCrunch.
* :kbd:`Escape`
    Clear expression.
* :kbd:`Control+N`
    Clear history.
* :kbd:`Control+C`
    Copy selected text to clipboard.
* :kbd:`Control+R`
    Copy last result to clipboard.
* :kbd:`Control+V`
    Paste from clipboard.
* :kbd:`Control+A`
    Select entire expression.
* :kbd:`Control+P`
    Wrap the current selection in parentheses. If no text is selected, the entire expression is wrapped.

Widgets and Docks
+++++++++++++++++

* :kbd:`Control+1`
    Show/hide formula book.
* :kbd:`Control+2`
    Show/hide constants widget.
* :kbd:`Control+3`
    Show/hide functions widgets.
* :kbd:`Control+4`
    Show/hide variables widget.
* :kbd:`Control+5`
    Show/hide user functions widget.
* :kbd:`Control+6`
    Show/hide bit field widget.
* :kbd:`Control+7`
    Show/hide history widget.
* :kbd:`Control+B`
    Show/hide the status bar.
* :kbd:`Control+K`
    Show/hide the keypad.

Scrolling
+++++++++

* :kbd:`Page Up` and :kbd:`Page Down`
    Scroll the result window page-wise.
* :kbd:`Shift+Page Up` and :kbd:`Shift+Page Down`
    Scroll the result window line-wise.
* :kbd:`Control+Page Up` and :kbd:`Control+Page Down`
    Scroll to the top or bottom of the result window.


Format
++++++

* :kbd:`F2`
    Set result format to general decimal.
* :kbd:`F3`
    Set result format to fixed decimal.
* :kbd:`F4`
    Set result format to engineering decimal.
* :kbd:`F5`
    Set result format to scientific decimal.
* :kbd:`F6`
    Set result format to binary.
* :kbd:`F7`
    Set result format to octal.
* :kbd:`F8`
    Set result format to hexadecimal.
* :kbd:`F9`
    Set angle unit to radian.
* :kbd:`F10`
    Set angle unit to degree.
* :kbd:`Control+.`
    Use a period as decimal separator.
* :kbd:`Control+,`
    Use a comma as decimal separator.

Various
+++++++

* :kbd:`F1`
    Show context help (dismiss with :kbd:`Escape`).

    .. versionadded:: 0.12

* :kbd:`F11`
    Toggle full screen.
* :kbd:`Control` + mouse wheel, :kbd:`Shift` + mouse wheel, or :kbd:`Shift+Up` and :kbd:`Shift+Down`
    Change the font size.
* :kbd:`Control+Shift` + mouse wheel
    Change the window opacity.

    .. versionadded:: 0.12


.. rubric:: Footnotes

.. [#f1] Starting with SpeedCrunch 0.12, the session format is based on `JSON <json_>`_. Previous
         versions used a simple custom text format.

.. _json: http://json.org/
