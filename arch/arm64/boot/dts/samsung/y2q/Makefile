define __sec_dtbo_build
dtbo-$(2) += $(1)
$(1)-base := $(3)
endef

define sec_dtbo_build
$(foreach dtbo, $(1), $(eval $(call __sec_dtbo_build, $(dtbo),$(2),$(3))))
endef

QCOM_DTB := ../../vendor/qcom
SEC_KONA_BASE_DTB := $(QCOM_DTB)/kona.dtb $(QCOM_DTB)/kona-v2.dtb $(QCOM_DTB)/kona-v2.1.dtb

ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)

SEC_Y2Q_JPN_DTBO := kona-sec-y2q-jpn-overlay-r08.dtbo kona-sec-y2q-jpn-overlay-r10.dtbo kona-sec-y2q-jpn-overlay-r13.dtbo kona-sec-y2q-jpn-overlay-r14.dtbo kona-sec-y2q-jpn-overlay-r15.dtbo

$(eval $(call sec_dtbo_build, $(SEC_Y2Q_JPN_DTBO),$(CONFIG_MACH_Y2Q_JPN_SINGLE),$(SEC_KONA_BASE_DTB)))

endif
