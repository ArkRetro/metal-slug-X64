/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05dd2a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5dd2a(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  unaff_A6[0xd] = 0x4cccc;
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0xffff851f;
  unaff_A6[0xc] = 0xffff3334;
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x5dd66;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    uVar1 = unaff_A6[0xd];
    uVar2 = CARRY4(*(uint *)((int)unaff_A6 + 0x76),uVar1);
    unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x76) + uVar1;
  }
  else {
    *unaff_A6 = candidate_41ac;
    uVar2 = false;
  }
  candidate_427a();
  candidate_19504();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
