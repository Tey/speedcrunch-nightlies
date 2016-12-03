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


"""
Add a directive that allows to specify extra keywords for a QtHelp project. To
use, enable the extension and use the qtkeyword directive with the included
qthelp2 builder.
"""


from docutils.parsers.rst import Directive
from sphinx.builders.qthelp import QtHelpBuilder
from docutils import nodes


def add_keyword(env, docname, lineno, target, designation, ref_uri):
    if not hasattr(env, 'all_qtkeywords'):
        env.all_qtkeywords = []

    env.all_qtkeywords.append({
        'docname': docname,
        'lineno': lineno,
        'target': target,
        'designation': designation,
        'refuri': ref_uri
    })


def add_id_keyword(env, id, docname, anchor):
    if not env.config.ignore_qtkeywords:
        add_keyword(env, docname, 0, None, 'id="%s"' % id,
                    env.app.builder.get_target_uri(docname) + '#' + anchor)


class QtKeywordDirective(Directive):
    """
    Directive to pass a keyword to a QtHelp project.

    Use:
        .. qtkeyword:: ARGUMENTS
    When building with the custom qthelp2 builder, ARGUMENTS will be copied
    verbatim into a corresponding keyword entry:
        <keyword ARGUMENTS ref=somewhere/>
    """

    has_content = False
    required_arguments = 1
    optional_arguments = 10

    def run(self):
        self.arguments = " ".join(self.arguments)
        env = self.state.document.settings.env
        if env.config.ignore_qtkeywords:
            return []

        # create a (resolvable) target  we can link to
        targetid = "kw-%d" % env.new_serialno('kw')
        targetnode = nodes.target('', '', ids=[targetid])

        add_keyword(
            env, env.docname, self.lineno, targetnode, self.arguments,
            env.app.builder.get_target_uri(env.docname) + '#' + targetid)
        return [targetnode]


class MyQtHelpBuilder(QtHelpBuilder):
    """
    Custom version of the qthelp builder.

    This one automatically generates useful id entries from the index, e.g.
    'cos() (built-in function)' will be mapped to 'cos'.

    Also, it allows the use of the 'qtkeyword' directive.
    """

    name = "qthelp2"
    search = False

    def init(self):
        self.keywords_completed = False
        QtHelpBuilder.init(self)

    def keyword_item(self, name, ref):
        # don't do any id guessing; for regular index entries, we don't generate
        # any id attributes
        item = ' ' * 12 + '<keyword name="%s" ref="%s"/>' % (name, ref[1])
        # error checking?
        item.encode('ascii', 'xmlcharrefreplace')
        return item

    def build_keywords(self, title, refs, subitems):
        # call parent method for constructing the keywords from index
        keywords = QtHelpBuilder.build_keywords(self, title, refs, subitems)

        # now we add our keywords
        if (
                self.keywords_completed or
                not hasattr(self.app.env, 'all_qtkeywords') or
                self.env.config.ignore_qtkeywords):
            return keywords
        for kw in self.env.all_qtkeywords:
            keywords.append(' ' * 12 + '<keyword {} ref="{}"/>'
                            .format(kw["designation"], kw['refuri']))
        self.keywords_completed = True
        return keywords


def purge_keywords(app, env, docname):
    """Purge outdated keywords.

    If the document has become out of date, clear all keywords that are
    associated with it. They will be regenerated when the document is parsed.
    This is necessary to make sure that any keywords that are removed from the
    source don't stay in the persistent environment.
    """
    if not hasattr(env, 'all_qtkeywords'):
        return

    env.all_qtkeywords = [kw for kw in env.all_qtkeywords
                          if kw['docname'] != docname]


def process_keywords(app, env, docname):
    if not hasattr(app.env, 'all_qtkeywords'):
        return
    for kw in app.env.all_qtkeywords:
        kw['refuri'] = app.builder.get_target_uri(docname)
        kw['refuri'] += '#' + kw['target']['refid']


def setup(app):
    app.add_config_value('ignore_qtkeywords', True, 'html')
    app.add_builder(MyQtHelpBuilder)
    app.add_directive('qtkeyword', QtKeywordDirective)
    app.connect('env-purge-doc', purge_keywords)
    # app.connect('doctree-resolved', process_keywords)

    return {'version': '0.1'}   # identifies the version of our extension
