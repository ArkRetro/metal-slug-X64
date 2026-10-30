/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x058efc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_58efc(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  undefined4 *unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x20) = 5;
  candidate_59c2c();
  if ((*(byte *)((int)unaff_A6 + 0x8e) & 1) == 0) {
    candidate_40bc();
    iVar1 = extraout_A0;
  }
  else {
    candidate_40bc();
    iVar1 = extraout_A0_00;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    *(byte *)(iVar1 + 0x15) = *(byte *)(iVar1 + 0x15) | 1;
  }
  *unaff_A6 = 0x58f4a;
  candidate_59214();
  if (*(char *)((int)unaff_A6 + 0x8d) != '\0') {
    *unaff_A6 = candidate_58f66;
  }
  candidate_591fe();
  candidate_5929a();
  return;
}
