################################################################################
#
# jq
#
################################################################################

JQ_VERSION = 1.3
JQ_SITE = http://stedolan.github.io/jq/download/source
JQ_DEPENDENCIES = host-flex host-bison
JQ_LICENSE = MIT (code), CC-BY-3.0 (documentation)
JQ_LICENSE_FILES = COPYING

# uses c99 specific features
JQ_CONF_ENV += CFLAGS="$(TARGET_CFLAGS) -std=c99"

$(eval $(autotools-package))
