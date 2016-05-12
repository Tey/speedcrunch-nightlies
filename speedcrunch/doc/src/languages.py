#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function, unicode_literals

SOURCE_LANGUAGE = 'en_US'
LANGUAGES = {
    'en_US': 'English',
    'de_DE': 'Deutsch',
    'es_ES': 'Español',
    'fr_FR': 'Français'
}

LANGUAGE_CODES = list(LANGUAGES.keys())

TRANSLATIONS = LANGUAGE_CODES[:]
TRANSLATIONS.remove(SOURCE_LANGUAGE)
