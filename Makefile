#

CROSS_COMPILE := $(DEB_HOST_GNU_TYPE)-

txt2mac : txt2mac
	$(CROSS_COMPILE)gcc -Wall -Os -o $@ $<

