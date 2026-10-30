/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01e604.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1e604(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  uVar1 = unaff_A6[0xd];
  uVar2 = CARRY4(*(uint *)((int)unaff_A6 + 0x72),uVar1);
  unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x72) + uVar1;
  func_0x000143e4();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = 0x1e4f2;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = 0x1e4f2;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = 0x1e4f2;
  }
  return;
}
