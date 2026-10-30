/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd2a4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd2a4(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x800;
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)(unaff_A6 + 0x26) = 0x3c;
  candidate_4256();
  *unaff_A6 = 0xcd2f0;
  uVar1 = 0;
  candidate_ce6b2();
  candidate_427a();
  if ((bool)uVar1) {
    candidate_4256();
  }
  if (*(char *)(unaff_A6[3] + 0xa1) == '\x02') {
    *unaff_A6 = candidate_cd376;
  }
  if (*(char *)(unaff_A6[3] + 0xa1) == '\x03') {
    *unaff_A6 = candidate_cd3cc;
  }
  if ((*(char *)(unaff_A6[3] + 0xa1) != '\0') &&
     (*(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1,
     *(short *)(unaff_A6 + 0x26) < 1)) {
    candidate_40bc();
    *(undefined2 *)(unaff_A6 + 0x26) = 0x3c;
  }
  bVar2 = false;
  candidate_409a();
  if (!bVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
