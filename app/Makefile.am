bin_PROGRAMS = propagatorInteractive loadTest

propagatorInteractive_CPPFLAGS = $(eigen3_CFLAGS) -I$(top_srcdir)/libspacesim
propagatorInteractive_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(eigen3_CFLAGS)
propagatorInteractive_LDADD = $(OGRE_LIBS) $(OIS_LIBS) ../libspacesim/libspacesim.la
propagatorInteractive_SOURCES = PropagatorInteractionTest.cpp PropagatorInteractionTest.hpp BaseApplication.hpp BaseApplication.cpp

loadTest_CPPFLAGS = $(eigen3_CFLAGS) -I$(top_srcdir)/libspacesim
loadTest_CXXFLAGS = $(eigen3_CFLAGS)
loadTest_LDADD = ../libspacesim/libspacesim.la
loadTest_SOURCES = LoadTest.cpp
