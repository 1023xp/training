export MAKEROOT := $(shell pwd)

TARGET_DIRS := binary \
               variant

define build_obj
for SubDir in $(TARGET_DIRS); do \
	if ! [ -d $$SubDir ]; then \
		echo "The $$SubDir is not exist !"; \
		exit 11; \
	fi; \
	echo "Building $$SubDir ..."; \
	make -C $$SubDir ; \
	if [ $$? -ne 0 ]; then \
		echo "Building $$SubDir is failed !"; \
		exit 12; \
	fi; \
done
endef

all :
	@$(call build_obj) #调用过程
