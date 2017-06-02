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

add_subdirectory(aha-compress)
add_subdirectory(aha-mont64)
add_subdirectory(bs)
add_subdirectory(bubblesort)
add_subdirectory(cnt)
add_subdirectory(compress)
add_subdirectory(cover)
add_subdirectory(crc)
add_subdirectory(crc32)
#add_subdirectory(cubic)
add_subdirectory(dijkstra)
add_subdirectory(duff)
add_subdirectory(edn)
add_subdirectory(expint)
add_subdirectory(fac)
add_subdirectory(fdct)
add_subdirectory(fibcall)
add_subdirectory(fir)
add_subdirectory(frac)
add_subdirectory(insertsort)
add_subdirectory(janne_complex)
add_subdirectory(jfdctint)
add_subdirectory(lcdnum)
add_subdirectory(levenshtein)
add_subdirectory(ludcmp)
add_subdirectory(matmult-int)
add_subdirectory(matmult-float)
add_subdirectory(minver)
add_subdirectory(nbody)
add_subdirectory(ndes)
add_subdirectory(nettle-arcfour)
add_subdirectory(nettle-cast128)
add_subdirectory(nettle-des)
add_subdirectory(nettle-md5)
add_subdirectory(newlib-exp)
add_subdirectory(newlib-log)
add_subdirectory(newlib-mod)
add_subdirectory(newlib-sqrt)
add_subdirectory(ns)
add_subdirectory(nsichneu)
#add_subdirectory(picojpeg)
add_subdirectory(prime)
add_subdirectory(qsort) #it does not work
add_subdirectory(qurt)
add_subdirectory(recursion)
add_subdirectory(rijndael)
add_subdirectory(select)
add_subdirectory(sglib-arrayheapsort)
add_subdirectory(sglib-arrayquicksort)
add_subdirectory(sglib-queue)
add_subdirectory(slre)
add_subdirectory(sqrt)
add_subdirectory(st)
add_subdirectory(statemate)
add_subdirectory(stb_perlin)
add_subdirectory(stringsearch1)
add_subdirectory(strstr)
add_subdirectory(tarai)
add_subdirectory(template)
add_subdirectory(ud)
add_subdirectory(whetstone)
