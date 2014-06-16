
# There's probably a better way of doing this (apart from automake),
# would anyone care to enlighten me?

# Note this is the build order, and reflects inter-subdir
# dependencies.
SUBDIRS=kernel jutil src tools

.PHONY: build clean test $(SUBDIRS)

DOSUBDIRS=for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done

build: $(SUBDIRS)
	$(DOSUBDIRS)

clean: $(SUBDIRS)
	$(DOSUBDIRS)

test: build
	make -C src test
