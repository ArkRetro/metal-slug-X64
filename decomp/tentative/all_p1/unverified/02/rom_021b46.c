/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x021b46.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_21b46(void)

{
  undefined4 *unaff_A6;

  candidate_9f88();
  unaff_A6[0x14] = 0xe005e;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  candidate_9ea8();
  candidate_146e8();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
  }
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x81) = 2;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  *(undefined1 *)(unaff_A6 + 0x20) = 0;
  *unaff_A6 = candidate_21bb2;
  return;
}
