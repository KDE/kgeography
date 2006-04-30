kde4_header()

add_subdirectory(flags)

########### install files ###############

install_files(${DATA_INSTALL_DIR}/kgeography/ FILES
   africa.png
   europe.png
   france.png
   germany.png
   ncamerica.png
   southamerica.png
   spain.png
   usa.png
   africa.kgm
   europe.kgm
   france.kgm
   germany.kgm
   ncamerica.kgm
   southamerica.kgm
   spain.kgm
   usa.kgm
   italy.png
   italy.kgm
   poland.kgm
   poland.png
   austria.kgm
   austria.png
   norway.kgm
   norway.png
   china.png
   china.kgm
   brazil.png
   brazil.kgm
   italyprovince.kgm
   italyprovince.png
   asia.png
   asia.kgm
   world.png
   world.kgm
   canada.png
   canada.kgm
   netherlands.png
   netherlands.kgm
   hungary.kgm
   hungary.png
)

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = flags
#
#kgeographydir = $(kde_datadir)/kgeography/
#kgeography_DATA = africa.png europe.png france.png germany.png ncamerica.png \
#                  southamerica.png spain.png  usa.png africa.kgm europe.kgm france.kgm \
#                  germany.kgm ncamerica.kgm southamerica.kgm spain.kgm usa.kgm \
#                  italy.png italy.kgm poland.kgm poland.png austria.kgm austria.png \
#                  norway.kgm norway.png china.png china.kgm brazil.png brazil.kgm \
#                  italyprovince.kgm italyprovince.png asia.png asia.kgm \
#                  world.png world.kgm canada.png canada.kgm netherlands.png netherlands.kgm \
#                  hungary.kgm hungary.png
