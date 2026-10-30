/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x013cf8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_13cf8(void)

{
  undefined4 in_D0;
  uint uVar1;
  undefined4 in_D1;
  uint unaff_D7;
  int extraout_A1;
  int iVar2;
  int extraout_A1_00;
  int unaff_A5;
  undefined8 uVar3;

  *(short *)(unaff_A5 + 0x4016) = (short)unaff_D7;
  candidate_1381c();
  candidate_1382a();
  iVar2 = extraout_A1;
  while( true ) {
    if (iVar2 == 0) {
      return CONCAT44(in_D0,in_D1);
    }
    uVar3 = candidate_12938();
    uVar1 = (uint)((ulonglong)uVar3 >> 0x20);
    if ((short)(unaff_D7 >> 0x10) == *(short *)(unaff_A5 + 0x4016)) break;
    candidate_1382a();
    unaff_D7 = unaff_D7 << 0x10 | unaff_D7 >> 0x10;
    iVar2 = extraout_A1_00;
  }
  return CONCAT44(uVar1 << 0x10 | uVar1 >> 0x10,(uint)uVar3 << 0x10 | (uint)uVar3 >> 0x10);
}
