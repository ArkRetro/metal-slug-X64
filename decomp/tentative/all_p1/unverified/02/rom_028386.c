/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x028386.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_28386(void)

{
  undefined1 uVar2;
  undefined2 uVar1;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x7a) = uVar2;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar1;
  if ((*(char *)((int)unaff_A6 + 0x7a) < '\x02') || (0x3c < *(short *)((int)unaff_A6 + 0x76))) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  candidate_1b1dc();
  *unaff_A6 = 0x283f0;
  uVar2 = 0;
  candidate_1436a();
  candidate_427a();
  if (((bool)uVar2) && (*(char *)((int)unaff_A6 + 0x7a) < '\x02')) {
    *unaff_A6 = 0x28118;
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x76) = uVar1;
  }
  if ((('\0' < *(char *)((int)unaff_A6 + 0x7a)) &&
      (*(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1,
      *(short *)((int)unaff_A6 + 0x76) < 1)) &&
     (*(char *)((int)unaff_A6 + 0x7a) = *(char *)((int)unaff_A6 + 0x7a) + -1,
     '\0' < *(char *)((int)unaff_A6 + 0x7a))) {
    *unaff_A6 = 0x2839c;
  }
  bVar3 = false;
  candidate_4c24();
  if (bVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_284f2;
    bVar3 = false;
    candidate_1b206();
    if (bVar3) {
      *unaff_A6 = candidate_2858c;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar3 = false;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
