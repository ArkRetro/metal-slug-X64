/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a6950.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a6950(void)

{
  int iVar1;
  byte bVar2;
  ushort uVar3;
  undefined2 uVar4;
  int extraout_A0;
  int extraout_A0_00;
  int iVar5;
  int unaff_A6;
  bool bVar6;

  bVar6 = false;
  candidate_411c();
  if (bVar6) {
    *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
  }
  *(undefined2 *)(extraout_A0 + 0x6e) = 0;
  *(undefined2 *)(extraout_A0 + 0x70) = 0;
  uVar3 = (ushort)*(byte *)(unaff_A6 + 0x83);
  iVar5 = extraout_A0;
  if (*(byte *)(unaff_A6 + 0x83) == 2) {
    uVar3 = candidate_29c6();
    iVar5 = extraout_A0_00;
  }
  *(ushort *)(iVar5 + 0x72) = (uVar3 & 1) + 1;
  *(ushort *)(iVar5 + 0x74) = (ushort)*(byte *)(unaff_A6 + 0x84);
  candidate_a6bb2();
  uVar4 = candidate_260c8();
  *(undefined2 *)(iVar5 + 0x76) = uVar4;
  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined1 *)(iVar5 + 0x69) = *(undefined1 *)(iVar1 + 0x69);
  *(undefined1 *)(iVar5 + 0x68) = *(undefined1 *)(iVar1 + 0x68);
  candidate_1acf2();
  bVar2 = *(char *)(unaff_A6 + 0x81) + 1;
  *(byte *)(unaff_A6 + 0x81) = bVar2;
  if (*(byte *)(unaff_A6 + 0x80) <= bVar2) {
    return;
  }
  return;
}
