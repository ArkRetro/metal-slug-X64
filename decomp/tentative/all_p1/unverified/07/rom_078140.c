/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078140.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78140(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  if (*(short *)((int)unaff_A6 + 0x8a) != 0) {
    candidate_9f88();
    candidate_9ea8();
    candidate_1b1dc();
    candidate_4256();
    *unaff_A6 = 0x7810a;
    uVar2 = 0;
    candidate_78cf4();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = 0x78148;
      uVar2 = 0;
    }
    candidate_78fa2();
    candidate_78d54();
    if ((bool)uVar2) {
      *unaff_A6 = 0x77e5c;
      uVar2 = 0;
    }
    candidate_78d7a();
    if ((bool)uVar2) {
      *unaff_A6 = 0x77e5c;
    }
    return;
  }
  candidate_9f88();
  candidate_9ea8();
  candidate_1b1dc();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffe0000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
  }
  candidate_4256();
  *unaff_A6 = 0x7818e;
  candidate_78cf4();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    if ((short)(*(short *)(unaff_A6 + 0xf) - *(short *)(unaff_A6[3] + 0x3c)) <=
        *(short *)(unaff_A6 + 0x24)) {
      *unaff_A6 = candidate_780da;
    }
  }
  else if (*(short *)((int)unaff_A6 + 0x92) <=
           (short)(*(short *)(unaff_A6 + 0xf) - *(short *)(unaff_A6[3] + 0x3c))) {
    *unaff_A6 = candidate_780da;
  }
  if (*(short *)(unaff_A6 + 0x23) == 0) {
    *unaff_A6 = candidate_78212;
    uVar2 = false;
  }
  else {
    sVar1 = *(short *)(unaff_A6 + 0x23);
    uVar2 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x23) = sVar1 + -1;
  }
  candidate_78fa2();
  candidate_78d54();
  if ((bool)uVar2) {
    *unaff_A6 = 0x77e5c;
    uVar2 = 0;
  }
  candidate_78d7a();
  if ((bool)uVar2) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
