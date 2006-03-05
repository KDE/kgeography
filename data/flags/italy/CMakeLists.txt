kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kgeography/flags/italy FILES it-bas200.png  it-emi200.png  it-lig200.png  it-mol200.png  it-sar200.png  it-tus200.png                it-abr200.png  it-cal200.png  it-fri200.png  it-lom200.png  it-pie200.png  it-sic200.png  it-umb200.png               it-aos200.png  it-cam200.png  it-lat200.png  it-mar200.png  it-pug200.png  it-tre200.png  it-ven200.png )

kde4_footer()



#original Makefile.am contents follow:

#flagsdir = $(kde_datadir)/kgeography/flags/italy
#flags_DATA = it-bas200.png  it-emi200.png  it-lig200.png  it-mol200.png  it-sar200.png  it-tus200.png  \
#             it-abr200.png  it-cal200.png  it-fri200.png  it-lom200.png  it-pie200.png  it-sic200.png  it-umb200.png \
#             it-aos200.png  it-cam200.png  it-lat200.png  it-mar200.png  it-pug200.png  it-tre200.png  it-ven200.png
