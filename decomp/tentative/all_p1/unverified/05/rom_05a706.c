/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05a706.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5a706(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else {
    candidate_1436a();
  }
  candidate_427a();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_5a734;
    in_CF = 0;
  }
  candidate_4c24();
  if ((bool)in_CF) {
    candidate_5a894();
    in_CF = *(short *)((int)unaff_A6 + 0x72) != -1;
    if (*(short *)((int)unaff_A6 + 0x72) == -1) {
      candidate_406b6();
    }
    else {
      *unaff_A6 = candidate_9b596;
      in_CF = 0;
    }
  }
  candidate_1945c();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
