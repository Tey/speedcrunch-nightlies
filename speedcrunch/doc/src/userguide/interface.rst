User Interface
==============
.. _context-help :

Widgets
-------

Apart from the main display, SpeedCrunch offers a number of extra panels and tools, here simply referred to as *widgets*. They may be enabled and disabled via the menu :menuselection:`View`.

* Formula book
    The formula book serves as a library for commonly used formulas and calculations. You can insert any formula into the expression editor by clicking on it.
    
    You can help expanding the formula book by posting your requests to the `issue tracker <tracker_>`_.
* Constants
    The constants dock allows the insertion of one out of over 150 scientific constants. Just double click on an entry to paste it into the editor.
    
    As of version 0.12, the constants have not yet been adapted to make use of the new unit system. All the inserted values thus lack a unit. This is likely to change in a future version.
* Variables
    The variables panel lists all the user-defined variables. Again, any of them may be inserted by double clicking. Additionally, it is possible to delete a variable by selecting it, and pressing the delete button :kbd:`DELETE` on your keyboard.
* (User-) Functions
    Very similar to the variables widget, a widget exists for built-in respectively for user-defined functions.
* Bit Field
    The bit field widget serves to facilitate calculations with binary numbers. Each of the squares represents a bit. You may toggle the corresponding bit by clicking it. The resulting number is automatically inserted into the editor. Additional buttons allow you to shift, invert and reset all the bits at once.
    
    If the mouse cursor is hovered over the bit field widget, scrolling the mouse wheel will also shift the bits.
* Keypad
    Except for version 0.11, an on-screen keypad is available. It allows the input of numbers even without a physical keyboard. The use of the keypad is discouraged, as most of SpeedCrunch's more advanced features cannot be accessed by it.
    
* History
    The history dock lists all previous inputs. Double click a line to recall it. Note that the main result display also supports this functionality.
    
.. _tracker: https://bitbucket.org/heldercorreia/speedcrunch/issues
    
Special Features of the Expression Editor
-----------------------------------------

* Autocompletion
    If you start typing a name (for instance of a variable, function or unit), a list will pop up, suggesting possible names. You may press :kbd:`Tab` or :kbd:`Enter` on your keyboard to automatically insert the first suggestion. Alternatively, use the arrow keys to select a different suggestion, or continue typing to refine the list.
* Constant quick insertion
    Press :kbd:`Ctrl` + :kbd:`Space` to open the *constant quick insertion* box. It allows access to the same constants as the constants dock (see above). Use the keyboard arrows to navigate the list.
* Context help
    Pressing :kbd:`F1` will show the manual page for the function under the cursor. This is extremely convenient when you need help to look up the usage of a function. Just press :kbd:`Esc` to dismiss the manual window again.
* Selection results
    If :menuselection:`Settings --> Behavior --> Partial Results` is enabled, selecting a partial expression in the expression editor will show
    you the result of the selected expression.



Import/Export
-------------

SpeedCrunch can save/export your session in a number of ways. The :menuselection:`Session --> Save` and :menuselection:`Session --> Load` allow you to easily save and restore your entire SpeedCrunch session. The data is stored in a JSON file.

The JSON file presents one major inconvenience: while it is human readable, it is very complete and contains many infromation specific to SpeedCrunch that are not necessarily interesting to another application or user. If you want to export your calculations for e.g. working on them in another program, or handing them to a colleague, it is easier to use on of the alternative export options.

You can save the session as HTML (:menuselection:`Session --> Export --> HTML`). The file created this way can be viewed in any web browser. It shows whatever was visible in the result display at the time the export was performed. Note that although SpeedCrunch currently does not offer a print feature, you may print the exported HTML documents. Note that since the colorscheme is maintained upon export, it is wise to enable a colorscheme with a white background (e.g. *Standard*) if you inted to print the document.

The last (and most basic) option is to export your session a text file (:menuselection:`Session --> Export --> Plain text`). In contrast to the HTML export, formatting like syntax highlighting and font will be lost.

SpeedCrunch also offers capabilities to *import* a session (:menuselection:`Session --> Import`). Select any plain text file, and SpeedCrunch will try to evaluate each line of the file as if the user entered it directly.


Settings
--------

Result Format
+++++++++++++

This section allows selecting the result format to use. You can select one of the following
formats:

* :menuselection:`Decimal --> General`
    Use fixed decimal form for most results; for very large (more than six integer places) or very small results (less than 0.0001),
    scientific notation will be used.
