/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x029ece.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_29ece(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0x29ee4;
  uVar1 = 0;
  candidate_2a472();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_29f72;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(byte *)((int)unaff_A6 + 0x86) = *(byte *)((int)unaff_A6 + 0x86) | 8;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_2a04c;
    uVar1 = 0;
  }
  if (((*(byte *)((int)unaff_A6 + 0x86) & 0x80) != 0) &&
     ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) {
    *unaff_A6 = candidate_2a0b2;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
