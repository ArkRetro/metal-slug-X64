/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x099e1c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_99e1c(void)

{
  short sVar1;
  undefined2 uVar3;
  undefined4 uVar2;
  undefined1 uVar4;
  undefined4 *unaff_A6;
  undefined1 uVar5;
  uint6 uVar6;

  candidate_1b1dc();
  candidate_9ac16();
  candidate_9f88();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  unaff_A6[0x14] = 0x9ad4c;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    uVar3 = 0xf;
  }
  else {
    if ((char)*(short *)((int)unaff_A6 + 0x6e) == '\x01') {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
    uVar3 = 0x7fff;
  }
  *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar3;
  candidate_4256();
  uVar2 = 0x18000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar2 = 0xfffe8000;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = uVar2;
  *unaff_A6 = 0x9a06e;
  if ((*(byte *)((int)unaff_A6 + 0x86) & 1) == 0) {
    candidate_9ac86();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_9a0f6;
    }
    else {
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
        *unaff_A6 = candidate_9a142;
      }
      if ((*(char *)((int)unaff_A6 + 0x6e) == '\0') &&
         (sVar1 = *(short *)((int)unaff_A6 + 0x7a), *(short *)((int)unaff_A6 + 0x7a) = sVar1 + -1,
         sVar1 == 0)) {
        *unaff_A6 = candidate_9a142;
      }
    }
  }
  else {
    candidate_1436a();
  }
  uVar6 = candidate_40200();
  if (-1 < (int6)uVar6) {
    *(char *)(unaff_A6 + 0x21) = (char)(uVar6 >> 0x20);
    if ((uVar6 & 1) != 0) {
      *unaff_A6 = candidate_9a396;
    }
    if ((uVar6 & 2) != 0) {
      *unaff_A6 = candidate_9a4fa;
    }
  }
  uVar5 = 0;
  candidate_427a();
  uVar4 = candidate_9ab9a();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x83) = uVar4;
    *unaff_A6 = candidate_9a17e;
    uVar5 = 0;
  }
  candidate_c8bec();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
