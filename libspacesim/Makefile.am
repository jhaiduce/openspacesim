lib_LTLIBRARIES=libspacesim.la

libspacesim_la_CPPFLAGS = -I$(top_srcdir) $(eigen3_CFLAGS)
libspacesim_la_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(eigen3_CFLAGS)
libspacesim_la_LIBADD = $(OGRE_LIBS) $(OIS_LIBS)
libspacesim_la_SOURCES = Spacecraft.hpp Spacecraft.cpp SpaceObject.hpp TimeIntegrator.hpp TimeIntegrator.cpp MassiveObject.hpp MassiveObject.cpp Actuator.hpp Actuator.cpp SimulationManager.hpp SimulationManager.cpp GameManager.hpp GameManager.cpp

EXTRA_DIST = bootstrap
AUTOMAKE_OPTIONS = foreign
