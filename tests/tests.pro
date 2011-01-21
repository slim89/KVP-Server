TEMPLATE = subdirs
contains(BUILD_FEATURES,check) {
    system(echo CHECK)
    subdirs=$$system(ls -1d ut_*/*.pro ft_*/*.pro 2>/dev/null | sed 's!/.*!!')
    for(suitename, subdirs):{
       SUBDIRS += $$suitename
    }
} else {
SUBDIRS =
}
