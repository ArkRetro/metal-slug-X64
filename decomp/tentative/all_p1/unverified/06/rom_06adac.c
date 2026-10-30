/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06adac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6adac(void)

{
  char cVar1;
  short sVar3;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  if (*(short *)((int)unaff_A6 + 0x86) == 0) {
    if (*(char *)(unaff_A6 + 0x1b) == '\f') {
      candidate_4256();
    }
    else if ((*(byte *)((int)unaff_A6 + 0x92) & 0x10) == 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    candidate_1b1dc();
    candidate_6b8d2();
    *unaff_A6 = 0x6ae12;
    uVar4 = 0;
    candidate_6b7e0();
    candidate_427a();
    candidate_6b3ec();
    if ((!(bool)uVar4) && (uVar4 = false, *(char *)(unaff_A6 + 0x24) == '\0')) {
      candidate_6bb2e();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6a924;
        uVar4 = 0;
      }
      candidate_6b982();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6a992;
        uVar4 = 0;
      }
      candidate_6b9e0();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6ac28;
        uVar4 = 0;
      }
      candidate_6bbca();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6a7de;
      }
    }
  }
  else {
    sVar3 = candidate_260c8();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      sVar3 = -sVar3;
    }
    iVar2 = (int)sVar3 << 8;
    if ((*(byte *)((int)unaff_A6 + 0x92) & 1) != 0) {
      iVar2 = iVar2 >> 1;
    }
    *(int *)((int)unaff_A6 + 0x62) = iVar2;
    if (*(char *)(unaff_A6 + 0x1b) == '\f') {
      candidate_4256();
    }
    else if ((*(byte *)((int)unaff_A6 + 0x92) & 0x10) == 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    candidate_6b416();
    *unaff_A6 = 0x6aefe;
    uVar4 = 0;
    candidate_6b7e0();
    candidate_427a();
    candidate_6bb8a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_6adac;
      uVar4 = 0;
    }
    candidate_6bab8();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_6a924;
      uVar4 = 0;
    }
    candidate_6bace();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_6ad3e;
      uVar4 = 0;
    }
    candidate_6baf8();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_6ad7a;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_6accc;
  }
  if (*(short *)((int)unaff_A6 + 0x82) != 0) {
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  }
  if (*(char *)(unaff_A6 + 0x24) != '\0') {
    *(char *)(unaff_A6 + 0x24) = *(char *)(unaff_A6 + 0x24) + -1;
  }
  uVar4 = false;
  if (*(char *)((int)unaff_A6 + 0x91) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x91);
    uVar4 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x91) = cVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_6af44;
    uVar4 = 0;
  }
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = 0x6b27e;
  }
  return;
}
