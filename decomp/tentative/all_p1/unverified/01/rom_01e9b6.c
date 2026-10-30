/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01e9b6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1e9b6(void)

{
  undefined4 *unaff_A6;

  candidate_146e8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  if (*(char *)(unaff_A6 + 0x22) != '\x01') {
    candidate_9ea8();
    candidate_4256();
    *unaff_A6 = candidate_1e9f8;
    return;
  }
  candidate_9ea8();
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0x7f;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0x7f;
  *unaff_A6 = candidate_1eaac;
  return;
}
