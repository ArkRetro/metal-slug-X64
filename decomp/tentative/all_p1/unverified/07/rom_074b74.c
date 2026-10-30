/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074b74.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x0001469e: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000146a2) */

void candidate_74b74(void)

{
  int iVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  undefined4 unaff_D4;
  uint unaff_D6;
  undefined4 unaff_D7;
  undefined4 extraout_A0;
  int unaff_A5;
  int unaff_A6;
  undefined8 uVar5;
  undefined4 uStack_4;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    *(int *)(unaff_A6 + 0x34) = *(int *)(unaff_A6 + 0x34) + -0xa000;
    uStack_4 = 0x143ea;
    candidate_128da();
    *(undefined1 *)(unaff_A5 + 0x4009) = 0;
    *(undefined4 *)(unaff_A5 + 0x4004) = 0;
    *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
    iVar3 = *(int *)(unaff_A6 + 0x30);
    iVar4 = *(int *)(unaff_A6 + 0x34);
    bVar2 = *(byte *)(unaff_A6 + 0x15);
    *(byte *)(unaff_A6 + 0x15) = bVar2 & 0xf7;
    if (((bVar2 & 8) != 0) && (-1 < iVar3)) {
      iVar3 = 0;
    }
    bVar2 = *(byte *)(unaff_A6 + 0x15);
    *(byte *)(unaff_A6 + 0x15) = bVar2 & 0xef;
    if (((bVar2 & 0x10) != 0) && (iVar3 < 0)) {
      iVar3 = 0;
    }
  }
  else {
    uStack_4 = 0x14570;
    candidate_128da();
    *(undefined4 *)(unaff_A5 + 0x4004) = 0;
    *(undefined1 *)(unaff_A5 + 0x4009) = 0xff;
    *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
    if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
      iVar3 = *(int *)(unaff_A6 + 0x62);
      iVar4 = 0;
    }
    else {
      if (((*(byte *)(unaff_A6 + 0x5f) & 0x20) == 0) && (*(char *)(unaff_A6 + 0x6d) == '@')) {
        *(uint *)(unaff_A5 + 0x4004) = *(uint *)(unaff_A5 + 0x4004) | 0x7fe;
      }
      if (((*(byte *)(unaff_A6 + 0x5f) & 0x10) != 0) &&
         (*(uint *)(unaff_A5 + 0x4004) = *(uint *)(unaff_A5 + 0x4004) | 0x28009800,
         *(char *)(unaff_A6 + 0x6d) != '@')) {
        *(uint *)(unaff_A5 + 0x4004) = *(uint *)(unaff_A5 + 0x4004) | 0x7fe;
      }
      uStack_4 = 0x14642;
      uVar5 = candidate_2e98();
      iVar3 = (int)uVar5;
      iVar4 = -(int)((ulonglong)uVar5 >> 0x20);
      if (*(char *)(unaff_A6 + 0x6c) == '\r') {
        iVar1 = *(int *)(unaff_A6 + 0x34) + -0x4000;
        iVar3 = *(int *)(unaff_A6 + 0x30) + (iVar3 - *(int *)(unaff_A6 + 0x30) >> 5);
        iVar4 = iVar1 + (iVar4 - iVar1 >> 5);
      }
    }
    bVar2 = *(byte *)(unaff_A6 + 0x15);
    *(byte *)(unaff_A6 + 0x15) = bVar2 & 0xf7;
    if (((bVar2 & 8) != 0) && (-1 < iVar3)) {
      iVar3 = 0;
      iVar4 = 0;
    }
    bVar2 = *(byte *)(unaff_A6 + 0x15);
    *(byte *)(unaff_A6 + 0x15) = bVar2 & 0xef;
    if (((bVar2 & 0x10) != 0) && (iVar3 < 0)) {
      iVar3 = 0;
      iVar4 = 0;
    }
    register0x0000003c = (BADSPACEBASE *)&uStack_4;
    uStack_4 = 0x146a2;
  }
  *(undefined4 *)((int)register0x0000003c + -4) = 0x14492;
  uVar5 = candidate_13968();
  *(undefined8 *)(unaff_A6 + 0x28) = uVar5;
  *(int *)(unaff_A6 + 0x30) = iVar3;
  *(int *)(unaff_A6 + 0x34) = iVar4;
  *(undefined4 *)(unaff_A6 + 0x38) = extraout_A0;
  bVar2 = *(byte *)(unaff_A6 + 0x5f) & 0xf8;
  if ((char)unaff_D6 < '\0') {
    bVar2 = bVar2 | 0x80;
    *(undefined1 *)(unaff_A6 + 0x5a) = 0xf;
  }
  *(byte *)(unaff_A6 + 0x15) =
       -((unaff_D6 & 2) != 0) & 0x80U |
       -((unaff_D6 & 1) != 0) & 0x40U | *(byte *)(unaff_A6 + 0x15) & 0x3f;
  *(byte *)(unaff_A6 + 0x5f) =
       -((unaff_D6 & 0x10) != 0) & 2U |
       -((unaff_D6 & 8) != 0) & 4U | -((unaff_D6 & 4) != 0) & 1U | bVar2;
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
