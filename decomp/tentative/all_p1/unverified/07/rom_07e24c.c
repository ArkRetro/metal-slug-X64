/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07e24c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7e24c(void)

{
  ushort uVar1;
  short sVar2;
  int extraout_A0;
  int unaff_A6;
  bool bVar3;

  candidate_1af9a();
  sVar2 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
  if (sVar2 < 0) {
    sVar2 = -sVar2;
  }
  if (0x1f < sVar2) {
    uVar1 = *(ushort *)(unaff_A6 + 0x28);
    bVar3 = uVar1 < 0x18;
    *(ushort *)(unaff_A6 + 0x28) = uVar1 - 0x18;
    candidate_1b0be();
    if (!bVar3) {
      *(short *)(unaff_A6 + 0x28) = *(short *)(unaff_A6 + 0x28) + 0x18;
      return;
    }
    *(short *)(unaff_A6 + 0x28) = *(short *)(unaff_A6 + 0x28) + 0x18;
  }
  return;
}
