/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0678dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_678dc(void)

{
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_1436a();
  candidate_1af9a();
  *(undefined4 *)((int)unaff_A6 + 0x86) = extraout_A0;
  bVar1 = false;
  candidate_68b4c();
  if (bVar1) {
    candidate_68b7e();
    if (bVar1) {
      candidate_6905a();
      if (*(short *)(unaff_A6 + 0xf) < 1) {
        *unaff_A6 = 0x67ab4;
        return;
      }
      if (0x15f < *(short *)(unaff_A6 + 0xf)) {
        *unaff_A6 = 0x67ab4;
        return;
      }
      if ((*(ushort *)((int)unaff_A6 + 0x6e) & 0x80) == 0) {
        *unaff_A6 = candidate_679b0;
      }
      else {
        *unaff_A6 = candidate_67a6c;
      }
      return;
    }
  }
  candidate_688f4();
  return;
}
