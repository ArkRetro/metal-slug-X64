/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d91e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x0001439e: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000143a2) */

void candidate_8d91e(void)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 unaff_D4;
  uint unaff_D6;
  undefined4 unaff_D7;
  undefined4 extraout_A0;
  int unaff_A5;
  int unaff_A6;
  undefined8 uVar4;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;

  if ((*(char *)(unaff_A6 + 0x90) == '\0') && ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0)) {
    *(int *)(unaff_A6 + 0x34) = *(int *)(unaff_A6 + 0x34) + -0x5000;
    uStack_4 = 0x143ea;
    candidate_128da();
    *(undefined1 *)(unaff_A5 + 0x4009) = 0;
    *(undefined4 *)(unaff_A5 + 0x4004) = 0;
    *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
    iVar2 = *(int *)(unaff_A6 + 0x30);
    uVar3 = *(undefined4 *)(unaff_A6 + 0x34);
    bVar1 = *(byte *)(unaff_A6 + 0x15);
    *(byte *)(unaff_A6 + 0x15) = bVar1 & 0xf7;
    if (((bVar1 & 8) != 0) && (-1 < iVar2)) {
      iVar2 = 0;
    }
    bVar1 = *(byte *)(unaff_A6 + 0x15);
    *(byte *)(unaff_A6 + 0x15) = bVar1 & 0xef;
    if (((bVar1 & 0x10) != 0) && (iVar2 < 0)) {
      iVar2 = 0;
    }
  }
  else {
    uStack_4 = 0x14370;
    candidate_128da();
    *(undefined1 *)(unaff_A5 + 0x4009) = 0;
    *(undefined4 *)(unaff_A5 + 0x4004) = 0;
    *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
    iVar2 = 0;
    uVar3 = 0;
    uStack_4 = *(undefined4 *)(unaff_A6 + 0x30);
    uStack_8 = *(undefined4 *)(unaff_A6 + 0x34);
    register0x0000003c = (BADSPACEBASE *)&uStack_c;
    uStack_c = 0x143a2;
  }
  *(undefined4 *)((int)register0x0000003c + -4) = 0x14492;
  uVar4 = candidate_13968();
  *(undefined8 *)(unaff_A6 + 0x28) = uVar4;
  *(int *)(unaff_A6 + 0x30) = iVar2;
  *(undefined4 *)(unaff_A6 + 0x34) = uVar3;
  *(undefined4 *)(unaff_A6 + 0x38) = extraout_A0;
  bVar1 = *(byte *)(unaff_A6 + 0x5f) & 0xf8;
  if ((char)unaff_D6 < '\0') {
    bVar1 = bVar1 | 0x80;
    *(undefined1 *)(unaff_A6 + 0x5a) = 0xf;
  }
  *(byte *)(unaff_A6 + 0x15) =
       -((unaff_D6 & 2) != 0) & 0x80U |
       -((unaff_D6 & 1) != 0) & 0x40U | *(byte *)(unaff_A6 + 0x15) & 0x3f;
  *(byte *)(unaff_A6 + 0x5f) =
       -((unaff_D6 & 0x10) != 0) & 2U |
       -((unaff_D6 & 8) != 0) & 4U | -((unaff_D6 & 4) != 0) & 1U | bVar1;
  *(undefined4 *)((int)register0x0000003c + -4) = unaff_D7;
  *(uint *)((int)register0x0000003c + -8) = unaff_D6;
  *(undefined4 *)((int)register0x0000003c + -0xc) = unaff_D4;
  *(short *)(unaff_A6 + 0x6c) = (short)((uint)unaff_D7 >> 8);
  *(undefined4 *)((int)register0x0000003c + -0x10) = 0x1450c;
  candidate_1abc8();
  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
    *(undefined4 *)((int)register0x0000003c + -0x10) = 0x1451c;
    candidate_1ad8c();
  }
  return;
}
