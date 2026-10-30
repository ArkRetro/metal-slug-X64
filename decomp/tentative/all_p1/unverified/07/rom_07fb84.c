/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07fb84.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_7fb84(void)

{
  short in_D0w;
  int iVar1;
  undefined2 in_D1w;
  int unaff_D2;
  int extraout_A0;
  bool bVar2;
  undefined4 uStack_c;

  uStack_c = CONCAT22(in_D1w,uStack_c._2_2_);
  bVar2 = false;
  candidate_411c();
  if (bVar2) {
    iVar1 = candidate_1b220();
    *(int *)(extraout_A0 + 0x30) = uStack_c + *(int *)(extraout_A0 + 0x30);
    *(int *)(extraout_A0 + 0x34) = unaff_D2 + *(int *)(extraout_A0 + 0x34);
    return iVar1;
  }
  return (int)in_D0w;
}
