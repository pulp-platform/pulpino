__attribute__ ((section(".heapsram"))) unsigned int next;

int jrand()
{
    next = next * 1103515245 + 12345;
    return next;
}
