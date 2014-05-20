bin_PROGRAMS = propagatorInteractive

propagatorInteractive_CPPFLAGS = $(eigen3_CFLAGS) -I$(top_srcdir)/libspacesim
propagatorInteractive_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(eigen3_CFLAGS)
propagatorInteractive_LDADD = $(OGRE_LIBS) $(OIS_LIBS) ../libspacesim/libspacesim.la
propagatorInteractive_SOURCES = PropagatorInteractionTest.cpp PropagatorInteractionTest.hpp BaseApplication.hpp BaseApplication.cpp
