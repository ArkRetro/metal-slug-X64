/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04fade.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4fade(void)

{
  short sVar1;
  ushort uVar2;
  int extraout_A0;
  int unaff_A6;
  bool bVar3;

  bVar3 = false;
  if (*(char *)(unaff_A6 + 0x61) == '\0') {
    if ((*(byte *)(unaff_A6 + 0x8e) & 1) == 0) {
      candidate_1af9a();
      if (bVar3) {
        sVar1 = 0xa0;
      }
      else if (*(short *)(extraout_A0 + 0x3c) < 0xc1) {
        sVar1 = *(short *)(extraout_A0 + 0x3c);
      }
      else {
        sVar1 = 0xc0;
      }
      uVar2 = candidate_29c6();
      sVar1 = sVar1 + ((uVar2 & 7) - 4);
    }
    else {
      *(undefined2 *)(unaff_A6 + 0x6e) = 0x20;
      *(undefined2 *)(unaff_A6 + 0x70) = 0xc0;
      if (*(short *)(unaff_A6 + 0x86) == 0) {
        uVar2 = *(short *)(unaff_A6 + 0x82) - *(short *)(unaff_A6 + 0x3c);
        if ((int)((uint)uVar2 << 0x10) < 0) {
          uVar2 = -uVar2;
        }
        if ((short)uVar2 < 6) {
          sVar1 = candidate_29f2();
          sVar1 = sVar1 + 0x10;
          if (0x9f < *(short *)(unaff_A6 + 0x82)) {
            sVar1 = -sVar1;
          }
          *(short *)(unaff_A6 + 0x82) = sVar1 + 0xa0;
          *(undefined2 *)(unaff_A6 + 0x86) = 0x10;
        }
      }
      else {
        *(short *)(unaff_A6 + 0x86) = *(short *)(unaff_A6 + 0x86) + -1;
      }
      sVar1 = *(short *)(unaff_A6 + 0x82);
    }
    if (sVar1 < *(short *)(unaff_A6 + 0x3c)) {
      if (*(short *)(unaff_A6 + 0x3c) < *(short *)(unaff_A6 + 0x6e)) {
        *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) + 0x2000;
        if (-1 < *(int *)(unaff_A6 + 0x30)) {
          *(undefined4 *)(unaff_A6 + 0x30) = 0;
        }
      }
      else {
        *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) + -0x2000;
        if (*(int *)(unaff_A6 + 0x30) < -0x2ffff) {
          *(undefined4 *)(unaff_A6 + 0x30) = 0xfffd0000;
        }
      }
    }
    else if (*(short *)(unaff_A6 + 0x70) < *(short *)(unaff_A6 + 0x3c)) {
      *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) + -0x2000;
      if (*(int *)(unaff_A6 + 0x30) < 1) {
        *(undefined4 *)(unaff_A6 + 0x30) = 0;
      }
    }
    else {
      *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) + 0x2000;
      if (0x2ffff < *(int *)(unaff_A6 + 0x30)) {
        *(undefined4 *)(unaff_A6 + 0x30) = 0x30000;
      }
    }
    return;
  }
  if (*(short *)(unaff_A6 + 0x3c) < 0x90) {
    *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) + 0x2000;
    if (0x60000 < *(int *)(unaff_A6 + 0x30)) {
      *(undefined4 *)(unaff_A6 + 0x30) = 0x60000;
    }
  }
  else if (*(int *)(unaff_A6 + 0x30) < 0x30001) {
    *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) + 0x2000;
  }
  else {
    *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) + -0x4000;
  }
  return;
}
