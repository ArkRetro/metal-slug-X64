/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a30b2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a30b2(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  candidate_1b1dc();
  if (*(int *)((int)unaff_A6 + 0x76) != 0) {
    candidate_a3ef0();
  }
  candidate_a3ef0();
  candidate_4256();
  *unaff_A6 = 0xa30f6;
  uVar1 = 0;
  candidate_143dc();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = 0xa322a;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_a35e8;
    bVar2 = false;
    candidate_1b206();
    if (bVar2) {
      *unaff_A6 = candidate_a3624;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a36ac;
    uVar1 = false;
    candidate_1b206();
    if ((bool)uVar1) {
      *unaff_A6 = candidate_a36f4;
      uVar1 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_a378a;
      uVar1 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = 0xa385c;
  }
  return;
}
