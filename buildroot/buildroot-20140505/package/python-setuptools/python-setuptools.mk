################################################################################
#
# python-setuptools
#
################################################################################

PYTHON_SETUPTOOLS_VERSION = 2.1.2
PYTHON_SETUPTOOLS_SOURCE  = setuptools-$(PYTHON_SETUPTOOLS_VERSION).tar.gz
PYTHON_SETUPTOOLS_SITE    = http://pypi.python.org/packages/source/s/setuptools
PYTHON_SETUPTOOLS_SETUP_TYPE = setuptools

$(eval $(python-package))
$(eval $(host-python-package))
