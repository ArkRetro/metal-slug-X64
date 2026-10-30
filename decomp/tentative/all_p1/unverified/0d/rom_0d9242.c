/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d9242.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d9242(void)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  int unaff_A6;

  sVar3 = 1;
  sVar1 = *(short *)(unaff_A6 + 0x6e);
  if (sVar1 != *(short *)(unaff_A6 + 0x7c)) {
    if (*(short *)(unaff_A6 + 0x7c) <= sVar1) {
      sVar3 = -1;
    }
    uVar2 = sVar3 + sVar1 & 0xff;
    *(ushort *)(unaff_A6 + 0x6e) = uVar2;
    if (uVar2 != *(ushort *)(unaff_A6 + 0x7c)) {
      return;
    }
  }
  return;
}
