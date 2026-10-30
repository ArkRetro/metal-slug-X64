/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x064a64.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_64a64(void)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined4 *unaff_A6;

  uVar1 = candidate_4e962();
  unaff_A6[0x24] = uVar1;
  *unaff_A6 = 0x64a88;
  candidate_a49b0();
  iVar2 = candidate_4e5d0();
  uVar3 = iVar2 + unaff_A6[0x23];
  if (0x1fffff < uVar3) {
    uVar3 = 0x200000;
    *unaff_A6 = candidate_41ac;
  }
  unaff_A6[0x23] = uVar3;
  return;
}
