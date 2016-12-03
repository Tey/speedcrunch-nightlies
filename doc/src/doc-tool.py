#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# This file is part of the SpeedCrunch project
# Copyright (c) 2016 Pol Welter <polwelter@gmail.com
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

# This file should work with Python 2.7 and 3.4+

from __future__ import print_function, unicode_literals

import argparse
from io import open
import os
import subprocess
import sys

from languages import TRANSLATIONS, LANGUAGE_CODES as LANGUAGES

DOC_DIR = os.path.dirname(os.path.abspath(__file__))


class Tools:
    TOOLS = ['sphinx-build', 'sphinx-intl', 'qcollectiongenerator']

    def __init__(self, args):
        self._tools = {}
        for tool in self.TOOLS:
            safename = tool.replace('-', '_')
            self._tools[safename] = getattr(args, '%s_binary' % safename)

    def run_tool(self, toolname, *args):
        cmd = [self._tools[toolname]]
        cmd.extend(args)
        return subprocess.check_call(cmd)

    def __getattr__(self, name):
        return lambda *args: self.run_tool(name, *args)

    # Let the user specify the path to the tool's binary via CLI argument
    @classmethod
    def add_arguments(cls, parser):
        for tool in cls.TOOLS:
            parser.add_argument('--%s-binary' % tool, default=tool,
                                help='%s command' % tool)


def build_docs(tools, source_dir, build_dir, lang=None, builder='html',
               tags=[], extra_config={}):
    config = extra_config.copy()
    if lang:
        print('Building docs for %s...' % lang)
        outdir = os.path.join(build_dir, lang)
        config['language'] = lang
    else:
        outdir = build_dir
    args = [source_dir, outdir, '-b', builder]
    for k, v in config.items():
        args.extend(['-D', '%s=%s' % (k, v)])
    if tags:
        for tag in tags:
            args.extend(['-t', tag])
    return tools.sphinx_build(*args)


def generate_qrc(f, files, prefix='/'):
    f.write('<RCC>\n')
    f.write('  <qresource prefix="%s">\n' % prefix)
    for alias, path in files:
        f.write('    <file alias="%s">%s</file>\n' % (alias, path))
    f.write('  </qresource>\n')
    f.write('</RCC>\n')


def extract_python_strings(dirname, outfile, domain=None):
    """Extract translatable strings from all Python files in `dir`.

    Writes a PO template to `outfile`. Recognises `_` and `l_`. Needs babel!
    """
    from babel.messages.catalog import Catalog
    from babel.messages.extract import extract_from_dir
    from babel.messages.pofile import write_po
    base_dir = os.path.abspath(os.path.dirname(outfile))
    msgs = extract_from_dir(dirname, keywords={'_': None, 'l_': None},
                            comment_tags=['l10n:'])
    cat = Catalog(domain=domain, charset='utf-8')
    for fname, lineno, message, comments, context in msgs:
        filepath = os.path.join(dirname, fname)
        cat.add(message, None, [(os.path.relpath(filepath, base_dir), lineno)],
                auto_comments=comments, context=context)
    with open(outfile, 'wb') as f:
        write_po(f, cat)


# commands
def extract_strings(tools, args):
    build_docs(tools, args.source_dir, args.build_dir, builder='gettext')
    extract_python_strings(
        os.path.join(args.source_dir, 'extensions'),
        os.path.join(args.build_dir, 'extra-doc-strings.pot'),
        'extra-doc-strings')


def update_translations(tools, args):
    args = ['--config', os.path.join(args.source_dir, 'conf.py'),
            'update', '--pot-dir', args.pot_dir]
    for lang in TRANSLATIONS:
        args.extend(['--language', lang])
    tools.sphinx_intl(*args)


def build_standalone_docs(tools, args):
    for lang in LANGUAGES:
        build_docs(tools, args.source_dir, args.build_dir, lang)


def build_bundled_docs(tools, args):
    resources = []
    for lang in LANGUAGES:
        basename = 'manual-%s' % lang
        build_docs(tools, args.source_dir, args.build_dir, lang,
                   builder='qthelp2', tags=['sc_bundled_docs'],
                   extra_config={'qthelp_basename': basename})
        tools.qcollectiongenerator(os.path.join(args.build_dir, lang,
                                                '%s.qhcp' % basename))
        resources.append(('%s.qch' % basename, '%s/%s.qch' % (lang, basename)))
        resources.append(('%s.qhc' % basename, '%s/%s.qhc' % (lang, basename)))
    with open(os.path.join(args.build_dir, 'manual.qrc'), 'w',
              encoding='utf-8') as f:
        print("Creating resource file...")
        generate_qrc(f, resources, prefix='/manual')


# CLI
def build_argument_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--source-dir', '-s', default=DOC_DIR,
        help='Documentation source directory (default: %(default)s)')
    Tools.add_arguments(parser)
    subparsers = parser.add_subparsers()

    # extract-strings
    parser_extract_strings = subparsers.add_parser(
        'extract-strings',
        help='Extract translatable strings into POT files')
    parser_extract_strings.add_argument(
        '--build-dir', '-b', default='_build-pot',
        help='Output directory (default: %(default)s')
    parser_extract_strings.set_defaults(func=extract_strings)

    # update-translations
    parser_update_translations = subparsers.add_parser(
        'update-translations',
        help='Update the PO files for all languages from templates')
    parser_update_translations.add_argument(
        '--pot-dir', '-p', default='_build-pot',
        help='POT files directory, generated by extract-files '
             '(default: %(default)s')
    parser_update_translations.set_defaults(func=update_translations)

    # build-html-docs
    parser_build_html = subparsers.add_parser(
        'build-standalone-docs',
        help='Build stand-alone HTML docs for all languages')
    parser_build_html.add_argument(
        '--build-dir', '-b', default='_build-html',
        help='Output base directory (default: %(default)s')
    parser_build_html.set_defaults(func=build_standalone_docs)

    # build-bundled-docs
    parser_build_bundled = subparsers.add_parser(
        'build-bundled-docs',
        help='Build Qt Help docs for all languages')
    parser_build_bundled.add_argument(
        '--build-dir', '-b', default='_build-bundled',
        help='Output base directory (default: %(default)s')
    parser_build_bundled.set_defaults(func=build_bundled_docs)

    return parser


def main(argv):
    parser = build_argument_parser()
    args = parser.parse_args(argv[1:])
    tools = Tools(args)
    if hasattr(args, 'func') and args.func:
        args.func(tools, args)
    else:
        parser.print_help()


if __name__ == '__main__':
    main(sys.argv)