* :menuselection:`Decimal --> Fixed Decimal`
    Display results in decimal form, i.e. as ``<integer>.<fraction>``. For excessively
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

In addition to the primary result format, some other options are available that are independent
of the chosen format:

* :menuselection:`Precision`
    Select the number of fractional digits to display.
    **Automatic** always displays as many digits as are necessary to represent the number
    precisely. The other settings explicitly specify a certain number of digits and will
    append additional zeroes to the fraction to reach that number of digits, if necessary.
* :menuselection:`Radix Character`
    Select the decimal separator to use in results. If :menuselection:`Settings --> Behavior --> Detect All Radix Characters` is
    disabled, this is also the radix character that needs to be used when entering numbers.
    This can either be explicitly set to period (``.``) or comma (``,``), or the system default
    setting can be used.


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

This section contains several settings that allow you to configure SpeedCrunch's behavior.

* :menuselection:`Save History on Exit`
    Controls if the calculation history is automatically saved when SpeedCrunch exits
    and restored when it is launched again.
* :menuselection:`Save Window Position on Exit`
    Controls if the window position is saved and restored.
* :menuselection:`Partial Results`
    If set, SpeedCrunch will display partial results as you type your expression as well
    as results when selecting a partial expression in the editor.
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
* :menuselection:`Detect All Radix Characters`
    If enabled, both periods (``.``) and commas (``,``) are interpreted as decimal separators.
    If disabled, only the radix character set in :menuselection:`Settings --> Result Format --> Radix Character`
    is accepted.
* :menuselection:`Strict Digit Groups Detection`
    If enabled, only known digit grouping characters are allowed. If disabled, any non-alphanumeric character
    that's not an operator can be used for digit grouping.
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
* :kbd:`Ctrl` + :kbd:`L`
    Load session.
* :kbd:`Ctrl` + :kbd:`S`
    Save session.
* :kbd:`Ctrl` + :kbd:`Q`
    Quit SpeedCrunch.
* :kbd:`ESC`
    Clear expression.
* :kbd:`Ctrl` + :kbd:`N`
    Clear history.
* :kbd:`Ctrl` + :kbd:`R`
    Copy last result to clipboard.
* :kbd:`Ctrl` + :kbd:`C`
    Copy selected text to clipboard.
* :kbd:`Ctrl` + :kbd:`V`
    Paste from clipboard.
* :kbd:`Ctrl` + :kbd:`A`
    Select entire expression.
* :kbd:`Ctrl` + :kbd:`V`
    Paste from clipboard.
* :kbd:`Ctrl` + :kbd:`P`
    Wrap the current selection in parentheses. If no text is selected, the entire expression is wrapped.

Widgets and Docks
+++++++++++++++++

* :kbd:`Ctrl` + :kbd:`1`
    Show/hide formula book.
* :kbd:`Ctrl` + :kbd:`2`
    Show/hide constants dock.
* :kbd:`Ctrl` + :kbd:`3`
    Show/hide functions dock.
* :kbd:`Ctrl` + :kbd:`4`
    Show/hide variables dock.
* :kbd:`Ctrl` + :kbd:`5`
    Show/hide user functions dock.
* :kbd:`Ctrl` + :kbd:`6`
    Show/hide bit field widget.
* :kbd:`Ctrl` + :kbd:`7`
    Show/hide history dock.
* :kbd:`Ctrl` + :kbd:`B`
    Show/hide the status bar.
* :kbd:`Ctrl` + :kbd:`K`
    Show/hide the keypad.
    
Scrolling
+++++++++

* :kbd:`PageUp` and :kbd:`PageDown`
    Scroll the result window, page by page.
* :kbd:`Shift` + :kbd:`PageUp` and :kbd:`Shift` + :kbd:`PageDown`
    Scroll the result window, line by line.
* :kbd:`Ctrl` + :kbd:`PageUp` and :kbd:`Ctrl` + :kbd:`PageDown`
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
* :kbd:`Ctrl` + :kbd:`.`
    Use a point as decimal separator.
* :kbd:`Ctrl` + :kbd:`,`
    Use a comma as decimal separator. 
    
Various
+++++++

* :kbd:`F1`
    Show context help (dismiss with :kbd:`Esc`).
* :kbd:`F11`
    Enable fullscreen. Press again to exit.
* :kbd:`Ctrl` + mouse wheel or :kbd:`Shift` + mouse wheel or :kbd:`Shift` + :kbd:`Up`/\ :kbd:`Down`
    Change the font size (zoom).
* :kbd:`Ctrl` + :kbd:`Shift` + mouse wheel
    Change the window opacity.