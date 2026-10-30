/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x091290.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_91290(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_9ea8();
  *(short *)(unaff_A6 + 10) = (short)*(char *)(unaff_A6 + 0x22) + *(short *)(unaff_A6 + 10);
  *(short *)(unaff_A6 + 0xb) = *(char *)((int)unaff_A6 + 0x89) + -8 + *(short *)(unaff_A6 + 0xb);
  candidate_4256();
  *unaff_A6 = 0x912d6;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
