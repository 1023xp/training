################################################################################
#
# znc
#
################################################################################

ZNC_VERSION = b396cafdb249544164ed02942a5babba59e519a3
ZNC_SITE = git://github.com/znc/znc.git
ZNC_LICENSE = Apache-2.0
ZNC_LICENSE_FILES = LICENSE
ZNC_DEPENDENCIES = host-pkgconf host-autoconf host-automake
ZNC_CONF_OPT = --disable-perl

# The standard <pkg>_AUTORECONF = YES invocation doesn't work for this
# package, because it does not use automake in a normal way.
define ZNC_RUN_AUTOGEN
	cd $(@D) && PATH=$(BR_PATH) ./autogen.sh
endef
ZNC_PRE_CONFIGURE_HOOKS += ZNC_RUN_AUTOGEN

ifeq ($(BR2_PACKAGE_ICU),y)
ZNC_DEPENDENCIES += icu
ZNC_CONF_OPT += --enable-icu
else
ZNC_CONF_OPT += --disable-icu
endif

ifeq ($(BR2_PACKAGE_OPENSSL),y)
ZNC_DEPENDENCIES += openssl
ZNC_CONF_OPT += --enable-openssl
else
ZNC_CONF_OPT += --disable-openssl
endif

ifeq ($(BR2_PACKAGE_ZLIB),y)
ZNC_DEPENDENCIES += zlib
ZNC_CONF_OPT += --enable-zlib
else
ZNC_CONF_OPT += --disable-zlib
endif

ifeq ($(BR2_PACKAGE_PYTHON3),y)
ZNC_DEPENDENCIES += python3 host-swig
ZNC_CONF_OPT += --enable-python=python3
else
ZNC_CONF_OPT += --disable-python
endif

$(eval $(autotools-package))
