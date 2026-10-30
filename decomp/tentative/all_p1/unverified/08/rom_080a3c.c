/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x080a3c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_80a3c(void)

{
  byte bVar1;
  int unaff_A6;
  bool bVar2;

  candidate_260c8();
  bVar1 = candidate_29f2();
  bVar2 = (char)(*(char *)(unaff_A6 + 0x8c) + '@') < '\0';
  *(char *)(unaff_A6 + 0x8c) = (-bVar2 ^ bVar1) + bVar2 + *(char *)(unaff_A6 + 0x8c);
  return;
}
