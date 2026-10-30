/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f98e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f98e(void)

{
  byte bVar1;
  char cVar2;
  int unaff_A6;

  bVar1 = candidate_8f836();
  if ((bVar1 & 0xf) != *(byte *)(unaff_A6 + 0x8a)) {
    cVar2 = '\x01';
    if ((char)((bVar1 & 0xf) - *(char *)(unaff_A6 + 0x8a)) < '\x01') {
      cVar2 = -1;
    }
    *(char *)(unaff_A6 + 0x8a) = cVar2 + *(char *)(unaff_A6 + 0x8a);
    return;
  }
  return;
}
