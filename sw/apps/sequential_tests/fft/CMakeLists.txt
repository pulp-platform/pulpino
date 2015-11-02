set(FFT_SOURCES main.c kiss_fft.c kiss_fftr.c fft_precomputed.c)
add_application(fft "${FFT_SOURCES}" LABELS "sequential_tests")
set(FFT_DEFINES KISS_FFT_USE_ALLOCA FIXED_POINT=16)
set_target_properties(fft PROPERTIES COMPILE_DEFINITIONS "${FFT_DEFINES}")
