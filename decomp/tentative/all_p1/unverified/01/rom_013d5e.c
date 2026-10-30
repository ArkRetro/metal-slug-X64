/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x013d5e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_13d5e(void)

{
  uint uVar1;
  short unaff_D7w;
  int extraout_A1;
  int iVar2;
  int extraout_A1_00;
  undefined8 uVar3;

  candidate_1381c();
  uVar3 = candidate_1382a();
  iVar2 = extraout_A1;
  while( true ) {
    if (iVar2 == 0) {
      return uVar3;
    }
    uVar3 = candidate_12938();
    uVar1 = (uint)((ulonglong)uVar3 >> 0x20);
    if (unaff_D7w != 0) break;
    uVar3 = candidate_1382a();
    iVar2 = extraout_A1_00;
  }
  return CONCAT44(uVar1 << 0x10 | uVar1 >> 0x10,(uint)uVar3 << 0x10 | (uint)uVar3 >> 0x10);
}
