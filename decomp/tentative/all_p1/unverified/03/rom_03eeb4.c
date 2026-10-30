/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03eeb4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3eeb4(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  candidate_4256();
  *unaff_A6 = 0x3eeca;
  uVar1 = 0;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar1) {
    bVar2 = false;
    if (*(short *)(unaff_A6 + 0x1c) == 0) {
      bVar2 = false;
      candidate_3fc26();
      if (bVar2) {
        *unaff_A6 = candidate_3efc0;
      }
      else {
        candidate_3f138();
      }
    }
    else {
      candidate_3fb40();
      if (bVar2) {
        *unaff_A6 = candidate_3efc0;
      }
      else {
        candidate_3f138();
      }
    }
  }
  candidate_3fada();
  return;
}
