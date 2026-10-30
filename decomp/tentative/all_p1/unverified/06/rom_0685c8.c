/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0685c8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_685c8(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1436a();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
      candidate_13f1e();
      candidate_1da40();
      candidate_1da40();
      candidate_40bc();
      candidate_1acf2();
      candidate_120f8();
      *unaff_A6 = candidate_6875a;
      return;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  candidate_140c0();
  candidate_688d6();
  return;
}
