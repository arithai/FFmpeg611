# $LynxId: makefile.inn,v 1.33 2024/01/14 21:39:14 tom Exp $
#
# Makefile for program source directory in GNU NLS utilities package.
# Copyright (C) 1995, 1996, 1997 by Ulrich Drepper <drepper@gnu.ai.mit.edu>
#
# This file may be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU General
# Public License but which still want to provide support for the GNU
# gettext functionality.
#
# Please note that the actual code is *not* freely available.

SHELL = /bin/sh


PACKAGE		= lynx
VERSION		= 2.9.1
NLS_TEXTDOMAIN	= lynx

srcdir		= .
top_srcdir	= ..


DESTDIR		= 

prefix		= /usr
exec_prefix	= ${prefix}

datadir		= /usr/share
localedir	= $(datadir)/locale
gnulocaledir	= $(prefix)/share/locale
gettextsrcdir	= $(prefix)/share/gettext/po

subdir		= po

DATADIR		= $(DESTDIR)$(datadir)
LOCALEDIR	= $(DESTDIR)$(localedir)
GNULOCALEDIR	= $(DESTDIR)$(gnulocaledir)
GETTEXTSRCDIR	= $(DESTDIR)$(gettextsrcdir)

INSTALL		= /usr/bin/install -c
INSTALL_DATA	= ${INSTALL} -m 644

CC		= gcc
GENCAT		= gencat
GMSGFMT		= PATH=../src:$$PATH /usr/bin/msgfmt.exe
MSGFMT		= /usr/bin/msgfmt.exe
XGETTEXT	= PATH=../src:$$PATH /usr/bin/xgettext.exe
MSGMERGE	= PATH=../src:$$PATH msgmerge

DEFS		= -DHAVE_CONFIG_H
CFLAGS		= -Wno-error=incompatible-pointer-types 
CPPFLAGS	= -DIGNORE_MSGFMT_HACK -DWIN_EX -D_WINDOWS_NSL -D_GNU_SOURCE -D_DEFAULT_SOURCE -DNCURSES_WIDECHAR -D_XOPEN_SOURCE_EXTENDED

INCLUDES	= -I.. -I$(top_srcdir)/intl

COMPILE		= $(CC) -c $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(XCFLAGS)

SOURCES		= # cat-id-tbl.c
POFILES		=  ca.po cs.po da.po de.po eo.po et.po fi.po fr.po hu.po id.po it.po ja.po nl.po pt_BR.po ro.po ru.po sl.po sv.po tr.po uk.po vi.po zh_CN.po zh_TW.po en.po
GMOFILES	=  ca.gmo cs.gmo da.gmo de.gmo eo.gmo et.gmo fi.gmo fr.gmo hu.gmo id.gmo it.gmo ja.gmo nl.gmo pt_BR.gmo ro.gmo ru.gmo sl.gmo sv.gmo tr.gmo uk.gmo vi.gmo zh_CN.gmo zh_TW.gmo en.gmo
DISTFILES	= ChangeLog makefile.inn POTFILES.in $(PACKAGE).pot \
stamp-cat-id $(POFILES) $(GMOFILES) $(SOURCES)

POTFILES = \
	../LYMessages.c \
     ../WWW/Library/Implementation/HTAABrow.c \
     ../WWW/Library/Implementation/HTAccess.c \
     ../WWW/Library/Implementation/HTFTP.c \
     ../WWW/Library/Implementation/HTFinger.c \
     ../WWW/Library/Implementation/HTNews.c \
     ../WWW/Library/Implementation/HTTCP.c \
     ../WWW/Library/Implementation/HTTP.c \
     ../WWW/Library/Implementation/HTTelnet.c \
     ../WWW/Library/Implementation/HTWAIS.c \
     ../WWW/Library/Implementation/HTWSRC.c \
     ../src/GridText.c \
     ../src/HTAlert.c \
     ../src/HTML.c \
     ../src/LYBookmark.c \
     ../src/LYCgi.c \
     ../src/LYClean.c \
     ../src/LYCookie.c \
     ../src/LYCurses.c \
     ../src/LYEdit.c \
     ../src/LYDownload.c \
     ../src/LYHistory.c \
     ../src/LYLeaks.c \
     ../src/LYList.c \
     ../src/LYLocal.c \
     ../src/LYMail.c \
     ../src/LYMain.c \
     ../src/LYMainLoop.c \
     ../src/LYNews.c \
     ../src/LYOptions.c \
     ../src/LYPrint.c \
     ../src/LYReadCFG.c \
     ../src/LYShowInfo.c \
     ../src/LYStyle.c \
     ../src/LYTraversal.c \
     ../src/LYUpload.c \
     ../src/LYUtils.c \
     ../src/LYrcFile.c 

