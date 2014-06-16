################################################################################
#
# nmap
#
################################################################################

NMAP_VERSION = 6.40
NMAP_SITE = http://nmap.org/dist
NMAP_SOURCE = nmap-$(NMAP_VERSION).tar.bz2
NMAP_DEPENDENCIES = libpcap pcre
NMAP_CONF_OPT = --without-liblua --without-zenmap \
	--with-libdnet=included --with-liblinear=included \
	--with-libpcre="$(STAGING_DIR)/usr" --without-ncat
NMAP_LICENSE = GPLv2
NMAP_LICENSE_FILES = COPYING

ifeq ($(BR2_PACKAGE_OPENSSL),y)
NMAP_CONF_OPT += --with-openssl="$(STAGING_DIR)/usr"
NMAP_DEPENDENCIES += openssl
else
NMAP_CONF_OPT += --without-openssl
endif

# ndiff only works with python2.x
ifeq ($(BR2_PACKAGE_PYTHON),y)
NMAP_DEPENDENCIES += python
else
NMAP_CONF_OPT += --without-ndiff
endif

$(eval $(autotools-package))
