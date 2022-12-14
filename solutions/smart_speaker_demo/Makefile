CPRE := @
ifeq ($(V),1)
CPRE :=
VERB := --verbose
endif

MK_GENERATED_IMGS_PATH:=generated
BOARD_NAME:=$(shell cat package.yaml | grep board_name | sed 's/[[:space:]\"]//g' | awk -F":" '{print $$2}')
BOARD_PATH:=../../boards/$(BOARD_NAME)
PRODUCT_BIN:=product

CKBOOT_BIN:=${BOARD_PATH}/bootimgs/boot_0278.bin
M0BOOT_BIN=${BOARD_PATH}/bootimgs/m0flash_0278_dbs.bin

VOICE_PKG:=$(shell cat package.yaml | grep "\- voice_" | sed 's/[[:space:]\"]//g' | awk -F"[:-]" '{print $$2}')
DSPFW_BIN=../../components/${VOICE_PKG}/sc5654/dspfw.bin
CONFIG_YAML=../../components/${VOICE_PKG}/sc5654/configs/config.yaml
CONFIG_LDF=../../components/${VOICE_PKG}/sc5654/configs/gcc_flash.ld
LFS_BIN:=app/src/audio/lfs.bin

.PHONY:startup
startup: all

all:
	cp $(CONFIG_LDF) ./
	$(CPRE) scons $(VERB) -j4
	@echo YoC SDK Done
	$(CPRE) rm gcc_flash.ld

	@echo [INFO] Create bin files
	$(CPRE) cp $(CKBOOT_BIN) $(MK_GENERATED_IMGS_PATH)/data/boot
	$(CPRE) cp $(M0BOOT_BIN) $(MK_GENERATED_IMGS_PATH)/data/m0flash
	$(CPRE) cp $(DSPFW_BIN) $(MK_GENERATED_IMGS_PATH)/data/dsp
	$(CPRE) cp $(CONFIG_YAML) $(MK_GENERATED_IMGS_PATH)/data/
	$(CPRE) cp $(LFS_BIN) $(MK_GENERATED_IMGS_PATH)/data/lfs
	$(CPRE) $(PRODUCT_BIN) image $(MK_GENERATED_IMGS_PATH)/images.zip -i $(MK_GENERATED_IMGS_PATH)/data -l -p -v "v1.0"
	$(CPRE) $(PRODUCT_BIN) image $(MK_GENERATED_IMGS_PATH)/images.zip -e $(MK_GENERATED_IMGS_PATH) -x -tb
	# $(CPRE) rm -fr $(MK_GENERATED_IMGS_PATH)/data

	$(CPRE) mv $(MK_GENERATED_IMGS_PATH)/total_image.hex $(MK_GENERATED_IMGS_PATH)/total_image_tmp.hex
	$(CPRE) cat $(MK_GENERATED_IMGS_PATH)/total_image_tmp.hex | awk 'BEGIN{pt=0}{if(match($$0,":020000040007F3")||pt==1){pt=1;print $$0}}' > $(MK_GENERATED_IMGS_PATH)/total_image.hex
	$(CPRE) rm $(MK_GENERATED_IMGS_PATH)/total_image_tmp.hex

	# $(CPRE) csky-abiv2-elf-objdump -D yoc.elf > yoc.asm
	$(CPRE) ls .gdbinit >/dev/null 2>&1 || (cp $(BOARD_PATH)/configs/gdbinit ./.gdbinit)
.PHONY:flash
flash:
	$(CPRE) $(PRODUCT_BIN) flash $(MK_GENERATED_IMGS_PATH)/images.zip -w prim

.PHONY:flashdsp
flashdsp:
	$(CPRE) $(PRODUCT_BIN) flash $(MK_GENERATED_IMGS_PATH)/images.zip -w dsp

flashlfs:
	$(CPRE) $(PRODUCT_BIN) flash $(MK_GENERATED_IMGS_PATH)/images.zip -w lfs

.PHONY:flashboot
flashboot:
	$(CPRE) $(PRODUCT_BIN) flash $(MK_GENERATED_IMGS_PATH)/images.zip -w m0flash
	$(CPRE) $(PRODUCT_BIN) flash $(MK_GENERATED_IMGS_PATH)/images.zip -w boot

.PHONY:flashall
flashall:
	$(CPRE) $(PRODUCT_BIN) flash $(MK_GENERATED_IMGS_PATH)/images.zip -w dsp
	$(CPRE) $(PRODUCT_BIN) flash $(MK_GENERATED_IMGS_PATH)/images.zip -w prim
	$(CPRE) $(PRODUCT_BIN) flash $(MK_GENERATED_IMGS_PATH)/images.zip -w lfs
#	$(CPRE) $(PRODUCT_BIN) flash $(MK_GENERATED_IMGS_PATH)/images.zip -a

.PHONY:clean
clean:
	$(CPRE) rm yoc_sdk out generated yoc.* -rf
	$(CPRE) scons -c
	$(CPRE) find . -name "*.[od]" -delete

%:
	$(CPRE) scons --component="$@" -j4
