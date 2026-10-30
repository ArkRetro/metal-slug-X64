/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ce026.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ce026(void)

{
  undefined4 *unaff_A6;

  candidate_1b1dc();
  unaff_A6[0xd] = 0x21c00;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0xffdc;
  candidate_4256();
  *unaff_A6 = 0xce050;
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    candidate_18f40();
    candidate_4256();
    *unaff_A6 = 0xce07c;
    candidate_1abc8();
  }
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    *unaff_A6 = candidate_ce098;
  }
  return;
}
