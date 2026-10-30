/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078dc8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78dc8(void)

{
  short sVar1;
  int iVar2;
  char cVar3;
  int extraout_A0;
  int unaff_A6;
  bool bVar4;

  if (*(char *)(unaff_A6 + 0x9d) == '\0') {
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x40;
    cVar3 = '\0';
    sVar1 = *(short *)(unaff_A6 + 0x3c) + -0xa0;
    if (sVar1 < 0) {
      sVar1 = -sVar1;
      cVar3 = -1;
    }
    if (0x30 < sVar1) {
      if (cVar3 == '\0') {
        iVar2 = *(int *)(unaff_A6 + 0x30) + -0x200;
        if (iVar2 < -0x12000) {
          iVar2 = -0x12000;
        }
        *(int *)(unaff_A6 + 0x30) = iVar2;
        *(undefined2 *)(unaff_A6 + 0x7a) = 0x18;
        *(undefined1 *)(unaff_A6 + 0x9a) = 1;
      }
      else {
        iVar2 = *(int *)(unaff_A6 + 0x30) + 0x200;
        if (0x12000 < iVar2) {
          iVar2 = 0x12000;
        }
        *(int *)(unaff_A6 + 0x30) = iVar2;
        *(undefined2 *)(unaff_A6 + 0x7a) = 0x30;
        *(undefined1 *)(unaff_A6 + 0x9a) = 2;
      }
      return;
    }
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0xbf;
    *(undefined1 *)(unaff_A6 + 0x9d) = 0xff;
  }
  bVar4 = false;
  candidate_1af9a();
  if (!bVar4) {
    sVar1 = *(short *)(unaff_A6 + 0x3c) - *(short *)(extraout_A0 + 0x3c);
    if (sVar1 < 0x91) {
      if ((sVar1 < 0x60) && (*(short *)(unaff_A6 + 0x3c) < *(short *)(unaff_A6 + 0x70))) {
        iVar2 = *(int *)(unaff_A6 + 0x30) + 0x200;
        if (0x12000 < iVar2) {
          iVar2 = 0x12000;
        }
        *(int *)(unaff_A6 + 0x30) = iVar2;
        *(undefined2 *)(unaff_A6 + 0x7a) = 0x30;
        *(undefined1 *)(unaff_A6 + 0x9a) = 2;
        return;
      }
    }
    else if (*(short *)(unaff_A6 + 0x6e) < *(short *)(unaff_A6 + 0x3c)) {
      iVar2 = *(int *)(unaff_A6 + 0x30) + -0x200;
      if (iVar2 < -0x12000) {
        iVar2 = -0x12000;
      }
      *(int *)(unaff_A6 + 0x30) = iVar2;
      *(undefined2 *)(unaff_A6 + 0x7a) = 0x18;
      *(undefined1 *)(unaff_A6 + 0x9a) = 1;
      return;
    }
  }
  *(undefined2 *)(unaff_A6 + 0x7a) = 0x24;
  iVar2 = *(int *)(unaff_A6 + 0x30);
  if (iVar2 < 0) {
    *(undefined1 *)(unaff_A6 + 0x9a) = 1;
    iVar2 = iVar2 + 0x400;
    if (-1 < iVar2) {
LAB_00078f30:
      candidate_1b1dc();
      *(undefined1 *)(unaff_A6 + 0x9a) = 0;
      return;
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x9a) = 2;
    iVar2 = iVar2 + -0x400;
    if (iVar2 < 0) goto LAB_00078f30;
  }
  *(int *)(unaff_A6 + 0x30) = iVar2;
  return;
}
