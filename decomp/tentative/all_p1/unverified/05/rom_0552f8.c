/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0552f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

char candidate_552f8(void)

{
  byte bVar1;
  int in_A0;

  bVar1 = *(byte *)(in_A0 + 0x60);
  return ((bVar1 & 1) != 0) + ((bVar1 >> 1 & 1) != 0) + ((bVar1 >> 2 & 1) != 0);
}