CATALOGS	=  ca.gmo cs.gmo da.gmo de.gmo eo.gmo et.gmo fi.gmo fr.gmo hu.gmo id.gmo it.gmo ja.gmo nl.gmo pt_BR.gmo ro.gmo ru.gmo sl.gmo sv.gmo tr.gmo uk.gmo vi.gmo zh_CN.gmo zh_TW.gmo en.gmo
CATOBJEXT	= .gmo
INSTOBJEXT	= .mo

INSTOBJ		= $(NLS_TEXTDOMAIN)$(INSTOBJEXT)

.SUFFIXES:
.SUFFIXES: .c .o .po .pox .gmo .mo .msg .cat

.c.o:
	
	$(COMPILE) $<

.po.pox:
	$(MAKE) $(PACKAGE).pot
	$(MSGMERGE) -w 132 $< $(srcdir)/$(PACKAGE).pot -o $*.pox

.po.mo:
	@echo "translating $< to $@"
	$(MSGFMT) -o $@ $<

.po.gmo:
	@echo "translating $< to $@"
	@$(SHELL) -c 'LC_ALL=C \
	sed	-e s,http://lynx.browser.org/,https://lynx.invisible-island.net/,g \
		-e s,http://lynx.isc.org/,https://lynx.invisible-island.net/,g $< >pass1-$(*F).tmp;\
	if cmp -s $< pass1-$(*F).tmp; \
	then \
		echo ...no change made to homepage URL; \
	else \
		echo ...updated homepage URL;\
	fi; \
	if $(MSGMERGE) -w 132 pass1-$(*F).tmp $(srcdir)/$(PACKAGE).pot -o pass2-$(*F).tmp; \
	then \
		echo "...merged against $(srcdir)/$(PACKAGE).pot";\
	else \
		echo "...workaround without merging against $(srcdir)/$(PACKAGE).pot";\
		cat  $< >pass2-$(*F).tmp; \
	fi '
	rm -f $@ && $(GMSGFMT) -o $@ pass2-$(*F).tmp
	@-rm -f pass*-$(*F).tmp

.po.cat:
	@echo "translating $< to $@"
	sed -f ../intl/po2msg.sed < $< > $*.msg \
	  && rm -f $@ && $(GENCAT) $@ $*.msg


all: all-yes

all-yes: $(SOURCES) $(CATALOGS)
all-no:

en.po :
	msginit --no-translator -l en -i $(PACKAGE).pot
# cygwin's msginit 1.19.3 creates "en_US.po" rather than "en.po"...
	@$(SHELL) -c 'for n in en*.po; \
		do \
		if test -f $$n; \
		then \
			test $$n != $@ && mv -vf $$n $@; \
			break; \
		fi \
		done'

$(srcdir)/$(PACKAGE).pot: $(POTFILES)
	$(XGETTEXT) \
	  --default-domain=$(NLS_TEXTDOMAIN) \
	  --directory=$(top_srcdir) \
	  --add-comments \
	  --keyword=_ \
	  --keyword=N_ \
	  --files-from=$(srcdir)/POTFILES.in \
	&& test ! -f $(PACKAGE).po \
	   || ( rm -f $(srcdir)/$(PACKAGE).pot \
		&& ( sed -e 1,5d -e s/"Project-Id-Version: PACKAGE VERSION"/"Project-Id-Version: $(PACKAGE) $(VERSION)"/ $(PACKAGE).po > $(srcdir)/$(PACKAGE).pot ) )

$(srcdir)/cat-id-tbl.c: stamp-cat-id; @:
$(srcdir)/stamp-cat-id: $(PACKAGE).pot
	rm -f cat-id-tbl.tmp
	sed -f ../intl/po2tbl.sed $(srcdir)/$(PACKAGE).pot \
		| sed -e "s/@PACKAGE NAME@/$(PACKAGE)/" > cat-id-tbl.tmp
	if cmp -s cat-id-tbl.tmp $(srcdir)/cat-id-tbl.c; then \
	  rm cat-id-tbl.tmp; \
	else \
	  echo cat-id-tbl.c changed; \
	  rm -f $(srcdir)/cat-id-tbl.c; \
	  mv cat-id-tbl.tmp $(srcdir)/cat-id-tbl.c; \
	fi
	( cd $(srcdir) && rm -f stamp-cat-id && echo timestamp > stamp-cat-id )


