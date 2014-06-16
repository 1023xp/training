################################################################################
#
# postgresql
#
################################################################################

POSTGRESQL_VERSION = 9.3.4
POSTGRESQL_SOURCE = postgresql-$(POSTGRESQL_VERSION).tar.bz2
POSTGRESQL_SITE = http://ftp.postgresql.org/pub/source/v$(POSTGRESQL_VERSION)
POSTGRESQL_LICENSE = PostgreSQL
POSTGRESQL_LICENSE_FILES = COPYRIGHT

ifneq ($(BR2_TOOLCHAIN_HAS_THREADS),y)
	POSTGRESQL_CONF_OPT += --disable-thread-safety
endif

ifeq ($(BR2_PACKAGE_READLINE),y)
	POSTGRESQL_DEPENDENCIES += readline
else
	POSTGRESQL_CONF_OPT += --without-readline
endif

ifeq ($(BR2_PACKAGE_ZLIB),y)
	POSTGRESQL_DEPENDENCIES += zlib
else
	POSTGRESQL_CONF_OPT += --without-zlib
endif

ifeq ($(BR2_PACKAGE_TZDATA),y)
	POSTGRESQL_DEPENDENCIES += tzdata
	POSTGRESQL_CONF_OPT += --with-system-tzdata=/usr/share/zoneinfo
endif

ifeq ($(BR2_PACKAGE_OPENSSL),y)
	POSTGRESQL_DEPENDENCIES += openssl
	POSTGRESQL_CONF_OPT += --with-openssl
endif

define POSTGRESQL_USERS
	postgres -1 postgres -1 * /var/lib/pgsql /bin/sh - PostgreSQL Server
endef

define POSTGRESQL_INSTALL_TARGET_FIXUP
	$(INSTALL) -dm 0700 $(TARGET_DIR)/var/lib/pgsql
endef

POSTGRESQL_POST_INSTALL_TARGET_HOOKS += POSTGRESQL_INSTALL_TARGET_FIXUP

define POSTGRESQL_INSTALL_INIT_SYSV
	$(INSTALL) -m 0755 -D package/postgresql/S50postgresql \
		$(TARGET_DIR)/etc/init.d/S50postgresql
endef

$(eval $(autotools-package))
