set(BLOWFISH_SOURCES bf.c bf_cfb64.c bf_enc.c bf_skey.c)
add_application(blowfish "${BLOWFISH_SOURCES}")