/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a0356.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a0356(void)

{
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else if ((*(byte *)(unaff_A6 + 0x90) & 2) == 0) {
    if (*(code **)(unaff_A6 + 0x7a) == (code *)0x0) {
      candidate_1436a();
    }
    else {
      (**(code **)(unaff_A6 + 0x7a))();
      candidate_a047e();
    }
  }
  else {
    *(undefined4 *)(unaff_A6 + 0x62) = 0;
    candidate_1436a();
  }
  return;
}
