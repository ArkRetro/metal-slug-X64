/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x068c3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_68c3a(void)

{
  short sVar1;
  ushort uVar2;
  int extraout_A0;
  int unaff_A6;
  bool bVar3;

  uVar2 = *(short *)(unaff_A6 + 0x8e) - *(short *)(unaff_A6 + 0x28);
  if ((short)uVar2 < 0) {
    uVar2 = -uVar2;
  }
  bVar3 = uVar2 < *(ushort *)(unaff_A6 + 0x70);
  if (((short)uVar2 < (short)*(ushort *)(unaff_A6 + 0x70)) && (candidate_1af9a(), !bVar3)) {
    *(int *)(unaff_A6 + 0x86) = extraout_A0;
    sVar1 = *(short *)(unaff_A6 + 0x3c) - *(short *)(extraout_A0 + 0x3c);
    if (sVar1 < 0) {
      if (((*(byte *)(unaff_A6 + 0x15) & 1) != 0) &&
         (-*(short *)(unaff_A6 + 0x94) == sVar1 || -sVar1 < *(short *)(unaff_A6 + 0x94))) {
        return;
      }
    }
    else if (((*(byte *)(unaff_A6 + 0x15) & 1) == 0) && (sVar1 <= *(short *)(unaff_A6 + 0x94))) {
      return;
    }
  }
  return;
}
