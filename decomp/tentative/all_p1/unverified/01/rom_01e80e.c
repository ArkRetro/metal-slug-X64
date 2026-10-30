/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01e80e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1e80e(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(undefined1 *)((int)unaff_A6 + 0x86) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 4;
  candidate_1abc8();
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  *(undefined1 *)(iVar1 + 0x5e) = *(undefined1 *)((int)unaff_A6 + 0x5e);
  *(undefined1 *)(iVar1 + 0x88) = *(undefined1 *)(unaff_A6 + 0x22);
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0x100000;
  if (*(char *)(unaff_A6 + 0x22) == '\x01') {
    *(undefined2 *)(unaff_A6 + 0x1f) = 4;
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0x18;
    candidate_120f8();
  }
  else {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
    candidate_120f8();
  }
  *unaff_A6 = 0x1e894;
  candidate_1abc8();
  *(short *)(unaff_A6 + 0x1f) = *(short *)(unaff_A6 + 0x1f) + -1;
  if (*(short *)(unaff_A6 + 0x1f) < 1) {
    if (*(char *)(unaff_A6 + 0x22) == '\x01') {
      *(undefined2 *)(unaff_A6 + 0x1f) = 3;
    }
    else {
      *(undefined2 *)(unaff_A6 + 0x1f) = 2;
    }
    *(char *)((int)unaff_A6 + 0x86) = *(char *)((int)unaff_A6 + 0x86) + '\x01';
    bVar2 = false;
    candidate_40bc();
    iVar1 = extraout_A0_01;
    if (bVar2) {
      candidate_1b220();
      iVar1 = extraout_A0_02;
    }
    *(undefined1 *)(iVar1 + 0x5e) = *(undefined1 *)((int)unaff_A6 + 0x5e);
    *(undefined4 *)(iVar1 + 0x62) = *(undefined4 *)((int)unaff_A6 + 0x62);
    *(undefined1 *)(iVar1 + 0x86) = *(undefined1 *)((int)unaff_A6 + 0x86);
    *(undefined1 *)(iVar1 + 0x88) = *(undefined1 *)(unaff_A6 + 0x22);
    bVar2 = false;
    candidate_40bc();
    iVar1 = extraout_A0_03;
    if (bVar2) {
      candidate_1b220();
      iVar1 = extraout_A0_04;
    }
    *(undefined1 *)(iVar1 + 0x5e) = *(undefined1 *)((int)unaff_A6 + 0x5e);
    *(int *)(iVar1 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
    *(undefined1 *)(iVar1 + 0x86) = *(undefined1 *)((int)unaff_A6 + 0x86);
    *(undefined1 *)(iVar1 + 0x88) = *(undefined1 *)(unaff_A6 + 0x22);
    if ('\x03' < *(char *)((int)unaff_A6 + 0x86)) {
      *unaff_A6 = candidate_1e97a;
    }
  }
  if (0x13f < *(ushort *)(unaff_A6 + 0xf)) {
    *unaff_A6 = candidate_1e97a;
  }
  if (0x13f < (ushort)(*(short *)(unaff_A6 + 0x10) - 0x100U)) {
    *unaff_A6 = candidate_1e97a;
  }
  if ('\0' < *(char *)((int)unaff_A6 + 0x87)) {
    *(char *)((int)unaff_A6 + 0x87) = *(char *)((int)unaff_A6 + 0x87) + -1;
  }
  return;
}
