/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d4678.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d4678(void)

{
  char cVar1;
  undefined4 *unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x1d) = 0x3c;
  *unaff_A6 = 0xd4684;
  candidate_d5506();
  candidate_d54ee();
  cVar1 = *(char *)(unaff_A6 + 0x1d) + -1;
  *(char *)(unaff_A6 + 0x1d) = cVar1;
  if (cVar1 == '\0') {
    *unaff_A6 = candidate_d46ca;
  }
  if ((*(byte *)(unaff_A6 + 0x1d) & 1) == 0) {
    candidate_427a();
  }
  cVar1 = *(char *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e));
  *(undefined1 *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e)) = 0;
  if (cVar1 == '\x05') {
    *unaff_A6 = candidate_d470e;
  }
  return;
}
