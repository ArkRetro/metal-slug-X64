/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d600c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d600c(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1ad6e();
  *(undefined4 *)((int)unaff_A6 + 0x92) = 0xe91fc;
  unaff_A6[0x14] = 0xe9144;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 100;
  candidate_9ea8();
  candidate_4520();
  candidate_4256();
  *unaff_A6 = 0xd605e;
  uVar1 = 0;
  candidate_d7126();
  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d60b4;
  }
  if (*(char *)(unaff_A6[3] + 0x79) != '\0') {
    *unaff_A6 = candidate_d60b4;
  }
  if (*(char *)(unaff_A6[3] + 0x7a) == '\x06') {
    *unaff_A6 = candidate_d60b4;
  }
  return;
}
