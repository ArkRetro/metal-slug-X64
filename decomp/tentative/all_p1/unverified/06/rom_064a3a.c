/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x064a3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_64a3a(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 *unaff_A6;

  unaff_A6[0x22] = unaff_A6[0x1d];
  *unaff_A6 = 0x64a46;
  if (unaff_A6[0x22] != 0) {
    iVar3 = candidate_a49b0();
    iVar1 = unaff_A6[0x22];
    iVar2 = iVar1 - iVar3;
    unaff_A6[0x22] = iVar2;
    if (iVar2 == 0 || iVar1 < iVar3) {
      *unaff_A6 = candidate_64a64;
    }
  }
  return;
}
