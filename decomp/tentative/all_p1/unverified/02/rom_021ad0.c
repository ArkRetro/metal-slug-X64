/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x021ad0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_21ad0(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_4256();
  *unaff_A6 = 0x21ae6;
  candidate_1436a();
  candidate_1ad8c();
  candidate_427a();
  bVar1 = *(char *)((int)unaff_A6 + 0x81) == '\0';
  if (*(char *)((int)unaff_A6 + 0x81) == '\x01') {
    candidate_22178();
    if (bVar1) {
      *unaff_A6 = candidate_21e24;
    }
  }
  else if ((*(short *)(unaff_A6 + 0x1c) != 0) &&
          (*(short *)(unaff_A6 + 0x1c) <= *(short *)(unaff_A5 + 0x699e))) {
    *unaff_A6 = candidate_21f4e;
  }
  candidate_220bc();
  return;
}
