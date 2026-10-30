/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd376.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd376(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 uVar2;

  candidate_4256();
  *unaff_A6 = 0xcd38c;
  uVar2 = 0;
  candidate_ce6b2();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_cd376;
  }
  if (*(char *)(unaff_A6[3] + 0xa1) == '\x01') {
    *unaff_A6 = 0xcd2d4;
  }
  if (*(char *)(unaff_A6[3] + 0xa1) == '\x03') {
    *unaff_A6 = candidate_cd3cc;
  }
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -3;
  if ((*(char *)(unaff_A6[3] + 0xa1) != '\0') &&
     (*(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1,
     *(short *)(unaff_A6 + 0x26) < 1)) {
    candidate_40bc();
    *(undefined2 *)(unaff_A6 + 0x26) = 0x3c;
  }
  bVar1 = false;
  candidate_409a();
  if (!bVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
