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
Extension to handle SpeedCrunch-specific config settings.

Mostly, it sets various things based on the `language` setting. The docs
language is specified on the command-line and not available in `conf.py`.
"""

import re

_LANG_RE = re.compile(r'([^._@]+)')


def _parse_lang_code(lang_code):
    """Extract the language part from a full language code.

    `lang_code` may be of this form:
    language[_script][_country][.codeset][@modifier]
    """
    if not lang_code:
        return None
    m = _LANG_RE.match(lang_code)
    if m is None:
        return None
    return m.group(1)


def _post_config(app, env, *args, **kwargs):
    """Fixup config values based on the selected doc language."""
    lang = _parse_lang_code(env.config.language)
    if env.config.language and lang:
        if not env.config.qthelp_basename:
            env.config.qthelp_basename = 'SpeedCrunch-%s' % env.config.language
        if not env.config.html_search_language:
            env.config.html_search_language = lang


def setup(app):
    # this event seems fine for our fixup
    app.connect('env-before-read-docs', _post_config)
