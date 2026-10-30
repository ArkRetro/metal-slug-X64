/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06c0d8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6c0d8(void)

{
  uint uVar1;
  uint uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_13f1e();
  func_0x0006cc72();
  candidate_9f88();
  candidate_9ea8();
  *unaff_A6 = 0x6c102;
  unaff_A6[0xc] = ((int)-unaff_A6[0xc] >> 3) + unaff_A6[0xc];
  uVar2 = -0x10000 - unaff_A6[0xd] >> 3;
  uVar1 = unaff_A6[0xd];
  uVar3 = CARRY4(uVar2,uVar1);
  unaff_A6[0xd] = uVar2 + uVar1;
  candidate_1ac38();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_6c14c;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_6c14c;
  }
  return;
}
