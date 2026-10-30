/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x030f76.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_30f76(void)

{
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x3c;
  *unaff_A6 = 0x30f82;
  *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
  if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
    *unaff_A6 = candidate_4188;
  }
  *(char *)(unaff_A6[3] + 0x60) = *(char *)(unaff_A6[3] + 0x60) + '\x01';
  candidate_427a();
  return;
}
