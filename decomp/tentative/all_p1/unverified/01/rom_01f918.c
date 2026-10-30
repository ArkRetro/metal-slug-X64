/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01f918.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1f918(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x23) & 4) == 0) {
    candidate_c88b0();
  }
  *unaff_A6 = 0x1f934;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_203de();
  if ((*(byte *)(unaff_A6 + 0x23) & 0x80) == 0) {
    if ((*(byte *)(unaff_A6 + 0x23) & 2) == 0) {
      sVar1 = *(short *)(unaff_A6 + 0x1f) + -1;
      *(short *)(unaff_A6 + 0x1f) = sVar1;
      if (sVar1 < 0) {
        *(undefined2 *)(unaff_A6 + 0x1f) = *(undefined2 *)(unaff_A6 + 0x1c);
        candidate_203fc();
      }
    }
    else if (*(short *)(unaff_A6 + 0x1f) == 0) {
      if (*(short *)(unaff_A6 + 0x20) == 0) {
        candidate_203fc();
        *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + 1;
        if (*(short *)((int)unaff_A6 + 0x7e) < *(short *)((int)unaff_A6 + 0x76)) {
          *(undefined2 *)(unaff_A6 + 0x20) = *(undefined2 *)(unaff_A6 + 0x1e);
        }
        else {
          *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
          *(undefined2 *)(unaff_A6 + 0x1f) = *(undefined2 *)(unaff_A6 + 0x1c);
        }
      }
      else {
        *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
      }
    }
    else {
      *(short *)(unaff_A6 + 0x1f) = *(short *)(unaff_A6 + 0x1f) + -1;
    }
  }
  return;
}
