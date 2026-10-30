/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6130.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6130(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1ad6e();
  candidate_9ea8();
  *(undefined1 *)((int)unaff_A6 + 0xa6) = 8;
  candidate_4256();
  *unaff_A6 = 0xd6168;
  candidate_d60c8();
  *(short *)(unaff_A6 + 0xb) = (short)*(char *)(unaff_A6[3] + 0x7c);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)(unaff_A6 + 0x20) + *(short *)(unaff_A6[3] + 0x1e);
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    candidate_4256();
  }
  uVar1 = 0;
  if (*(char *)(unaff_A6[3] + 0x61) != '\0') {
    candidate_4256();
    uVar1 = 0;
    candidate_8fa1a();
  }
  candidate_19424();
  if ((bool)uVar1) {
    *unaff_A6 = 0x8eebc;
  }
  return;
}
