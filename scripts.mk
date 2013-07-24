#quiet=
quiet=quiet_
echo-cmd = $(if $($(quiet)cmd_$(1)), echo '  $($(quiet)cmd_$(1))';)
cmd = $(echo-cmd) $(cmd_$(1))

quiet_cmd_cc_o_c=CC $*
 cmd_cc_o_c=$(Q)$(CC) $(CFLAGS) -c -o $@ $<
quiet_cmd_ld_bin=LD $*
 cmd_ld_bin=$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARY)
quiet_cmd_ld_slib=LD $*
 cmd_ld_slib=$(RM) $@ && \
	$(AR) -cvq $@ $^ > /dev/null && \
	$(RANLIB) $@
quiet_cmd_ld_dlib=LD $*
 cmd_ld_dlib=$(Q)$(LD) -shared -Wl,-soname,$@ -o $@ $^ $(LIBRARY)

$(obj)/%.o:$(src)/%.c
	@$(call cmd,cc_o_c)

ifdef STATIC
lib-static-target:=$(addprefix $(obj)/lib,$(addsuffix $(slib-ext:%=.%),$(lib-y)))
endif
ifdef DYNAMIC
lib-dynamic-target:=$(addprefix $(obj)/lib,$(addsuffix $(dlib-ext:%=.%),$(lib-y)))
endif
bin-target:=$(addprefix $(obj)/,$(addsuffix $(bin-ext:%=.%),$(bin-y)))

targets=$(lib-dynamic-target)
targets+=$(bin-target)
targets+=$(lib-static-target)

target-objs:=$(foreach t, $(lib-y) $(bin-y), $(if $($(t)-objs), $(addprefix $(obj)/,$($(t)-objs)), $(obj)/$(t).o))

CFLAGS+=-I./$(src)

all: $(targets)

.SECONDEXPANSION:
$(lib-static-target): $(obj)/lib%$(slib-ext:%=.%): $$(if $$(%-objs), $$(addprefix $(obj)/,$$(%-objs)), $(obj)/%.o)
	@$(call cmd,ld_slib)

$(lib-dynamic-target):CFLAGS+=-fPIC
$(lib-dynamic-target): $(obj)/lib%$(dlib-ext:%=.%): $$(if $$(%-objs), $$(addprefix $(obj)/,$$(%-objs)), $(obj)/%.o)
	@$(call cmd,ld_dlib)

$(bin-target): $(obj)/%$(bin-ext:%=.%) : $$(if $$(%-objs), $$(addprefix $(obj)/,$$(%-objs)), $(obj)/%.o) $(lib-dynamic-target) $$(%-libs)
	@$(call cmd,ld_bin)
