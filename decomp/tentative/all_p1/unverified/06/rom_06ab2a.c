/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06ab2a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6ab2a(void)

{
  char cVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  sVar2 = candidate_260c8();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    sVar2 = -sVar2;
  }
  *(int *)((int)unaff_A6 + 0x62) = (int)sVar2 << 8;
  candidate_4256();
  candidate_6b416();
  candidate_6b8d2();
  *unaff_A6 = 0x6ab7e;
  uVar4 = 0;
  candidate_6b7e0();
  candidate_427a();
  candidate_6bab8();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_6abe2;
    uVar4 = 0;
  }
  candidate_6b3ec();
  if (!(bool)uVar4) {
    uVar4 = false;
    if (*(char *)(unaff_A6 + 0x24) == '\0') {
      candidate_6bb2e();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6abe2;
        uVar4 = 0;
      }
      candidate_6b916();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6a9e8;
        uVar4 = 0;
      }
    }
    candidate_6b9e0();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_6ac14;
      uVar4 = 0;
    }
  }
  candidate_6bace();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_6a9e8;
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
