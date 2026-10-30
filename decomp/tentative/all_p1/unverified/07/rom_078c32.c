/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078c32.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78c32(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1af9a();
  if (*(short *)(unaff_A6 + 0xf) < *(short *)(extraout_A0 + 0x3c)) {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
LAB_00078ca8:
      candidate_4256();
      *unaff_A6 = 0x78cbe;
      uVar1 = 0;
      candidate_78cf4();
      candidate_427a();
      if ((bool)uVar1) {
        *unaff_A6 = candidate_78bd2;
        uVar1 = 0;
      }
      candidate_78fa2();
      candidate_78d54();
      if ((bool)uVar1) {
        *unaff_A6 = 0x77e5c;
        uVar1 = 0;
      }
      candidate_78d7a();
      if ((bool)uVar1) {
        *unaff_A6 = 0x77e5c;
      }
      return;
    }
  }
  else if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) goto LAB_00078ca8;
  candidate_4256();
  *unaff_A6 = 0x78c72;
  uVar1 = 0;
  candidate_78cf4();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = 0x78ca8;
    uVar1 = 0;
  }
  candidate_78fa2();
  candidate_78d54();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
    uVar1 = 0;
  }
  candidate_78d7a();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
