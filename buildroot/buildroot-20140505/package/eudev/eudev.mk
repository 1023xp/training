################################################################################
#
# eudev
#
################################################################################

EUDEV_VERSION = 1.5.3
EUDEV_SITE = $(call github,gentoo,eudev,$(EUDEV_VERSION))
EUDEV_LICENSE = GPLv2+ (programs), LGPLv2.1+ (libraries)
EUDEV_LICENSE_FILES = COPYING
EUDEV_INSTALL_STAGING = YES
EUDEV_AUTORECONF = YES

# mq_getattr is in librt
EUDEV_CONF_ENV += LIBS=-lrt

EUDEV_CONF_OPT =		\
	--disable-manpages	\
	--sbindir=/sbin		\
	--with-rootlibdir=/lib	\
	--libexecdir=/lib	\
	--with-firmware-path=/lib/firmware	\
	--disable-introspection			\
	--enable-split-usr			\
	--enable-libkmod

EUDEV_DEPENDENCIES = host-gperf host-pkgconf util-linux kmod

ifeq ($(BR2_PACKAGE_EUDEV_RULES_GEN),y)
EUDEV_CONF_OPT += --enable-rule_generator
endif

ifeq ($(BR2_PACKAGE_LIBGLIB2),y)
EUDEV_CONF_OPT += --enable-gudev
EUDEV_DEPENDENCIES += libglib2
else
EUDEV_CONF_OPT += --disable-gudev
endif

define EUDEV_INSTALL_INIT_SYSV
	$(INSTALL) -m 0755 package/eudev/S10udev $(TARGET_DIR)/etc/init.d/S10udev
endef

$(eval $(autotools-package))
