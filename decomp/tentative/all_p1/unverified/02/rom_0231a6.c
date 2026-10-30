/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0231a6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_231a6(void)

{
  undefined4 uVar1;
  ushort uVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
  candidate_4256();
  *unaff_A6 = 0x231c6;
  candidate_1abc8();
  *(undefined2 *)(unaff_A6[3] + 0x9a) = *(undefined2 *)((int)unaff_A6 + 0x9a);
  uVar3 = *(short *)((int)unaff_A6 + 0x9a) * 4;
  if (*(short *)((int)unaff_A6 + 0x9a) == 9) {
    uVar3 = uVar3 - 2;
  }
  uVar1 = unaff_A6[0xf];
  uVar2 = *(ushort *)(unaff_A6 + 0xf);
  bVar4 = CARRY2(uVar3,uVar2);
  *(ushort *)(unaff_A6 + 0xf) = uVar3 + uVar2;
  candidate_427a();
  if (bVar4) {
    *unaff_A6 = candidate_23208;
  }
  unaff_A6[0xf] = uVar1;
  return;
}