install: install-exec install-data
install-exec:
install-data: install-data-yes
install-data-no: all
install-data-yes: all
	mkdir -p $(DATADIR)
	@catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  case "$$cat" in \
	    *.gmo) destdir=$(GNULOCALEDIR);; \
	    *)     destdir=$(LOCALEDIR);; \
	  esac; \
	  lang=`echo $$cat | sed 's/\$(CATOBJEXT)$$//'`; \
	  dir=$$destdir/$$lang/LC_MESSAGES; \
	  mkdir -p $$dir; \
	  if test -r $$cat; then \
	    $(INSTALL_DATA) $$cat $$dir/$(INSTOBJ); \
	    echo "installing $$cat as $$dir/$(INSTOBJ)"; \
	  else \
	    $(INSTALL_DATA) $(srcdir)/$$cat $$dir/$(INSTOBJ); \
	    echo "installing $(srcdir)/$$cat as" \
		 "$$dir/$(INSTOBJ)"; \
	  fi; \
	  if test -r $$cat.m; then \
	    $(INSTALL_DATA) $$cat.m $$dir/$(INSTOBJ).m; \
	    echo "installing $$cat.m as $$dir/$(INSTOBJ).m"; \
	  else \
	    if test -r $(srcdir)/$$cat.m ; then \
	      $(INSTALL_DATA) $(srcdir)/$$cat.m \
		$$dir/$(INSTOBJ).m; \
	      echo "installing $(srcdir)/$$cat as" \
		   "$$dir/$(INSTOBJ).m"; \
	    else \
	      true; \
	    fi; \
	  fi; \
	done
	if test "$(PACKAGE)" = "gettext"; then \
	  mkdir -p $(GETTEXTSRCDIR); \
	  $(INSTALL_DATA) $(srcdir)/makefile.inn \
			  $(GETTEXTSRCDIR)/makefile.inn; \
	else \
	  : ; \
	fi

# Define this as empty until I find a useful application.
installcheck:

uninstall:
	catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  lang=`echo $$cat | sed 's/\$(CATOBJEXT)$$//'`; \
	  rm -f $(LOCALEDIR)/$$lang/LC_MESSAGES/$(INSTOBJ); \
	  rm -f $(LOCALEDIR)/$$lang/LC_MESSAGES/$(INSTOBJ).m; \
	  rm -f $(GNULOCALEDIR)/$$lang/LC_MESSAGES/$(INSTOBJ); \
	  rm -f $(GNULOCALEDIR)/$$lang/LC_MESSAGES/$(INSTOBJ).m; \
	done
	rm -f $(GETTEXTSRCDIR)/po-makefile.inn

check: all

cat-id-tbl.o: ../intl/libgettext.h

dvi info tags TAGS ID:

mostlyclean:
	rm -f core core.* *.pox $(PACKAGE).po en.po *.old.po cat-id-tbl.tmp
	rm -f *.gmo *.mo *.tmp
	rm -fr *.o

clean: mostlyclean

distclean: clean
	rm -f cat-id-tbl.c stamp-cat-id
	rm -f makefile makefile.in POTFILES *.mo *.msg *.cat *.cat.m

maintainer-clean: distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f $(GMOFILES)

distdir = ../$(PACKAGE)-$(VERSION)/$(subdir)
dist distdir: update-po $(DISTFILES)
	dists="$(DISTFILES)"; \
	for file in $$dists; do \
	  ln $(srcdir)/$$file $(distdir) 2> /dev/null \
	    || cp -p $(srcdir)/$$file $(distdir); \
	done

# GNU msgfmt -C option can also check for X/Open compat.
check-po:
	( cd $(srcdir); \
	catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  lang=`echo $$cat | sed 's/\$(CATOBJEXT)$$//'`; \
	  echo "$$lang:"; \
	  $(MSGFMT) -f -c -v $$lang.po; \
	done )

update-po: makefile
	$(MAKE) $(PACKAGE).pot
	PATH=`pwd`/../src:$$PATH; \
	( cd $(srcdir); \
	catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  lang=`echo $$cat | sed 's/\$(CATOBJEXT)$$//'`; \
	  mv $$lang.po $$lang.old.po; \
	  echo "$$lang:"; \
	  if $(MSGMERGE) -w 132 $$lang.old.po $(PACKAGE).pot -o $$lang.po; then \
	    rm -f $$lang.old.po; \
	  else \
	    echo "msgmerge for $$cat failed!"; \
	    rm -f $$lang.po; \
	    mv $$lang.old.po $$lang.po; \
	  fi; \
	done )

POTFILES: POTFILES.in
	( if test 'x$(srcdir)' != 'x.'; then \
	    posrcprefix='$(top_srcdir)/'; \
	  else \
	    posrcprefix="../"; \
	  fi; \
	  rm -f $@-t $@ \
	    && (sed -e '/^#/d' -e '/^[ 	]*$$/d' \
		    -e "s@.*@	$$posrcprefix& \\\\@" < $(srcdir)/$@.in \
		| sed -e '$$s/\\$$//') > $@-t \
	    && chmod a-w $@-t \
	    && mv $@-t $@ )

makefile: makefile.inn ../config.status POTFILES
	( cd .. \
	  && CONFIG_FILES=$(subdir)/$@.in:$(subdir)/$@.inn CONFIG_HEADERS= \
	       $(SHELL) ./config.status )

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
