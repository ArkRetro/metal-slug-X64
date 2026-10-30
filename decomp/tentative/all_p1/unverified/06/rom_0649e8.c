/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0649e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_649e8(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *unaff_A6;

  uVar1 = candidate_4e962();
  unaff_A6[0x24] = uVar1;
  *unaff_A6 = 0x64a0c;
  candidate_a49b0();
  iVar2 = candidate_4e5d0();
  iVar2 = iVar2 + unaff_A6[0x23];
  if (iVar2 < 1) {
    iVar2 = 0;
    *unaff_A6 = candidate_64a3a;
  }
  unaff_A6[0x23] = iVar2;
  return;
}
