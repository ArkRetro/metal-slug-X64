/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04893e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4893e(void)

{
  undefined4 *unaff_A6;

  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_1abc8();
  candidate_427a();
  if (*(char *)(unaff_A6[3] + 0x60) == '\x01') {
    *unaff_A6 = candidate_48982;
  }
  candidate_49030();
  return;
}
