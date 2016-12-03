# -*- coding: utf-8 -*-
# This file is part of the SpeedCrunch project
# Copyright (c) 2016 Felix Krull <f_krull@gmx.de>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.

"""
Helper extension to provide translation strings for our custom extensions. This
extension piggy-backs onto Sphinx's i18n system to load the 'extra-doc-strings'
catalog and hand out translations from it. To use, enable the extension (this
causes the translations to be loaded); in other extensions, import this module
and use the `_` and `l_` functions.

The translations are loaded during the 'env-before-read-docs' event;
module-level strings should only ever use `l_` (to lazy-load the translation).
"""

import os

from sphinx import locale

# translation stuff
_CATALOG = 'extra-doc-strings'


def _(message):
    return locale.translators[_CATALOG].gettext(message)


def l_(string):
    """Lazy gettext, puts off resolving the string for as long as possible."""
    return locale._TranslationProxy(_, string)


def load_translations(app):
    env = app.env
    # (try to) load string translations
    locale_dirs = [os.path.join(env.srcdir, x)
                   for x in env.config.locale_dirs]
    print("loading '%s' translations [%s]..." % (_CATALOG, env.config.language))
    locale.init(locale_dirs, env.config.language, _CATALOG)


def setup(app):
    app.connect('builder-inited', load_translations)
    return {'version': '0.1'}
