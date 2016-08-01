 TEMPLATE = subdirs

 SUBDIRS = \
			ofl_creator\
		   libs\
		mesh_converter\
mesh_viewer



 # where to find the sub projects - give the folders
 libs.subdir = ./libs
 ofl_creator.subdir  = ./tools/ofl_creator
mesh_converter.subdir  = ./tools/mesh_converter
mesh_viewer.subdir  = ./tools/mesh_viewer

 # what subproject depends on others
 CONFIG += ordered

DISTFILES += \
	README.md \
    LICENSE.md
