/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x026e08.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_26e08(void)

{
  undefined4 *unaff_A6;

  candidate_9bb0();
  candidate_1da40();
  candidate_1dbaa();
  *unaff_A6 = 0x26e28;
  unaff_A6[0xd] = unaff_A6[0xd] + -0x1000;
  if ((int)unaff_A6[0xd] < -0x20000) {
    unaff_A6[0xd] = 0xfffe0000;
    *unaff_A6 = 0x26e4a;
  }
  candidate_1ac38();
  if (*(short *)(unaff_A6 + 0x10) < 0x60) {
    candidate_18f40();
    *(char *)(unaff_A6 + 0x22) = *(char *)(unaff_A6 + 0x22) + '\x01';
    *unaff_A6 = candidate_26e7a;
  }
  candidate_427a();
  candidate_27bd4();
  return;
}
