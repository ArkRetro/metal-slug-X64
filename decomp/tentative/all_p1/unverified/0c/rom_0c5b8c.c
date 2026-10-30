/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c5b8c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c5b8c(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  if (*(char *)((int)unaff_A6 + 0x97) == '\x02') {
    unaff_A6[0x1e] = 0x8000;
  }
  else {
    unaff_A6[0x1e] = 0xffff8000;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    unaff_A6[0x1e] = -unaff_A6[0x1e];
  }
  *(undefined2 *)(unaff_A6 + 0x23) = 0;
  *(undefined2 *)(unaff_A6 + 0x24) = 0x28;
  candidate_12192();
  candidate_4256();
  *unaff_A6 = 0xc5be2;
  candidate_c6b02();
  candidate_c6a02();
  candidate_427a();
  candidate_c699c();
  uVar2 = false;
  if ((*(short *)((int)unaff_A6 + 0x72) != 0) &&
     (uVar2 = *(byte *)((int)unaff_A6 + 0x97) < 4, *(byte *)((int)unaff_A6 + 0x97) != 4)) {
    sVar1 = *(short *)(unaff_A6 + 0x24);
    uVar2 = sVar1 == 0;
    sVar1 = sVar1 + -1;
    *(short *)(unaff_A6 + 0x24) = sVar1;
    if (sVar1 == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x97) = 3;
      *unaff_A6 = candidate_c5c60;
      uVar2 = false;
    }
  }
  candidate_4c24();
  if ((bool)uVar2) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_c5d26;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_c5dcc;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_c66e8;
  }
  return;
}
