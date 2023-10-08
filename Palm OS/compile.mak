N	=	Hi-Launcher
C	=	Rhil
BUILD	=	build_$(V)

TEMPFILE	=	$(BUILD)/temp.dat
RESOURCES	=	$(BUILD)/*.1.o.grc $(BUILD)/*.set.2.o.grc $(BUILD)/*.pop.2.o.grc $(BUILD)/$(N)_data.Rsrc.prc $(BUILD)/$(N).Rsrc.prc

$(N)_$(V).prc	:	$(RESOURCES)
	echo "==== BUILD-PRC"
	build-prc -z 7 $@ $(N) $(C) $(RESOURCES)

$(BUILD)/*.1.o.grc	:	$(BUILD)/$(N).1.o
	echo "===== OBJ MAIN"
	m68k-palmos-obj-res $<
	mv *.grc $(BUILD)

$(BUILD)/*.set.2.o.grc	:	$(BUILD)/$(N).set.2.o
	echo "===== OBJ SET2"
	m68k-palmos-obj-res $< 2>&1 | grep -v -fgrep_skip_gcc.txt > $(TEMPFILE); cat $(TEMPFILE); test ! -s $(TEMPFILE)
	rm $(BUILD)/temp.dat 2>&1 | grep tekstktoryniewystapi ; true
	mv *.grc $(BUILD)

$(BUILD)/*.pop.2.o.grc	:	$(BUILD)/$(N).pop.2.o
	echo "===== OBJ POP2"
	m68k-palmos-obj-res $<
	mv *.grc $(BUILD)

$(BUILD)/$(N).pop.1.a	:	$(N).pop.c *.h
	echo "======= GCC POP"
	m68k-palmos-gcc -c -D $V -O2 -Wunused -Wimplicit-function-declaration $< -o $@

$(BUILD)/$(N).1.a	:	$(N).c *.h
	echo "======= GCC MAIN"
	m68k-palmos-gcc -c -D $V -O2 -Wunused -Wimplicit-function-declaration $< -o $@

$(BUILD)/$(N).set.1.a	:	$(N).set.c *.h
	echo "======= GCC SET"
	m68k-palmos-gcc -c -D $V -O2 -Wunused -Wimplicit-function-declaration $< -o $@

$(BUILD)/$(N).1.o	:	$(BUILD)/$(N).pop.1.a $(BUILD)/$(N).1.a $(BUILD)/$(N).set.1.a
	echo "====== LINK MAIN"
	m68k-palmos-gcc -T text_64k -O2 -Wunused -nostartfiles /usr/m68k-palmos/lib/crt0.o /usr/m68k-palmos/lib/libc.a /usr/m68k-palmos/lib/libcrt.a $(BUILD)/$(N).pop.1.a $(BUILD)/$(N).1.a $(BUILD)/$(N).set.1.a -o $@ -lPalmOSGlue

$(BUILD)/$(N).set.2.o	:	$(N).set.2.c *.h
	echo "====== GCC+LINK SET2"
	m68k-palmos-gcc -D $V -O2 -Wunused -Wimplicit-function-declaration -nostartfiles $< -o $@

$(BUILD)/$(N).pop.2.o	:	$(N).pop.2.c *.h
	echo "====== GCC+LINK POP2"
	m68k-palmos-gcc -D $V -O2 -Wunused -Wimplicit-function-declaration -nostartfiles $< -o $@

$(BUILD)/%.Rsrc.prc	:	%.rcp
	echo "===== PILRC"
	pilrc -d $V -q -ro -o $@ $< 2>&1 | grep -v -fgrep_skip_pilrc.txt > $(TEMPFILE); cat $(TEMPFILE); test ! -s $(TEMPFILE)
	rm $(BUILD)/temp.dat 2>&1 | grep tekstktoryniewystapi ; true
