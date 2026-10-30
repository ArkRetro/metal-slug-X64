/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x022b34.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_22b34(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_18f92();
  *(undefined2 *)(unaff_A5 + 0x69b8) = 0xe0;
  *unaff_A6 = 0x22b46;
  if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
    candidate_1abc8();
    return;
  }
  candidate_120f8();
  *(undefined1 *)((int)unaff_A6 + 0x8b) = 7;
  *(undefined1 *)(unaff_A6 + 0x23) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x8e) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 2;
  candidate_1da40();
  candidate_1da40();
  candidate_1da40();
  *unaff_A6 = 0x22ba2;
  candidate_1abc8();
  if (*(char *)((int)unaff_A6 + 0x8e) == '\0') {
    *unaff_A6 = candidate_22bb8;
  }
  return;
}
