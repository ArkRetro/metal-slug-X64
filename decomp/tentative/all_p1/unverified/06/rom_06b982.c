/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06b982.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6b982(void)

{
  ushort uVar1;
  int unaff_A6;
  undefined1 uVar2;

  if (((*(short *)(unaff_A6 + 0x82) == 0) &&
      (uVar1 = *(ushort *)(unaff_A6 + 0x3c), 0xf < (short)uVar1)) &&
     (uVar2 = uVar1 < 0x130, (short)uVar1 < 0x131)) {
    candidate_6b3fc();
    candidate_1b08a();
    if (((!(bool)uVar2) &&
        (((*(byte *)(unaff_A6 + 0x92) & 1) == 0 || ((*(byte *)(unaff_A6 + 0x92) & 4) != 0)))) &&
       (((*(byte *)(unaff_A6 + 0x92) & 0x80) == 0 || ((*(byte *)(unaff_A6 + 0x92) & 8) == 0)))) {
      return;
    }
  }
  return;
}
