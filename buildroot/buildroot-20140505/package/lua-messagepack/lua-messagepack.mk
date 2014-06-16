################################################################################
#
# lua-messagepack
#
################################################################################

LUA_MESSAGEPACK_VERSION_UPSTREAM = 0.3.0
LUA_MESSAGEPACK_VERSION = $(LUA_MESSAGEPACK_VERSION_UPSTREAM)-1
LUA_MESSAGEPACK_SUBDIR  = lua-MessagePack-$(LUA_MESSAGEPACK_VERSION_UPSTREAM)
LUA_MESSAGEPACK_LICENSE = MIT
LUA_MESSAGEPACK_LICENSE_FILES = $(LUA_MESSAGEPACK_SUBDIR)/COPYRIGHT

$(eval $(luarocks-package))
