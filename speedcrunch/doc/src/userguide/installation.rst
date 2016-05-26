.. highlight:: shell

Installation
============

Microsoft Windows
-----------------

SpeedCrunch for Windows requires Windows Vista or newer (32-bit and 64-bit versions).
You can use choose to use either the installer or the portable
version of SpeedCrunch. Both of these are available from `the SpeedCrunch website <sc_>`_.

.. _sc: http://speedcrunch.org

The installer installs SpeedCrunch on your system and creates an entry in the menu
to launch it. This process requires administrator rights.

Alternatively, you may use the portable version of SpeedCrunch, which is distributed
as a ``.zip`` archive. This version requires no installation, simply extract it to
a folder of your choice and run :program:`speedcrunch.exe`. In addition, this version of
the software is fully portable: It stores all its settings and user data next to the
executable so the entire directory can be copied to another computer and run
there.


.. Apple OS X
.. ----------

.. TODO: OSX installation instructions


Linux
-----

On Linux, there are two main ways to install SpeedCrunch: Either install the package
provided by your distribution, if any, or install the binary packages from
the SpeedCrunch website.


Distribution Packages
+++++++++++++++++++++

Many Linux distributions already include packages for SpeedCrunch in their repositories. The
exact steps to install these depend on the distribution. For example, on Debian and
Ubuntu, the included SpeedCrunch package can be installed by running this command::

   sudo apt-get install speedcrunch

On Fedora, this command will install SpeedCrunch::

   sudo yum install speedcrunch

Check your system's documentation to find out how to install additional packages.
Many desktop distributions also include graphical package management software that
can be used to install SpeedCrunch.


Generic Packages
++++++++++++++++

.. versionadded:: 0.12

   The generic Linux packages are available starting with SpeedCrunch 0.12.

As an alternative to distribution packages, Linux binary packages of SpeedCrunch are
available on `the SpeedCrunch website <sc_>`_. They are generic packages that are built
to be compatible with many different Linux systems. As a rule of thumb, they are likely
to work on most Linux distributions released since around 2010; however, due to the
wide variety and flexibility of Linux systems, this is only a best-effort endeavor and
never guaranteed. Additionally, due to the generic nature of the packages, they may not
integrate as well with the system as your distribution's packages; in particular, they
won't adapt as well to the visual style of your desktop environment. In general, if there's
a SpeedCrunch package available from your distribution, that will likely work better for
your system than these generic packages, but it may be an older version.

.. _sc: http://speedcrunch.org

There's choice of three different types of packages per architecture (32-bit or 64-bit); which
one is right for you depends on your system:

* Use the ``.deb`` packages for Debian and derived distributions, like Ubuntu.
* Use the ``.rpm`` packages for RPM-based systems like Fedora, Red Hat Enterprise Linux/CentOS, openSUSE or Mageia.

The exact steps for installing the package again depend on your system. Most desktop Linux distributions
include a graphical utility to install package files from the file manager.

If neither of these fit your system, there are also ``.tar.bz2`` archives that simply contain
the binary. The contents can be copied to any directory and the :program:`speedcrunch` binary
can be run from there.


Development Builds
------------------

.. warning::

   Development builds of SpeedCrunch may not be as stable or well-tested as released versions
   and may contain changes that break your session history. Use them at your own risk.

Pre-built development versions of SpeedCrunch allow you to use newer features
and bug-fixes before they become available in a release, without having to compile SpeedCrunch
yourself. Currently, automatically updated
development builds of SpeedCrunch are available for the following platforms:

* `Windows <dev-win_>`_
* `Ubuntu <dev-ubuntu_>`_ (14.04 and later)

.. _dev-win: https://github.com/Tey/speedcrunch-nightlies/releases
.. _dev-ubuntu: https://code.launchpad.net/~fkrull/+archive/ubuntu/speedcrunch-daily



Building from Source
--------------------

The SpeedCrunch source code is maintained on `Bitbucket`_ in a Git repository. Clone
the repository onto your machine by running the following command::

    git clone https://bitbucket.org/heldercorreia/speedcrunch.git

.. _Bitbucket: https://bitbucket.org/heldercorreia/speedcrunch

The main readme file (:file:`README.md` at the root of the repository) contains
instructions on how to compile SpeedCrunch.
