/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02b72c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_2b72c(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1ad6e();
  *unaff_A6 = 0x2b74a;
  candidate_1abc8();
  uVar1 = 0;
  if ((_DAT_0010000e & 3) == 0) {
    uVar1 = false;
    candidate_40bc();
    if ((bool)uVar1) {
      candidate_1b220();
      uVar1 = 1;
    }
  }
  candidate_409a();
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
