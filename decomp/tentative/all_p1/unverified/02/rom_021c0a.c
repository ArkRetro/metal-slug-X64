/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x021c0a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_21c0a(void)

{
  undefined4 *unaff_A6;

  unaff_A6[0x14] = 0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  candidate_40bc();
  candidate_1acf2();
  *(undefined1 *)(unaff_A6 + 0x20) = 2;
  *unaff_A6 = 0x21c48;
  candidate_1436a();
  candidate_1ad8c();
  candidate_427a();
  if (*(char *)(unaff_A6 + 0x20) == '\x03') {
    *unaff_A6 = candidate_21c70;
  }
  candidate_220bc();
  return;
}
