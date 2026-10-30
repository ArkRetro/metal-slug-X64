/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d71ca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort candidate_d71ca(void)

{
  ushort uVar1;
  short sVar2;
  int unaff_A6;

  if ((_DAT_0010000e & 7) == 0) {
    uVar1 = candidate_1b12e();
    sVar2 = 1;
    if (uVar1 >> 3 != *(ushort *)(unaff_A6 + 0x82)) {
      if ((short)(uVar1 >> 3) <= (short)*(ushort *)(unaff_A6 + 0x82)) {
        sVar2 = -1;
      }
      if (*(short *)(unaff_A6 + 0x84) != 0) {
        sVar2 = *(short *)(unaff_A6 + 0x84);
      }
      *(short *)(unaff_A6 + 0x82) = sVar2 + *(short *)(unaff_A6 + 0x82);
      *(ushort *)(unaff_A6 + 0x82) = *(ushort *)(unaff_A6 + 0x82) & 0x1f;
      uVar1 = candidate_d7232();
      return uVar1;
    }
  }
  uVar1 = candidate_29c6();
  if ((uVar1 & 0x1f) == 0) {
    *(short *)(unaff_A6 + 0x84) = -*(short *)(unaff_A6 + 0x84);
  }
  return uVar1 & 0x1f;
}
