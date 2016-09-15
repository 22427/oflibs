 TEMPLATE = subdirs

 SUBDIRS = \
			ofl_creator\
			libs\
			mesh_converter\
			mesh_viewer\
			stereo_mesh_viewer\
			stereo_image_viewer\
			processing_graph_example\




 # where to find the sub projects - give the folders
libs.subdir = ./libs
ofl_creator.subdir  = ./tools/ofl_creator
mesh_converter.subdir  = ./tools/mesh_converter
mesh_viewer.subdir  = ./tools/mesh_viewer
stereo_mesh_viewer.subdir  = ./tools/stereo_mesh_viewer
stereo_image_viewer.subdir  = ./tools/stereo_image_viewer
processing_graph_example.subdir  = ./tools/processing_graph_example

 # what subproject depends on others
 CONFIG += ordered c++11

DISTFILES += \
	README.md \
	LICENSE.md
