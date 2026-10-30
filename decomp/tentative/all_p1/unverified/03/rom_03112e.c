/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03112e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3112e(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;

  if (*(short *)(unaff_A6 + 0x20) == 0) {
    uVar1 = 0x30;
  }
  else {
    uVar1 = 0xb8;
  }
  *(undefined2 *)(unaff_A6 + 0xf) = uVar1;
  *(undefined2 *)(unaff_A6 + 0x10) = 0x174;
  candidate_9ea8();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  candidate_4256();
  *unaff_A6 = 0x311a0;
  candidate_427a();
  if (*(short *)(unaff_A6[3] + 0x7e) < 0xc) {
    *unaff_A6 = candidate_311bc;
  }
  return;
}
