/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08b214.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8b214(void)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  iVar3 = 0x20000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar3 = -0x20000;
  }
  unaff_A6[0xc] = (iVar3 - unaff_A6[0xc] >> 3) + unaff_A6[0xc];
  uVar2 = 0x40000 - unaff_A6[0xd] >> 4;
  uVar1 = unaff_A6[0xd];
  uVar4 = CARRY4(uVar2,uVar1);
  unaff_A6[0xd] = uVar2 + uVar1;
  candidate_1ac38();
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = 0x8b250;
  }
  return;
}
