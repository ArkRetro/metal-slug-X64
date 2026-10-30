/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09153a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9153a(void)

{
  byte bVar1;
  char cVar2;
  int unaff_A6;

  bVar1 = candidate_91578();
  if ((bVar1 & 0xf) != *(byte *)(unaff_A6 + 0x91)) {
    if ((char)((bVar1 & 0xf) - *(char *)(unaff_A6 + 0x91)) < '\x01') {
      cVar2 = -1;
    }
    else {
      cVar2 = '\x01';
    }
    *(char *)(unaff_A6 + 0x91) = cVar2 + *(char *)(unaff_A6 + 0x91);
  }
  return;
}
