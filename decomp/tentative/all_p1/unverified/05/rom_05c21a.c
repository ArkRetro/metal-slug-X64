/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05c21a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_5c21a(void)

{
  int in_D0;
  int iVar1;
  int extraout_D1;
  int iVar2;
  undefined2 uVar3;
  short sVar4;
  short sVar5;
  uint unaff_D6;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x84) & 2) == 0) {
    uVar3 = 0x2c;
    sVar4 = 0x178;
    sVar5 = 0x168;
  }
  else {
    uVar3 = 0x10;
    sVar4 = 0x150;
    sVar5 = 0x140;
  }
  if (*(char *)(unaff_A6 + 0x83) != '\0') {
    if (*(char *)(unaff_A6 + 0x83) != '\x01') {
      sVar4 = sVar5;
    }
    if (sVar4 <= *(short *)(unaff_A6 + 0x40)) {
      return in_D0;
    }
    return in_D0;
  }
  *(undefined2 *)(unaff_A6 + 0x7a) = uVar3;
  candidate_128da();
  *(undefined1 *)(unaff_A5 + 0x4009) = 0;
  *(undefined4 *)(unaff_A5 + 0x4004) = 0;
  *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
  iVar1 = *(int *)(unaff_A6 + 0x2c);
  if (*(int *)(unaff_A6 + 0x38) != 0) {
    iVar1 = *(int *)(*(int *)(unaff_A6 + 0x38) + 0x18) + iVar1;
  }
  candidate_13968();
  if ((unaff_D6 & 1) != 0) {
    iVar2 = extraout_D1;
    if (extraout_A0 != 0) {
      iVar2 = *(int *)(extraout_A0 + 0x18) + extraout_D1;
    }
    return CONCAT22((short)((uint)iVar1 >> 0x10),(short)((uint)(iVar2 - iVar1) >> 0x10));
  }
  return iVar1;
}
