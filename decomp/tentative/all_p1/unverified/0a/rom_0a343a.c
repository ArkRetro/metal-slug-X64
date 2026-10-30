/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a343a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a343a(void)

{
  undefined1 uVar2;
  undefined2 uVar1;
  undefined4 *unaff_A6;

  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x8b) = uVar2;
  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x22) = uVar1;
  if ((*(byte *)((int)unaff_A6 + 0x96) & 0x10) == 0) {
    if (*(int *)((int)unaff_A6 + 0x76) != 0) {
      candidate_a3ef0();
    }
    candidate_a3ef0();
    candidate_4256();
  }
  else {
    if (*(int *)((int)unaff_A6 + 0x76) != 0) {
      candidate_a3ef0();
    }
    candidate_a3ef0();
    candidate_4256();
  }
  *unaff_A6 = 0xa34de;
  uVar2 = 0;
  candidate_a3f52();
  candidate_427a();
  if (((bool)uVar2) && (*(char *)((int)unaff_A6 + 0x8b) < '\x01')) {
    *unaff_A6 = candidate_a352c;
  }
  if ((('\0' < *(char *)((int)unaff_A6 + 0x8b)) &&
      (*(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1,
      *(short *)(unaff_A6 + 0x22) < 1)) &&
     (*(char *)((int)unaff_A6 + 0x8b) = *(char *)((int)unaff_A6 + 0x8b) + -1,
     '\0' < *(char *)((int)unaff_A6 + 0x8b))) {
    *unaff_A6 = 0xa344a;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_a30b2;
  }
  uVar2 = false;
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a36ac;
    uVar2 = false;
    candidate_1b206();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_a36f4;
      uVar2 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_a378a;
      uVar2 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = 0xa385c;
  }
  return;
}
