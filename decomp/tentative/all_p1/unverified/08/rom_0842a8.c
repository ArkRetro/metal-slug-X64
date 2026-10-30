/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0842a8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_842a8(void)

{
  int iVar1;
  ushort uVar2;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  candidate_4256();
  candidate_9ea8();
  uVar2 = candidate_29c6();
  unaff_A6[0xd] = (int)(short)((uVar2 & 0x300) + 0x200) << 8;
  *unaff_A6 = 0x842ee;
  uVar3 = 0;
  candidate_143dc();
  candidate_427a();
  if (!(bool)uVar3) {
    return;
  }
  candidate_40bc();
  *(int *)((int)unaff_A6 + 0x7a) = extraout_A0;
  iVar1 = unaff_A6[3];
  *(undefined2 *)(extraout_A0 + 0x6e) = *(undefined2 *)(iVar1 + 0x8c);
  *(undefined2 *)(extraout_A0 + 0x70) = *(undefined2 *)(iVar1 + 0x8e);
  *(undefined2 *)(extraout_A0 + 0x72) = *(undefined2 *)(iVar1 + 0x90);
  *(undefined2 *)(extraout_A0 + 0x74) = *(undefined2 *)(iVar1 + 0x92);
  *(undefined2 *)(extraout_A0 + 0x76) = *(undefined2 *)(iVar1 + 0x94);
  *(undefined2 *)(extraout_A0 + 0x78) = *(undefined2 *)(iVar1 + 0x96);
  *unaff_A6 = candidate_84344;
  return;
}
