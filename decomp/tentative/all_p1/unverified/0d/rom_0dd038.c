/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dd038.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dd038(void)

{
  uint uVar1;
  uint uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0xdd068;
  unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x82) - unaff_A6[0xc] >> 4) + unaff_A6[0xc];
  uVar2 = *(int *)((int)unaff_A6 + 0x86) - unaff_A6[0xd] >> 4;
  uVar1 = unaff_A6[0xd];
  uVar3 = CARRY4(uVar2,uVar1);
  unaff_A6[0xd] = uVar2 + uVar1;
  candidate_1ac38();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_dd0b0;
    uVar3 = 0;
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_dd0b0;
  }
  return;
}
