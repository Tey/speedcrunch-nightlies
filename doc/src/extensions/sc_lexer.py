# -*- coding: utf-8 -*-

"""
A lexer extension that reads and highlights the SpeedCrunch expression syntax.
To use, enable the extension and use the 'sc' or 'speedcrunch' syntax.
"""

from __future__ import print_function, unicode_literals
import re

from pygments.lexer import inherit, RegexLexer, words as _words
from pygments.token import *  # noqa


RADIX = r'[.,]'
SEPARATOR = '[ _\'u00b7\u066b\u066c\u02d9\u2396]'
SEPARATORS = SEPARATOR + '*'


def opt(string):
    return '(?:%s)?' % string


def either(a, b):
    return '(?:%s|%s)' % (a, b)


def casefolded(s):
    l, u = s.lower(), s.upper()
    return s if l == u else '[%s%s]' % (l, u)


def numprefix(prefix):
    return SEPARATORS.join(casefolded(s) for s in prefix) + SEPARATORS


def digits(d, required=True):
    return '([%s]%s)%s' % (d, SEPARATORS, '+' if required else '*')


class words(_words):
    def get(self):
        return super(words, self).get() + r'(?=\W+)'


def floatform(d):
    return digits(d, False) + RADIX + SEPARATORS + digits(d)


EXPONENT = '[eE]' + SEPARATORS + '[+-]?' + SEPARATORS + digits('0-9')

CONSTANTS = ['pi', '\u03c0', 'e', '\u212f', 'j', 'ans']
FUNCTIONS = []


class SpeedCrunchLexer(RegexLexer):
    name = 'SpeedCrunch'
    flags = re.MULTILINE | re.UNICODE

    tokens = {
        str('root'): [
            # comments
            (r'[?].*\n', Comment),

            # regular operators
            (r'(\*\*|<<|>>|->|[+\-*/=^!\\&|])', Operator),
            # keyword operators
            (words(['in']), Operator.Word),
            # additional punctuation
            (r'[();]', Punctuation),

            # built-in constants
            (words(CONSTANTS), Name.Constant),
            # built-in functions
            # (words(FUNCTIONS), Name.Builtin),
            # any other identifiers
            (r'([^\W\d]|\$)(\w|\$)*', Name),

            # binary numbers
            (numprefix('0b') + floatform('01'), Number.Bin),
            (numprefix('0b') + digits('01'), Number.Bin),

            # octal numbers
            (numprefix('0o') + floatform('0-7'), Number.Oct),
            (numprefix('0o') + digits('0-7'), Number.Oct),

            # hex numbers
            (either(numprefix('0x'), numprefix('#')) + floatform('0-9a-fA-F'), Number.Hex),
            (either(numprefix('0x'), numprefix('#')) + digits('0-9a-fA-F'), Number.Hex),

            # decimal numbers
            (opt(numprefix('0d')) + floatform('0-9') + opt(EXPONENT), Number.Float),
            (opt(numprefix('0d')) + digits('0-9') + opt(EXPONENT), Number.Integer),

            # any remaining whitespace
            (r'\s+', Text),
        ],
    }


class SpeedCrunchSessionLexer(SpeedCrunchLexer):
    name = 'SpeedCrunch Session'
    aliases = ['speedcrunch', 'sc']

    tokens = {
        str('root'): [
            # results; any line starting with '='
            (r'\n\s*=.*\n', Generic.Emph),

            inherit,

            # an ellipsis for use in examples
            (r'\.\.\.', Punctuation),
        ]
    }


__all__ = ['SpeedCrunchLexer', 'SpeedCrunchSessionLexer']


# Sphinx extension interface
def setup(app):
    sc_lexer = SpeedCrunchSessionLexer(stripnl=False)
    sc_lexer.add_filter('raiseonerror')
    app.add_lexer('sc', sc_lexer)
    app.add_lexer('speedcrunch', sc_lexer)
    return {
        'version': '0.1',
        'parallel_read_safe': True,
    }
