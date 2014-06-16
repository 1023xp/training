################################################################################
#
# mesa3d
#
################################################################################

MESA3D_VERSION = 10.0.4
MESA3D_SOURCE = MesaLib-$(MESA3D_VERSION).tar.bz2
MESA3D_SITE = ftp://ftp.freedesktop.org/pub/mesa/$(MESA3D_VERSION)
MESA3D_LICENSE = MIT, SGI, Khronos
MESA3D_LICENSE_FILES = docs/license.html
MESA3D_AUTORECONF = YES

MESA3D_INSTALL_STAGING = YES

MESA3D_DEPENDENCIES = \
	expat \
	host-bison \
	host-flex \
	host-gettext \
	host-libxml2 \
	host-python \
	host-xutil_makedepend \
	libdrm

ifeq ($(BR2_PACKAGE_XSERVER_XORG_SERVER),y)
MESA3D_DEPENDENCIES += \
	xproto_xf86driproto \
	xproto_dri2proto \
	xproto_glproto \
	xlib_libX11 \
	xlib_libXext \
	xlib_libXdamage \
	xlib_libXfixes \
	libxcb
MESA3D_CONF_OPT += \
	--enable-glx \
	--enable-xa
else
MESA3D_CONF_OPT += \
	--disable-glx \
	--disable-xa
endif

ifeq ($(BR2_PREFER_STATIC_LIB),)
# fix for "configure: error: Cannot use static libraries for DRI drivers"
MESA3D_CONF_OPT += --disable-static
endif

# Drivers

#Gallium Drivers
MESA3D_GALLIUM_DRIVERS-$(BR2_PACKAGE_MESA3D_GALLIUM_DRIVER_NOUVEAU)  += nouveau
MESA3D_GALLIUM_DRIVERS-$(BR2_PACKAGE_MESA3D_GALLIUM_DRIVER_SVGA)     += svga
MESA3D_GALLIUM_DRIVERS-$(BR2_PACKAGE_MESA3D_GALLIUM_DRIVER_SWRAST)   += swrast
# DRI Drivers
MESA3D_DRI_DRIVERS-$(BR2_PACKAGE_MESA3D_DRI_DRIVER_SWRAST) += swrast
MESA3D_DRI_DRIVERS-$(BR2_PACKAGE_MESA3D_DRI_DRIVER_I965)   += i965
MESA3D_DRI_DRIVERS-$(BR2_PACKAGE_MESA3D_DRI_DRIVER_RADEON) += radeon

ifeq ($(MESA3D_GALLIUM_DRIVERS-y),)
MESA3D_CONF_OPT += \
	--without-gallium-drivers
else
MESA3D_CONF_OPT += \
	--with-gallium-drivers=$(subst $(space),$(comma),$(MESA3D_GALLIUM_DRIVERS-y))
endif

ifeq ($(MESA3D_DRI_DRIVERS-y),)
MESA3D_CONF_OPT += \
	--disable-dri \
	--disable-shared-glapi \
	--without-dri-drivers
else
MESA3D_CONF_OPT += \
	--enable-dri \
	--enable-shared-glapi \
	--with-dri-drivers=$(subst $(space),$(comma),$(MESA3D_DRI_DRIVERS-y))
endif

# APIs

# Always enable OpenGL:
#   - it is needed for GLES (mesa3d's ./configure is a bit weird)
#   - but if no DRI driver is enabled, then libgl is not built
MESA3D_CONF_OPT += --enable-opengl

ifeq ($(BR2_PACKAGE_MESA3D_OPENGL_EGL),y)
# egl depends on gbm, gbm depends on udev
MESA3D_DEPENDENCIES += udev
MESA3D_EGL_PLATFORMS = drm
ifeq ($(BR2_PACKAGE_WAYLAND),y)
MESA3D_DEPENDENCIES += wayland
MESA3D_EGL_PLATFORMS += wayland
endif
ifeq ($(BR2_PACKAGE_XSERVER_XORG_SERVER),y)
MESA3D_EGL_PLATFORMS += x11
endif
MESA3D_CONF_OPT += \
	--enable-gbm \
	--enable-egl \
	--with-egl-platforms=$(subst $(space),$(comma),$(MESA3D_EGL_PLATFORMS))
else
MESA3D_CONF_OPT += \
	--disable-gbm \
	--disable-egl
endif

ifeq ($(BR2_PACKAGE_MESA3D_OPENGL_ES),y)
MESA3D_CONF_OPT += --enable-gles1 --enable-gles2
else
MESA3D_CONF_OPT += --disable-gles1 --disable-gles2
endif

$(eval $(autotools-package))
