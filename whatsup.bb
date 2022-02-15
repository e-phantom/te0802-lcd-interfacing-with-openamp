#
# This file is the whatsup recipe.
#

SUMMARY = "Simple whatsup application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://r5_amp.elf"
S = "${WORKDIR}"
INSANE_SKIP_${PN} = "arch"

do_install() {
	install -d ${D}/lib/firmware
	install -m 0644 ${S}/r5_amp.elf ${D}/lib/firmware/r5_amp.elf
}
FILES_${PN} = "/lib/firmware/r5_amp.elf"