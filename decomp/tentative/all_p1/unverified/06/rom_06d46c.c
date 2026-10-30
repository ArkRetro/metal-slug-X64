/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06d46c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6d46c(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  undefined8 uVar2;

  candidate_4256();
  uVar2 = candidate_3b332();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar2;
  *unaff_A6 = 0x6d49c;
  candidate_143dc();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0xd) < 0) {
    *unaff_A6 = candidate_6d4ca;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_6d500;
  }
  uVar1 = false;
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar1 = 0;
    }
    else {
      *unaff_A6 = candidate_6d57a;
      uVar1 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_6d7c2;
  }
  return;
}
