include_directories(
  ${CMAKE_SOURCE_DIR}/libs/beebs_lib/inc
  ${CMAKE_SOURCE_DIR}/libs/stdio_lib/inc
  ${CMAKE_SOURCE_DIR}/libs/string_lib/inc
  ${CMAKE_SOURCE_DIR}/libs/sys_lib/inc
  .
)


set(BEEBS_CORE_SOURCES main.c)

add_library(beebs.core STATIC ${BEEBS_CORE_SOURCES})


# vlsi soc paper compile flags:
# base "-mno-hwlp  -mno-idxls -mno-rrls  -mno-lv32 -mno_unalign-ls  -mno-mac3"
# full "-midxls -mrrls  -mhwlp -mmax-hwloops=2  -mlv32 -munalign-ls  -mmac3"
# note: base still includes min/max/cnt instructions (-mno-crtl missing)

# TODO: -fno-builtin was used to build, check if needed

add_subdirectory(fir)
add_subdirectory(sqrt)
add_subdirectory(ud)
add_subdirectory(insertsort)
add_subdirectory(levenshtein)
add_subdirectory(fibcall)
add_subdirectory(expint)
add_subdirectory(bs)
add_subdirectory(dijkstra)
add_subdirectory(cover)
add_subdirectory(nbody)
add_subdirectory(nettle-des)
add_subdirectory(newlib-mod)
add_subdirectory(jfdctint)
add_subdirectory(newlib-exp)
add_subdirectory(picojpeg)
add_subdirectory(nettle-cast128)
add_subdirectory(newlib-log)
add_subdirectory(qsort) #it does not work
add_subdirectory(sglib-arraysort)
add_subdirectory(qurt)
add_subdirectory(select)
add_subdirectory(nsichneu)
add_subdirectory(aha-mont64)
add_subdirectory(aha-compress)
add_subdirectory(bubblesort)
add_subdirectory(cnt)
add_subdirectory(compress)
add_subdirectory(crc)
add_subdirectory(crc32)
add_subdirectory(cubic)
add_subdirectory(duff)
add_subdirectory(edn)
add_subdirectory(fac)
add_subdirectory(fdct)
add_subdirectory(frac)
add_subdirectory(janne_complex)
add_subdirectory(lcdnum)
add_subdirectory(ludcmp)