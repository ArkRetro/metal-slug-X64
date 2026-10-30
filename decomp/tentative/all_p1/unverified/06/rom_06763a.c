/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06763a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6763a(void)

{
  uint unaff_A2;
  uint uVar1;
  int unaff_A5;

  uVar1 = 0x10c572;
  while (unaff_A2 <= uVar1) {
    *(uint *)(unaff_A5 + 0x4586) = uVar1;
    candidate_66ac2();
    candidate_66d84();
    uVar1 = *(int *)(unaff_A5 + 0x4586) - 0x10;
  }
  return;
}
