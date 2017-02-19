.. _color_schemes:

Color Scheme File Format
========================

.. versionadded:: 0.12
   Support for custom color schemes using this format was introduced in SpeedCrunch 0.12.

SpeedCrunch uses a simple :abbr:`JSON (JavaScript Object Notation)`-based file format
for color scheme definitions. New color schemes may easily be created, used and shared
without having to recompile SpeedCrunch.

In addition to the color schemes integrated into the application, color scheme files
are loaded from the following directory:

 * Windows: :file:`C:/Users/<USERNAME>/AppData/Roaming/SpeedCrunch/color-schemes`
 * Linux/Unix: :file:`~/.local/share/SpeedCrunch/color-schemes/`
 * OS X: :file:`~/Library/Application Support/SpeedCrunch/color-schemes`
 * When using a portable version of SpeedCrunch on any operating system, additional color schemes
   are loaded from the :file:`color-schemes` subdirectory in the portable application directory.

.. TODO: Unsure on the OSX path, can't really check.

All color scheme files must have the extension ``.json``. The file name without the extension
will be displayed in the color scheme menu (:menuselection:`Settings --> Display --> Color Scheme`).
The contents of the color scheme directory are retrieved when SpeedCrunch is launched, so to have it pick up
a newly-added color scheme file, it is necessary to restart the application. However, color scheme
files are always reloaded when a color scheme is selected in the menu; that means after making
changes to a color scheme file, re-selecting it in SpeedCrunch will reload the color scheme and show
the changes. This allows for quick feedback when editing color schemes.

The files themselves are simple `JSON <json_>`_ files containing a single JSON object mapping
role names to color values. By way of example, this is the color scheme definition for
the "Terminal" scheme that ships with SpeedCrunch:

.. code-block:: json

    {
        "cursor": "#8C648C",
        "number": "#FFFFFF",
        "parens": "#AD7FA8",
        "result": "#689FCF",
        "comment": "#894B7A",
        "matched": "#64507B",
        "function": "#EF2928",
        "operator": "#C4A000",
        "variable": "#4A9A07",
        "scrollbar": "#8C648C",
        "separator": "#AD7FA8",
        "background": "#300A24",
        "editorbackground": "#26001A"
    }

Here, the keys are *roles*, each referring to elements in the SpeedCrunch history view
and expression editor that can be colored. The values specify the colors for these
roles. Color values may be any valid argument to the method |setNamedColor|_; the
recommended form are simple HTML-style ``#RRGGBB`` definitions, using two
hexadecimal digits per color component as in the example.

.. |setNamedColor| replace:: ``QColor::setNamedColor``
.. _json: http://json.org
.. _setNamedColor: http://doc.qt.io/qt-5/qcolor.html#setNamedColor

Any unknown roles will be ignored. If a role is not defined in the color scheme, corresponding
elements will be using the application's default text color (or, for background roles, the default
background color). However, if the file is not valid JSON or otherwise does not follow the format
described above, the color scheme will not load and SpeedCrunch will use the fallback colors
for every element.


Supported Color Roles
---------------------

The following role names are supported by SpeedCrunch:

.. describe:: cursor

   The cursor in the expression editor.

.. describe:: number

   Any number.

.. describe:: parens

   Any parentheses. This role defines the text color, both for regular as well as for
   highlighted matching parentheses.

.. describe:: result

   The result value in the history view.

.. describe:: comment

   Any comments.

.. describe:: matched

   The background color for matching parentheses. When moving the cursor on a parenthesis,
   both it and its corresponding counterpart will be highlighted by changing their background
   to this color.

.. describe:: function

   The names of defined functions, built-in as well as user-defined functions.

.. describe:: operator

   Any operator.

.. describe:: variable

   The names of any variables or built-in constants.

.. describe:: scrollbar

   The color of the history view's scrollbar.

.. describe:: separator

   Separators; notably, the semicolon (``;``) used for defining and using multi-argument functions.

.. describe:: background

   The background color of the main history view.

.. describe:: editorbackground

   The background color of the expression editor.
