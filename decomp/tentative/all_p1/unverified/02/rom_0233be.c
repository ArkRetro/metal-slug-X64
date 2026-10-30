/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0233be.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_233be(void)

{
  int iVar1;
  undefined4 uVar2;
  ushort uVar3;
  ushort uVar4;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_120f8();
  candidate_4256();
  *unaff_A6 = 0x233de;
  candidate_1abc8();
  iVar1 = unaff_A6[3];
  *(undefined2 *)(iVar1 + 0x9a) = *(undefined2 *)((int)unaff_A6 + 0x9a);
  uVar4 = *(short *)(iVar1 + 0x9a) * 4;
  if (*(short *)((int)unaff_A6 + 0x9a) == 9) {
    uVar4 = uVar4 - 2;
  }
  uVar2 = unaff_A6[0xf];
  uVar3 = *(ushort *)(unaff_A6 + 0xf);
  bVar5 = CARRY2(uVar4,uVar3);
  *(ushort *)(unaff_A6 + 0xf) = uVar4 + uVar3;
  candidate_427a();
  if (bVar5) {
    iVar1 = unaff_A6[3];
    *(undefined1 *)(iVar1 + 0x60) = 0;
    *(byte *)(iVar1 + 0x8d) = *(byte *)(iVar1 + 0x8d) & 0xfd;
    *(undefined1 *)(iVar1 + 0x8e) = 0;
    *unaff_A6 = 0x22d08;
  }
  unaff_A6[0xf] = uVar2;
  return;
}
