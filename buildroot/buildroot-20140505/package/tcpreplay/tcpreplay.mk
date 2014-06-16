################################################################################
#
# tcpreplay
#
################################################################################

TCPREPLAY_VERSION = 4.0.3
TCPREPLAY_SITE = http://downloads.sourceforge.net/project/tcpreplay/tcpreplay/$(TCPREPLAY_VERSION)
TCPREPLAY_LICENSE = GPLv3
TCPREPLAY_LICENSE_FILES = docs/LICENSE
TCPREPLAY_CONF_ENV = \
	tr_cv_libpcap_version=">= 0.7.0" \
	ac_cv_have_bpf=no
TCPREPLAY_CONF_OPT = --with-libpcap=$(STAGING_DIR)/usr
TCPREPLAY_DEPENDENCIES = libpcap

# libpcap may depend on symbols in libusb as well
TCPREPLAY_LIBS = -lpcap $(if $(BR2_PACKAGE_LIBUSB),-lusb-1.0)
TCPREPLAY_CONF_ENV += ac_cv_search_pcap_close='$(TCPREPLAY_LIBS)'

ifeq ($(BR2_PACKAGE_TCPDUMP),y)
TCPREPLAY_CONF_ENV += ac_cv_path_tcpdump_path=/usr/sbin/tcpdump
else
TCPREPLAY_CONF_ENV += ac_cv_path_tcpdump_path=no
endif

$(eval $(autotools-package))
