/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e502.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e502(void)

{
  int iVar1;
  ushort uVar2;
  short sVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
  uVar4 = *(ushort *)(unaff_A6 + 10) < 0x148;
  if (((short)*(ushort *)(unaff_A6 + 10) < 0x149) ||
     (uVar4 = *(ushort *)(unaff_A6 + 10) < 0x1b0, 0x1af < (short)*(ushort *)(unaff_A6 + 10))) {
    candidate_1b1dc();
    candidate_8f4a0();
    if ((bool)uVar4) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = 0x8e69c;
    uVar4 = 0;
    candidate_8f4ce();
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_8e50c;
      bVar5 = false;
      candidate_8f8e0();
      if (bVar5) {
        *unaff_A6 = candidate_8e6c2;
      }
    }
  }
  else {
    iVar1 = *(int *)((int)unaff_A6 + 0x72);
    if ((*(byte *)((int)unaff_A6 + 0x93) & 0x80) == 0) {
      iVar1 = -iVar1;
    }
    *(int *)((int)unaff_A6 + 0x62) = iVar1;
    uVar2 = candidate_29c6();
    *(ushort *)((int)unaff_A6 + 0x7e) = (uVar2 & 0xf) + 0xf;
    bVar5 = false;
    candidate_8f4a0();
    if (bVar5) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = 0x8e5fa;
    uVar4 = false;
    candidate_8f51c();
    if ((bool)uVar4) {
      *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) ^ 0x80;
      *unaff_A6 = candidate_8e650;
      uVar4 = 0;
    }
    candidate_427a();
    candidate_8f8fa();
    if (!(bool)uVar4) {
      *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
    }
    if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
      *unaff_A6 = candidate_8e650;
    }
    uVar4 = false;
    candidate_8f90a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_8e650;
      uVar4 = 0;
    }
    candidate_8f8e0();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_8e650;
    }
  }
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0xf) < 0x60) {
    sVar3 = 3;
  }
  else if (*(short *)(unaff_A6 + 0xf) < 0xc1) {
    sVar3 = 4;
  }
  else {
    sVar3 = 5;
  }
  uVar4 = (undefined1)sVar3;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar4 = *(undefined1 *)(sVar3 + 0x8fee0);
  }
  *(undefined1 *)((int)unaff_A6 + 0x8a) = uVar4;
  uVar4 = false;
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xbf;
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_8e0d4;
    uVar4 = 0;
  }
  candidate_8f432();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_8e128;
  }
  return;
}
