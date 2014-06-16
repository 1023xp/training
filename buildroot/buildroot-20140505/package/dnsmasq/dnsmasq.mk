################################################################################
#
# dnsmasq
#
################################################################################

DNSMASQ_VERSION = 2.70
DNSMASQ_SOURCE = dnsmasq-$(DNSMASQ_VERSION).tar.xz
DNSMASQ_SITE = http://thekelleys.org.uk/dnsmasq
DNSMASQ_MAKE_ENV = $(TARGET_MAKE_ENV) CC="$(TARGET_CC)"
DNSMASQ_MAKE_OPT = COPTS="$(DNSMASQ_COPTS)" PREFIX=/usr CFLAGS="$(TARGET_CFLAGS)"
DNSMASQ_MAKE_OPT += DESTDIR=$(TARGET_DIR) LDFLAGS="$(TARGET_LDFLAGS)"
DNSMASQ_DEPENDENCIES = host-pkgconf
DNSMASQ_LICENSE = Dual GPLv2/GPLv3
DNSMASQ_LICENSE_FILES = COPYING COPYING-v3

ifneq ($(BR2_INET_IPV6),y)
	DNSMASQ_COPTS += -DNO_IPV6
endif

ifneq ($(BR2_PACKAGE_DNSMASQ_DHCP),y)
	DNSMASQ_COPTS += -DNO_DHCP
endif

ifeq ($(BR2_PACKAGE_DNSMASQ_DNSSEC),y)
	DNSMASQ_DEPENDENCIES += gmp nettle
	DNSMASQ_COPTS += -DHAVE_DNSSEC
ifeq ($(BR2_PREFER_STATIC_LIB),y)
	DNSMASQ_COPTS += -DHAVE_DNSSEC_STATIC
endif
endif

ifneq ($(BR2_PACKAGE_DNSMASQ_TFTP),y)
	DNSMASQ_COPTS += -DNO_TFTP
endif

# NLS requires IDN so only enable it (i18n) when IDN is true
ifeq ($(BR2_PACKAGE_DNSMASQ_IDN),y)
	DNSMASQ_DEPENDENCIES += libidn $(if $(BR2_NEEDS_GETTEXT_IF_LOCALE),gettext) host-gettext
	DNSMASQ_MAKE_OPT += LDFLAGS+="-lidn $(if $(BR2_NEEDS_GETTEXT_IF_LOCALE),-lintl)"
	DNSMASQ_COPTS += -DHAVE_IDN
	DNSMASQ_I18N = $(if $(BR2_ENABLE_LOCALE),-i18n)
endif

ifeq ($(BR2_PACKAGE_DNSMASQ_CONNTRACK),y)
	DNSMASQ_DEPENDENCIES += libnetfilter_conntrack
endif

ifeq ($(BR2_PACKAGE_DNSMASQ_CONNTRACK),y)
define DNSMASQ_ENABLE_CONNTRACK
	$(SED) 's^.*#define HAVE_CONNTRACK.*^#define HAVE_CONNTRACK^' \
		$(DNSMASQ_DIR)/src/config.h
endef
endif

ifeq ($(BR2_PACKAGE_DNSMASQ_LUA),y)
	DNSMASQ_DEPENDENCIES += lua
	DNSMASQ_MAKE_OPT += LDFLAGS+="-ldl"

define DNSMASQ_ENABLE_LUA
	$(SED) 's/lua5.1/lua/g' $(DNSMASQ_DIR)/Makefile
	$(SED) 's^.*#define HAVE_LUASCRIPT.*^#define HAVE_LUASCRIPT^' \
		$(DNSMASQ_DIR)/src/config.h
endef
endif

ifneq ($(BR2_LARGEFILE),y)
	DNSMASQ_COPTS += -DNO_LARGEFILE
endif

ifeq ($(BR2_PACKAGE_DBUS),y)
	DNSMASQ_DEPENDENCIES += dbus
endif

define DNSMASQ_FIX_PKGCONFIG
	$(SED) 's^PKG_CONFIG = pkg-config^PKG_CONFIG = $(PKG_CONFIG_HOST_BINARY)^' \
		$(DNSMASQ_DIR)/Makefile
endef

ifeq ($(BR2_PACKAGE_DBUS),y)
define DNSMASQ_ENABLE_DBUS
	$(SED) 's^.*#define HAVE_DBUS.*^#define HAVE_DBUS^' \
		$(DNSMASQ_DIR)/src/config.h
endef
else
define DNSMASQ_ENABLE_DBUS
	$(SED) 's^.*#define HAVE_DBUS.*^/* #define HAVE_DBUS */^' \
		$(DNSMASQ_DIR)/src/config.h
endef
endif

define DNSMASQ_BUILD_CMDS
	$(DNSMASQ_FIX_PKGCONFIG)
	$(DNSMASQ_ENABLE_DBUS)
	$(DNSMASQ_ENABLE_LUA)
	$(DNSMASQ_ENABLE_CONNTRACK)
	$(DNSMASQ_MAKE_ENV) $(MAKE1) -C $(@D) $(DNSMASQ_MAKE_OPT) all$(DNSMASQ_I18N)
endef

define DNSMASQ_INSTALL_TARGET_CMDS
	$(DNSMASQ_MAKE_ENV) $(MAKE) -C $(@D) $(DNSMASQ_MAKE_OPT) install$(DNSMASQ_I18N)
	mkdir -p $(TARGET_DIR)/var/lib/misc/
endef

define DNSMASQ_INSTALL_INIT_SYSV
	$(INSTALL) -m 755 -D package/dnsmasq/S80dnsmasq \
		$(TARGET_DIR)/etc/init.d/S80dnsmasq
endef

$(eval $(generic-package))
