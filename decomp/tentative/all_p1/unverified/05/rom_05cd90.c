/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05cd90.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5cd90(void)

{
  short sVar1;
  short sVar2;
  char cVar3;
  byte bVar4;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar5;
  undefined1 uVar6;

  if ((*(byte *)((int)unaff_A6 + 0x93) & 8) != 0) {
    candidate_1ae00();
    if (((bool)in_CF) &&
       (*(short *)(*(int *)((int)unaff_A6 + 0x6e) + 0x40) == *(short *)(unaff_A6 + 0x10))) {
      unaff_A6[0x14] = 0x5e93c;
      candidate_4256();
      *unaff_A6 = candidate_5d11e;
      uVar6 = 0;
      candidate_5e234();
      candidate_427a();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_5d144;
        uVar6 = 0;
      }
      candidate_5e244();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_5cd08;
      }
      candidate_5e6d2();
      sVar1 = *(short *)(unaff_A6 + 0x1f) + -1;
      *(short *)(unaff_A6 + 0x1f) = sVar1;
      if (sVar1 < 0) {
        *(undefined2 *)(unaff_A6 + 0x1f) = 0;
      }
      candidate_4c24();
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      }
      else {
        *unaff_A6 = candidate_5db92;
      }
      bVar5 = false;
      candidate_1945c();
      if (bVar5) {
        *unaff_A6 = candidate_5dc7a;
      }
      return;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  unaff_A6[0xc] = 0;
  cVar3 = candidate_1b12e();
  *(char *)((int)unaff_A6 + 0x8a) = *(char *)((int)unaff_A6 + 0x8b) + cVar3;
  bVar5 = false;
  candidate_5e784();
  if (bVar5) {
    bVar4 = *(byte *)(unaff_A6 + 0x23);
  }
  else {
    if (*(short *)(extraout_A0 + 0x3c) < *(short *)(unaff_A6 + 0xf)) {
      uVar6 = 0x80;
    }
    else {
      uVar6 = 0;
    }
    *(undefined1 *)((int)unaff_A6 + 0x8a) = uVar6;
    bVar4 = 0x80;
  }
  bVar4 = (bVar4 & *(byte *)((int)unaff_A6 + 0x8a)) >> 4;
  *(byte *)((int)unaff_A6 + 0x8a) = bVar4;
  *(byte *)((int)unaff_A6 + 0x89) = bVar4;
  if (bVar4 < 9) {
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xfd;
    candidate_4256();
  }
  else {
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 2;
    candidate_4256();
  }
  *unaff_A6 = 0x5ce54;
  uVar6 = 0;
  candidate_5e234();
  candidate_427a();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_5ce7a;
    uVar6 = 0;
  }
  candidate_5e244();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_5cd08;
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f);
  sVar2 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar2;
  if (-1 >= sVar2) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar5 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar5 = bVar5 && (-1 < sVar2 && sVar1 == 0);
  candidate_5e7a8();
  if (bVar5) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar5 = false;
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
