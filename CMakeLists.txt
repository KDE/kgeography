kde4_header()
project(kgeography)

add_subdirectory( src ) 
add_subdirectory( data ) 
add_subdirectory( icons ) 




########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = src data icons 
