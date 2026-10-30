/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0243e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_243e2(void)

{
  int iVar1;
  short sVar2;
  undefined2 uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  iVar1 = unaff_A6[3];
  *(byte *)(iVar1 + 0x8d) = *(byte *)(iVar1 + 0x8d) | 1;
  *(undefined1 *)((int)unaff_A6 + 0x8a) = *(undefined1 *)(iVar1 + 0x8a);
  if (*(char *)(iVar1 + 0x8a) == '\0') {
    uVar3 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x26) = uVar3;
  }
  else if (*(char *)(iVar1 + 0x8a) == '\x01') {
    uVar3 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x26) = uVar3;
  }
  else {
    uVar3 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x26) = uVar3;
  }
  if (*(short *)(unaff_A6 + 0x26) == 0) {
    *(undefined1 *)(unaff_A6[3] + 0x8e) = 0;
    if ((*(byte *)(unaff_A6[3] + 0x8d) & 1) == 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *(undefined1 *)(unaff_A6 + 0x23) = 0;
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
    unaff_A6[0x14] = 0;
    *unaff_A6 = 0x242d2;
    candidate_1abc8();
    candidate_427a();
    iVar1 = unaff_A6[3];
    uVar4 = false;
    if (*(char *)(iVar1 + 0x8e) != '\0') {
      if (*(char *)((int)unaff_A6 + 0x8b) == *(char *)(iVar1 + 0x8b)) {
        if ((*(byte *)(iVar1 + 0x8d) & 1) == 0) {
          if ((*(byte *)(iVar1 + 0x8d) & 2) != 0) {
            *unaff_A6 = candidate_24358;
          }
        }
        else {
          *unaff_A6 = candidate_243e2;
        }
      }
      uVar4 = *(byte *)(iVar1 + 0x8b) < 8;
      if ((*(byte *)(iVar1 + 0x8b) == 8) && ((*(byte *)(iVar1 + 0x8d) & 1) != 0)) {
        *unaff_A6 = candidate_2458c;
        uVar4 = false;
      }
    }
    candidate_4c24();
    if ((bool)uVar4) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar4 = 0;
    }
    candidate_24cca();
    candidate_24ab0();
    if ((bool)uVar4) {
      unaff_A6[0x14] = 0;
    }
    return;
  }
  candidate_4256();
  iVar1 = unaff_A6[3];
  *(undefined4 *)((int)unaff_A6 + 0x72) = *(undefined4 *)(iVar1 + 0x72);
  if (*(char *)(iVar1 + 0x8a) == '\0') {
    uVar3 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x92) = uVar3;
  }
  else if (*(char *)(iVar1 + 0x8a) == '\x01') {
    uVar3 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x92) = uVar3;
  }
  else {
    uVar3 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x92) = uVar3;
  }
  *unaff_A6 = 0x244b8;
  uVar4 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar4) {
    candidate_4256();
    candidate_24b62();
  }
  if (*(short *)((int)unaff_A6 + 0x92) == 0) {
    if ((*(byte *)(unaff_A6 + 0x27) & 1) == 0) {
      if (*(short *)(unaff_A6 + 0xf) < 0x30) {
        *(undefined1 *)(unaff_A6[3] + 0x8e) = 0x7f;
      }
      else {
        *unaff_A6 = candidate_24548;
      }
    }
    uVar4 = false;
  }
  else {
    sVar2 = *(short *)((int)unaff_A6 + 0x92);
    uVar4 = sVar2 == 0;
    *(short *)((int)unaff_A6 + 0x92) = sVar2 + -1;
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
  }
  candidate_24cca();
  candidate_24ab0();
  if (((bool)uVar4) && (unaff_A6[0x14] = 0, (*(byte *)(unaff_A6 + 0x27) & 1) == 0)) {
    *unaff_A6 = candidate_2458c;
  }
  return;
}
