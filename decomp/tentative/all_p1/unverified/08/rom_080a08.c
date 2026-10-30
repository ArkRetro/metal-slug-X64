/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x080a08.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_80a08(void)

{
  undefined2 uVar1;
  short sVar2;
  ushort uVar3;
  int unaff_A6;

  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x82) = uVar1;
  sVar2 = candidate_29f2();
  if (sVar2 == 0) {
    *(short *)(unaff_A6 + 0x82) = *(short *)(unaff_A6 + 0x82) << 1;
  }
  uVar3 = candidate_29c6();
  *(short *)(unaff_A6 + 0x82) = *(short *)(unaff_A6 + 0x82) - (uVar3 & 7);
  return;
}
