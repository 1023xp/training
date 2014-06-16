################################################################################
#
# libstrophe
#
################################################################################

LIBSTROPHE_VERSION = d408eaf2bbfe5ff5c56eab01463c278f9891c08e
LIBSTROPHE_SITE = $(call github,strophe,libstrophe,$(LIBSTROPHE_VERSION))
LIBSTROPHE_DEPENDENCIES = openssl host-pkgconf
LIBSTROPHE_AUTORECONF = YES
LIBSTROPHE_LICENSE = MIT or GPLv3
LIBSTROPHE_LICENSE_FILES = MIT-LICENSE.txt GPL-LICENSE.txt

# This package does not build a dynamic library, so there is nothing
# to be installed to the target.
LIBSTROPHE_INSTALL_STAGING = YES
LIBSTROPHE_INSTALL_TARGET = NO

ifeq ($(BR2_PACKAGE_EXPAT),y)
# Passing --without-libxml2 doesn't work, due to how AC_ARG_WITH is
# used in configure.ac. As long as --with-libxml2 is *not* passed, the
# configure script assumes expat should be used.
LIBSTROPHE_DEPENDENCIES += expat
else
LIBSTROPHE_CONF_OPT += --with-libxml2
LIBSTROPHE_DEPENDENCIES += libxml2
endif

$(eval $(autotools-package))
