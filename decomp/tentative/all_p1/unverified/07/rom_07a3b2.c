/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a3b2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a3b2(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  candidate_1b1dc();
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x82) = 2;
  *unaff_A6 = 0x7a3d4;
  iVar1 = unaff_A6[3];
  if (*(short *)(iVar1 + 0x82) == 2) {
    *(undefined2 *)((int)unaff_A6 + 0x82) = *(undefined2 *)(iVar1 + 0x82);
    *(undefined2 *)(iVar1 + 0x82) = 0;
  }
  candidate_1ac38();
  candidate_427a();
  if (*(short *)((int)unaff_A6 + 0x82) == 4) {
    *unaff_A6 = candidate_7a40c;
  }
  return;
}
