/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06a43e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6a43e(void)

{
  char cVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 in_CF;
  undefined1 uVar4;

  candidate_6b778();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_6a2e8;
  }
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x91) == '\0') {
    *unaff_A6 = candidate_6a2e8;
  }
  uVar4 = false;
  candidate_6b3ec();
  if ((!(bool)uVar4) && (uVar4 = 0, *(char *)(unaff_A6 + 0x24) == '\0')) {
    uVar4 = false;
    if (*(char *)((int)unaff_A6 + 0x91) == '\0') {
      candidate_6bb2e();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6a924;
        uVar4 = 0;
      }
      else {
        candidate_6ba52();
        if ((bool)uVar4) {
          *unaff_A6 = candidate_6a4f0;
          uVar4 = 0;
        }
        candidate_6ba20();
        if ((bool)uVar4) {
          *unaff_A6 = candidate_6a6b8;
          uVar4 = 0;
        }
      }
    }
    candidate_6b982();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_6a992;
      uVar4 = 0;
    }
    candidate_6b946();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_6aabc;
      uVar4 = 0;
    }
    candidate_6b9e0();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_6ac28;
      uVar4 = 0;
    }
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
  if ((*(byte *)((int)unaff_A6 + 0x92) & 2) != 0) {
    bVar3 = *(short *)(unaff_A6 + 0x20) == 0;
    if (!bVar3) {
      sVar2 = *(short *)(unaff_A6 + 0x20) + -1;
      *(short *)(unaff_A6 + 0x20) = sVar2;
      bVar3 = sVar2 == 0;
    }
    if (bVar3) {
      *unaff_A6 = candidate_6a7de;
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
