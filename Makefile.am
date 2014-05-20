SUBDIRS=libspacesim \
	app
ACLOCAL_AMFLAGS=-I m4

EXTRA_DIST = \
	bootstrap	\
	ChangeLog	\
	README

AUTOMAKE_OPTIONS = foreign

CHANGELOG_START = e29150e03963e290241aad0537a3234de748f9fd

ChangeLog:
	@echo Creating $@ based on git log
	@if test -d "$(srcdir)/.git"; then \
	  (GIT_DIR=$(top_srcdir)/.git ./missing --run \
	   git log $(CHANGELOG_START)..HEAD --stat) | fmt --split-only > $@ \
	  || ($(RM) $@; \
	      echo Failed to generate ChangeLog, your ChangeLog may be outdated >&2; \
	      (test -f $@ || echo git-log is required to generate this file >> $@)); \
	else \
	  test -f $@ || \
	  (echo A git checkout and git-log is required to generate ChangeLog >&2 && \
	  echo A git checkout and git-log is required to generate this file >> $@); \
	fi

README: README.md
	fgrep -v "[Build Status]" $< \
	        | markdown \
	        | html2text -style pretty -nobs \
	        | sed -e 's:&gt;:>:g' \
	        > $@
