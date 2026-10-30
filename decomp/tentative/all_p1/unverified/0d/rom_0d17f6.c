/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d17f6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d17f6(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined1 *)(unaff_A6 + 0x18) = 3;
  candidate_1ad6e();
  *unaff_A6 = 0xd1810;
  candidate_d1b46();
  candidate_d1b28();
  if (*(char *)(unaff_A6 + 0x18) != '\x03') {
    *unaff_A6 = 0xd166a;
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar1 = false;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  candidate_19424();
  if (bVar1) {
    *unaff_A6 = 0xd166a;
  }
  return;
}
