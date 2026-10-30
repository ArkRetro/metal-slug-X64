/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d455a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d455a(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_d5506();
  candidate_d54ee();
  candidate_427a();
  if ((bool)in_CF) {
    *unaff_A6 = 0xd44ca;
    in_CF = 0;
  }
  candidate_4c24();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_d45ee;
  }
  cVar1 = *(char *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e));
  *(undefined1 *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e)) = 0;
  if (cVar1 == '\x05') {
    *unaff_A6 = candidate_d470e;
  }
  return;
}
