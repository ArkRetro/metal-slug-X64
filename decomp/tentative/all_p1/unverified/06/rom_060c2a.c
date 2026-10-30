/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x060c2a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_60c2a(void)

{
  undefined4 *unaff_A6;

  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  *(char *)(unaff_A6[3] + 0x61) = *(char *)(unaff_A6[3] + 0x61) + '\x01';
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
  *unaff_A6 = 0x60c4a;
  if ((*(char *)((int)unaff_A6 + 0x61) == '\0') || ((*(ushort *)(unaff_A6[3] + 0x72) & 8) == 0)) {
    candidate_427a();
  }
  if (*(char *)(unaff_A6[3] + 0x60) == -1) {
    *unaff_A6 = candidate_60c80;
  }
  return;
}
