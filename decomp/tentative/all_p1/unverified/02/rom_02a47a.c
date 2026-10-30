/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02a47a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2a47a(void)

{
  undefined4 *puVar1;
  ushort uVar2;
  short sVar3;
  int extraout_A0;
  int extraout_A0_00;
  int iVar4;
  int unaff_A6;
  bool bVar5;

  uVar2 = candidate_260c8();
  uVar2 = uVar2 & 3;
  if (*(char *)(unaff_A6 + 0x84) == '\x02') {
    uVar2 = 4;
  }
  *(undefined4 *)(unaff_A6 + 0x30) = *(undefined4 *)((short)(uVar2 << 2) + 0x2a8b8);
  bVar5 = false;
  candidate_1affc();
  iVar4 = extraout_A0;
  if (bVar5) {
    candidate_1af9a();
    iVar4 = extraout_A0_00;
  }
  sVar3 = *(short *)(unaff_A6 + 0x3c) - *(short *)(iVar4 + 0x3c);
  if (sVar3 < 0x61) {
    sVar3 = 0x60;
  }
  uVar2 = (ushort)(sVar3 - 0x60U) >> 4;
  if (2 < uVar2) {
    uVar2 = 3;
  }
  puVar1 = (undefined4 *)(*(int *)(unaff_A6 + 0x30) + (int)(short)(uVar2 * 0xc));
  *(undefined4 *)(unaff_A6 + 0x30) = puVar1[2];
  *(undefined4 *)(unaff_A6 + 0x7a) = puVar1[1];
  *(undefined4 *)(unaff_A6 + 0x34) = *puVar1;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    *(int *)(unaff_A6 + 0x30) = -*(int *)(unaff_A6 + 0x30);
  }
  return;
}
