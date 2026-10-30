/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x091416.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_91416(void)

{
  byte bVar1;
  int unaff_A6;
  bool bVar2;

  if ((char)_DAT_0010000e != *(char *)(unaff_A6 + 0x8d)) {
    *(char *)(unaff_A6 + 0x8d) = (char)_DAT_0010000e;
    bVar1 = *(byte *)(unaff_A6 + 0x8e);
    bVar2 = 0xfe < bVar1;
    *(byte *)(unaff_A6 + 0x8e) = bVar1 + 1;
    candidate_918fc();
    if (bVar2) {
      *(byte *)(unaff_A6 + 0x96) = *(byte *)(unaff_A6 + 0x96) | 0x20;
    }
    else {
      *(byte *)(unaff_A6 + 0x96) = *(byte *)(unaff_A6 + 0x96) & 0xdf;
    }
    *(undefined2 *)(unaff_A6 + 0x82) = *(undefined2 *)(unaff_A6 + 0x3c);
    *(undefined2 *)(unaff_A6 + 0x84) = *(undefined2 *)(unaff_A6 + 0x40);
    if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
      *(int *)(unaff_A6 + 0x34) = *(int *)(unaff_A6 + 0x34) + -0xa000;
      candidate_14c2e();
      if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
        *(undefined4 *)(unaff_A6 + 0x30) = 0;
        *(undefined4 *)(unaff_A6 + 0x34) = 0;
      }
    }
    else {
      candidate_14c6a();
    }
    candidate_91a72();
  }
  return;
}
