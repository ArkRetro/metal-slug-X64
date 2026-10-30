/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01a7f4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x0001a824: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x0001a828) */

uint candidate_1a7f4(void)

{
  undefined4 uVar1;
  uint uVar2;
  byte bVar3;
  char cVar4;
  ushort uVar5;
  byte *pbVar6;
  int iVar7;
  code *extraout_A1;
  code *pcVar8;
  int unaff_A5;
  undefined4 uStack_a;
  int iStack_6;

  if (*(char *)(unaff_A5 + 0x7d82) == '\0') {
    iVar7 = unaff_A5 + -0x7ffc;
  }
  else {
    iVar7 = 0xd00034;
    if (*(char *)(unaff_A5 + 0x7e80) != '\0') {
      iVar7 = unaff_A5 + 0x7e00;
    }
  }
  if (DAT_00100003 != '\0') {
    register0x0000003c = (BADSPACEBASE *)&uStack_a;
    uStack_a = 0x1a828;
    iStack_6 = iVar7;
  }
  *(int *)((int)register0x0000003c + -4) = iVar7;
  *(undefined4 *)((int)register0x0000003c + -8) = 0x1a84c;
  uVar1 = candidate_3d244();
  uVar2 = CONCAT22((short)((uint)uVar1 >> 0x10),(short)uVar1 + -0x40);
  pbVar6 = *(byte **)((int)register0x0000003c + -4);
  uVar5 = 2;
  pcVar8 = candidate_3cf9a;
  cVar4 = '\0';
  do {
    bVar3 = *pbVar6;
    if ((uVar5 & 1) == 0) {
      bVar3 = bVar3 >> 4;
    }
    else {
      pbVar6 = pbVar6 + 1;
    }
    if (((bVar3 & 0xf) != 0) || (uVar5 == 1)) {
      cVar4 = -1;
    }
    if (cVar4 != '\0') {
      *(byte **)((int)register0x0000003c + -4) = pbVar6;
      *(undefined4 *)((int)register0x0000003c + -8) = 0x3d046;
      uVar2 = (*pcVar8)();
      pbVar6 = *(byte **)((int)register0x0000003c + -4);
      pcVar8 = extraout_A1;
    }
    uVar5 = uVar5 - 1;
  } while (uVar5 != 0);
  return uVar2 << 0x10 | uVar2 >> 0x10;
}
