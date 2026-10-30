/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dc5ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_dc5ee(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined8 uVar1;

  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    candidate_40bc();
    uVar1 = candidate_1acf2();
    if ((_DAT_0010000e & 1) != 0) {
      candidate_40bc(uVar1);
      *(undefined1 *)(extraout_A0 + 0x5b) = *(undefined1 *)((int)unaff_A6 + 0x5b);
      candidate_1acf2();
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_1da40();
      *unaff_A6 = candidate_dc682;
    }
  }
  candidate_dc79c();
  return;
}
