/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd3cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd3cc(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 uVar2;

  candidate_4256();
  *unaff_A6 = 0xcd3e2;
  uVar2 = 0;
  candidate_ce6b2();
  candidate_427a();
  if ((bool)uVar2) {
    candidate_4256();
  }
  if (*(char *)(unaff_A6[3] + 0xa1) == '\x01') {
    *unaff_A6 = 0xcd2d4;
  }
  if (*(char *)(unaff_A6[3] + 0xa1) == '\x02') {
    *unaff_A6 = candidate_cd376;
  }
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -7;
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
