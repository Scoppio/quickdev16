##############################
### platform configuration ###
##############################

objects := main $(if $(call streq,$(platform),win),resource) $(objects)

ifeq ($(platform),mac)  
    moc = moc
    rcc = rcc
    link +=-F/Developer/SDKs/Qt/lib \
	   -L/Developer/SDKs/Qt/lib \
	   -framework QtOpenGL \
	   -framework QtGui \
	   -framework Carbon \
	   -framework AppKit \
	   -framework QtCore 
    qtflags= -I/Developer/SDKs/Qt/include/ \
             -I/Developer/SDKs/Qt/include/Qt \
             -I/Developer/SDKs/Qt/include/QtCore \
             -I/Developer/SDKs/Qt/include/QtGui

endif
ifeq ($(platform),x) # X11
    moc = moc-qt4
    rcc = rcc
    link += $(call mklib,Xtst)
    link += `pkg-config --libs QtCore QtGui`
    qtflags = `pkg-config --cflags QtCore QtGui`
else ifeq ($(platform),win) # Windows
  qtdir = c:/qt450

  link += $(call mklibpath,$(qtdir)/lib)
  link += $(call mklibpath,$(qtdir)/plugins/imageformats)

  link += $(call mklib,mingw32)
  link += $(call mklib,qtmain)
  link += $(call mklib,QtGui)
  link += $(call mklib,comdlg32)
  link += $(call mklib,oleaut32)
  link += $(call mklib,imm32)
  link += $(call mklib,winmm)
  link += $(call mklib,winspool)
  link += $(call mklib,msimg32)
  link += $(call mklib,QtCore)
  link += $(call mklib,ole32)
  link += $(call mklib,advapi32)
  link += $(call mklib,ws2_32)
  link += $(call mklib,uuid)
  link += $(call mklib,gdi32)

  # optional image-file support:
  # link += $(call mklib,qjpeg)
  # link += $(call mklib,qmng)

  qtflags  = $(call mkincpath,$(qtdir)/include)
  qtflags += $(call mkincpath,$(qtdir)/include/QtCore)
  qtflags += $(call mkincpath,$(qtdir)/include/QtGui)
endif

moc_objects = \
  $(ui)/base/main.moc \
  $(ui)/base/loader.moc \
  $(ui)/base/htmlviewer.moc \
  $(ui)/base/about.moc \
  $(ui)/settings/settings.moc \
  $(ui)/settings/video.moc \
  $(ui)/settings/audio.moc \
  $(ui)/settings/input.moc \
  $(ui)/settings/paths.moc \
  $(ui)/settings/cheateditor.moc \
  $(ui)/settings/advanced.moc \
  $(ui)/settings/utility/inputcapture.moc \
  $(ui)/settings/utility/codeeditor.moc \

#############
### rules ###
#############

%.moc: $<; $(moc) $(patsubst %.moc,%.hpp,$@) -o $@
$(foreach object,$(moc_objects),$(eval $(object): $(patsubst %.moc,%.hpp,$(object))))

obj/main.$(obj): $(ui)/main.cpp \
$(ui)/* $(ui)/input/* $(ui)/utility/* $(ui)/base/* $(ui)/settings/* $(ui)/settings/utility/* \
data/*
	$(call compile,$(qtflags))

$(ui)/resource/resource.rcc: $(ui)/resource/resource.qrc data/*
	$(rcc) $(ui)/resource/resource.qrc -o $(ui)/resource/resource.rcc

obj/resource.$(obj): $(ui)/resource/resource.rc
	windres $(ui)/resource/resource.rc obj/resource.$(obj)

###############
### targets ###
###############

ui_build: $(ui)/resource/resource.rcc $(moc_objects);
ui_clean:
	-$(foreach object,$(moc_objects),@$(call delete,$(object)))
	-@$(call delete,$(ui)/resource/resource.rcc)
