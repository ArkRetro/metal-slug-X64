/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a360.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a360(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_4256();
  *unaff_A6 = 0x7a376;
  bVar2 = false;
  candidate_7b4ee();
  if (bVar2) {
    *unaff_A6 = candidate_7a3b2;
  }
  candidate_1ac38();
  candidate_427a();
  iVar1 = unaff_A6[3];
  if (*(short *)(iVar1 + 0x82) == 2) {
    *(undefined2 *)((int)unaff_A6 + 0x82) = *(undefined2 *)(iVar1 + 0x82);
    *(undefined2 *)(iVar1 + 0x82) = 0;
    *unaff_A6 = candidate_7a3b2;
  }
  return;
}
