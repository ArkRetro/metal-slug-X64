/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x023286.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_23286(void)

{
  int iVar1;
  undefined4 uVar2;
  ushort uVar3;
  byte *pbVar4;
  ushort uVar5;
  undefined4 *unaff_A6;
  bool bVar6;

  candidate_4256();
  *unaff_A6 = 0x2329c;
  candidate_1abc8();
  iVar1 = unaff_A6[3];
  *(undefined2 *)(iVar1 + 0x9a) = *(undefined2 *)((int)unaff_A6 + 0x9a);
  uVar5 = *(short *)(iVar1 + 0x9a) * 4;
  if (*(short *)((int)unaff_A6 + 0x9a) == 9) {
    uVar5 = uVar5 - 2;
  }
  uVar2 = unaff_A6[0xf];
  uVar3 = *(ushort *)(unaff_A6 + 0xf);
  bVar6 = CARRY2(uVar5,uVar3);
  *(ushort *)(unaff_A6 + 0xf) = uVar5 + uVar3;
  candidate_427a();
  if (bVar6) {
    iVar1 = unaff_A6[3];
    *(undefined1 *)(iVar1 + 0x60) = 0;
    pbVar4 = (byte *)(iVar1 + 0x8d);
    *pbVar4 = *pbVar4 | 2;
    *unaff_A6 = 0x22d08;
  }
  unaff_A6[0xf] = uVar2;
  return;
}
