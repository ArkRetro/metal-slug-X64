/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02c0c2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte candidate_2c0c2(void)

{
  byte bVar1;
  char cVar2;
  int unaff_A6;

  if ((_DAT_0010000e & 3) == 0) {
    bVar1 = candidate_2eb4();
    bVar1 = bVar1 >> 3;
    if (bVar1 != *(byte *)(unaff_A6 + 0x8c)) {
      if ((char)*(byte *)(unaff_A6 + 0x8c) < (char)bVar1) {
        cVar2 = '\x01';
      }
      else {
        cVar2 = -1;
      }
      if (*(char *)(unaff_A6 + 0x8d) != '\0') {
        cVar2 = *(char *)(unaff_A6 + 0x8d);
      }
      *(char *)(unaff_A6 + 0x8c) = cVar2 + *(char *)(unaff_A6 + 0x8c);
      *(byte *)(unaff_A6 + 0x8c) = *(byte *)(unaff_A6 + 0x8c) & 0x1f;
      return bVar1;
    }
    *(short *)(unaff_A6 + 0x82) = *(short *)(unaff_A6 + 0x82) >> 1;
  }
  bVar1 = candidate_29c6();
  if ((bVar1 & 0x1f) == 0) {
    *(char *)(unaff_A6 + 0x8d) = -*(char *)(unaff_A6 + 0x8d);
  }
  return bVar1 & 0x1f;
}
