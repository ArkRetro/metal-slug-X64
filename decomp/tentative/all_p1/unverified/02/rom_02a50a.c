/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02a50a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2a50a(void)

{
  short sVar2;
  ushort uVar3;
  ushort uVar4;
  uint uVar1;
  int extraout_A0;
  int extraout_A0_00;
  int iVar5;
  undefined4 *puVar6;
  int unaff_A6;
  undefined1 in_CF;

  candidate_1affc();
  iVar5 = extraout_A0;
  if ((bool)in_CF) {
    candidate_1af9a();
    iVar5 = extraout_A0_00;
  }
  sVar2 = *(short *)(unaff_A6 + 0x3c) - *(short *)(iVar5 + 0x3c);
  if (sVar2 < 0x21) {
    sVar2 = 0x20;
  }
  uVar3 = (ushort)(sVar2 - 0x20U) >> 4;
  if (0xe < uVar3) {
    uVar3 = 0xf;
  }
  uVar4 = candidate_260c8();
  puVar6 = (undefined4 *)((uint)uVar3 * 0xc + *(int *)((short)((uVar4 & 3) << 2) + 0x2cb114));
  *(undefined4 *)(unaff_A6 + 0x30) = puVar6[2];
  *(undefined4 *)(unaff_A6 + 0x7a) = puVar6[1];
  *(undefined4 *)(unaff_A6 + 0x34) = *puVar6;
  uVar1 = candidate_29c6();
  *(int *)(unaff_A6 + 0x30) = ((uVar1 & 0xffff) - 0x8000) + *(int *)(unaff_A6 + 0x30);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    *(int *)(unaff_A6 + 0x30) = -*(int *)(unaff_A6 + 0x30);
  }
  return;
}
